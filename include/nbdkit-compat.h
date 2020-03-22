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

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__) || \
    defined(_MSC_VER)
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
  void (*verror) (const char *msg, va_list args);
  void (*vdebug) (const char *msg, va_list args);
  char * (*absolute_path) (const char *path);
  int64_t (*parse_size) (const char *str);
  int (*parse_bool) (const char *str);
  int (*parse_int) (const char *what, const char *str, int *r);
  int (*parse_unsigned) (const char *what, const char *str, unsigned *r);
  int (*parse_int8_t) (const char *what, const char *str, int8_t *r);
  int (*parse_uint8_t) (const char *what, const char *str, uint8_t *r);
  int (*parse_int16_t) (const char *what, const char *str, int16_t *r);
  int (*parse_uint16_t) (const char *what, const char *str, uint16_t *r);
  int (*parse_int32_t) (const char *what, const char *str, int32_t *r);
  int (*parse_uint32_t) (const char *what, const char *str, uint32_t *r);
  int (*parse_int64_t) (const char *what, const char *str, int64_t *r);
  int (*parse_uint64_t) (const char *what, const char *str, uint64_t *r);
  int (*read_password) (const char *value, char **password);
  char * (*realpath) (const char *path);
  int (*nanosleep) (unsigned sec, unsigned nsec);
  const char * (*export_name) (void);
  int (*peer_name) (void *addr, void *addrlen);
  void (*shutdown) (void);
  
  struct nbdkit_extents *(*extents_new) (uint64_t start, uint64_t end);
  void (*extents_free) (struct nbdkit_extents *);
  size_t (*extents_count) (const struct nbdkit_extents *);
  struct nbdkit_extent (*get_extent) (const struct nbdkit_extents *, size_t);
  int (*add_extent) (struct nbdkit_extents *, uint64_t offset, uint64_t length,
                    uint32_t type);

  void (*set_error) (int err);
};

struct nbdkit_functions _nbdkit_functions;
#endif

#ifdef __cplusplus
}
#endif
#endif
#endif
