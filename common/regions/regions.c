/* nbdkit
 * Copyright (C) 2018 Red Hat Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of Red Hat nor the names of its contributors may be
 * used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RED HAT AND CONTRIBUTORS ''AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL RED HAT OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

#include <nbdkit-plugin.h>

#include "isaligned.h"
#include "regions.h"

void
init_regions (struct regions *regions)
{
  regions->regions = NULL;
  regions->nr_regions = 0;
}

void
free_regions (struct regions *regions)
{
  /* We don't need to free the data since that is not owned by the
   * regions structure.
   */
  free (regions->regions);
}

/* Find the region corresponding to the given offset.  Use region->end
 * to find the end of the region.
 */
static int
compare_offset (const void *offsetp, const void *regionp)
{
  const uint64_t offset = *(uint64_t *)offsetp;
  const struct region *region = (struct region *)regionp;

  if (offset < region->start) return -1;
  if (offset > region->end) return 1;
  return 0;
}

const struct region *
find_region (const struct regions *regions, uint64_t offset)
{
  return bsearch (&offset, regions->regions, regions->nr_regions,
                  sizeof (struct region), compare_offset);
}

/* This is the low level function for constructing the list of
 * regions.  It appends one region to the list, checking that the
 * invariants described above (about the regions being non-overlapping
 * and contiguous) is maintained.  Note it is not possible to
 * construct regions out of order using this function.
 */
static int __attribute__((__nonnull__ (1)))
append_one_region (struct regions *regions, struct region region)
{
  struct region *p;

  /* The assertions in this function are meant to maintain the
   * invariant about the array as described at the top of this file.
   */
  assert (region.start == virtual_size (regions));
  assert (region.len > 0);
  assert (region.end >= region.start);
  assert (region.len == region.end - region.start + 1);

  p = realloc (regions->regions,
               (regions->nr_regions+1) * sizeof (struct region));
  if (p == NULL) {
    nbdkit_error ("realloc: %m");
    return -1;
  }
  regions->regions = p;
  regions->regions[regions->nr_regions] = region;
  regions->nr_regions++;

  return 0;
}

static int
append_padding (struct regions *regions, uint64_t alignment)
{
  struct region region;

  assert (is_power_of_2 (alignment));

  region.start = virtual_size (regions);
  if (IS_ALIGNED (region.start, alignment))
    return 0;                   /* nothing to do */
  region.end = (region.start & ~(alignment-1)) + alignment - 1;
  region.len = region.end - region.start + 1;
  region.type = region_zero;
  region.description = "padding";
  return append_one_region (regions, region);
}

int
append_region_len (struct regions *regions,
                   const char *description, uint64_t len,
                   uint64_t pre_aligment, uint64_t post_alignment,
                   enum region_type type, ...)
{
  struct region region;

  /* Pre-alignment. */
  if (pre_aligment != 0) {
    if (append_padding (regions, pre_aligment) == -1)
      return -1;
    assert (IS_ALIGNED (virtual_size (regions), pre_aligment));
  }

  /* Main region. */
  region.description = description;
  region.start = virtual_size (regions);
  region.len = len;
  region.end = region.start + region.len - 1;
  region.type = type;
  if (type == region_file) {
    va_list ap;
    size_t i;

    va_start (ap, type);
    i = va_arg (ap, size_t);
    va_end (ap);
    region.u.i = i;
  }
  else if (type == region_data) {
    va_list ap;
    const unsigned char *data;

    va_start (ap, type);
    data = va_arg (ap, const unsigned char *);
    va_end (ap);
    region.u.data = data;
  }
  if (append_one_region (regions, region) == -1)
    return -1;

  /* Post-alignment. */
  if (post_alignment != 0) {
    if (append_padding (regions, post_alignment) == -1)
      return -1;
    assert (IS_ALIGNED (virtual_size (regions), post_alignment));
  }

  return 0;
}
