/* nbdkit
 * Copyright (C) 2013-2020 Red Hat Inc.
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

#ifndef NBDKIT_COMPAT_H
#define NBDKIT_COMPAT_H

#if !defined (NBDKIT_PLUGIN_H) && !defined (NBDKIT_FILTER_H)
#error this header file should not be directly included
#endif

#if defined(__MINGW32__) || defined(__CYGWIN__) || defined(_MSC_VER)
#define WINDOWS_COMPAT
#endif

struct nbdkit_extents;

#if defined(WINDOWS_COMPAT)
#include <string.h>
#include <stdarg.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WINDOWS_COMPAT
struct nbdkit_functions {
  void (*nbdkit_verror) (const char *msg, va_list args);
  void (*nbdkit_vdebug) (const char *msg, va_list args);
  char * (*nbdkit_absolute_path) (const char *path);
  int64_t (*nbdkit_parse_size) (const char *str);
  int (*nbdkit_parse_bool) (const char *str);
  int (*nbdkit_parse_int) (const char *what, const char *str, int *r);
  int (*nbdkit_parse_unsigned) (const char *what, const char *str, unsigned *r);
  int (*nbdkit_parse_int8_t) (const char *what, const char *str, int8_t *r);
  int (*nbdkit_parse_uint8_t) (const char *what, const char *str, uint8_t *r);
  int (*nbdkit_parse_int16_t) (const char *what, const char *str, int16_t *r);
  int (*nbdkit_parse_uint16_t) (const char *what, const char *str, uint16_t *r);
  int (*nbdkit_parse_int32_t) (const char *what, const char *str, int32_t *r);
  int (*nbdkit_parse_uint32_t) (const char *what, const char *str, uint32_t *r);
  int (*nbdkit_parse_int64_t) (const char *what, const char *str, int64_t *r);
  int (*nbdkit_parse_uint64_t) (const char *what, const char *str, uint64_t *r);
  int (*nbdkit_read_password) (const char *value, char **password);
  char * (*nbdkit_realpath) (const char *path);
  int (*nbdkit_nanosleep) (unsigned sec, unsigned nsec);
  const char * (*nbdkit_export_name) (void);
  int (*nbdkit_peer_name) (void *addr, void *addrlen);
  void (*nbdkit_shutdown) (void);
  
  struct nbdkit_extents *(*nbdkit_extents_new) (uint64_t start, uint64_t end);
  void (*nbdkit_extents_free) (struct nbdkit_extents *);
  size_t (*nbdkit_extents_count) (const struct nbdkit_extents *);
  struct nbdkit_extent (*nbdkit_get_extent) (const struct nbdkit_extents *, size_t);
  int (*nbdkit_add_extent) (struct nbdkit_extents *, uint64_t offset, uint64_t length, uint32_t type);

  void (*nbdkit_set_error) (int err);
};

struct nbdkit_functions _nbdkit_functions;
#endif

#ifdef __cplusplus
}
#endif
#endif
#endif
