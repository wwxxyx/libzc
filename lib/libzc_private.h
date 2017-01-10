/*
 *  zc - zip crack library
 *  Copyright (C) 2013  Marc Ferland
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _LIBZC_PRIVATE_H_
#define _LIBZC_PRIVATE_H_

#include <stdbool.h>
#include <syslog.h>
#include <stdint.h>
#include <error.h>

#include "libzc.h"
#include "crc32.h"

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

static inline void __attribute__((always_inline, format(printf, 2, 3)))
zc_log_null(struct zc_ctx *UNUSED(ctx), const char *UNUSED(format), ...) {}

#define zc_log_cond(ctx, prio, arg...)                                  \
   do {                                                                 \
      if (zc_get_log_priority(ctx) >= prio)                             \
         zc_log(ctx, prio, __FILE__, __LINE__, __FUNCTION__, ## arg);   \
   } while (0)

#ifdef ENABLE_LOGGING
#  ifdef ENABLE_DEBUG
#    define dbg(ctx, arg...) zc_log_cond(ctx, LOG_DEBUG, ## arg)
#  else
#    define dbg(ctx, arg...) zc_log_null(ctx, ## arg)
#  endif
#  define info(ctx, arg...) zc_log_cond(ctx, LOG_INFO, ## arg)
#  define err(ctx, arg...) zc_log_cond(ctx, LOG_ERR, ## arg)
#else
#  define dbg(ctx, arg...) zc_log_null(ctx, ## arg)
#  define info(ctx, arg...) zc_log_null(ctx, ## arg)
#  define err(ctx, arg...) zc_log_null(ctx, ## arg)
#endif

#define fatal(arg...)                                                   \
   do {                                                                 \
       error_at_line(EXIT_FAILURE, 0, __FILE__, __LINE__, ## arg);      \
   } while (0)

#define ZC_EXPORT __attribute__ ((visibility("default")))

void zc_log(struct zc_ctx *ctx,
            int priority, const char *file, int line, const char *fn,
            const char *format, ...)
__attribute__((format(printf, 6, 7)));

#define MULT 134775813u
#define MULTINV 3645876429u  /* modular multiplicative inverse mod2^32 */
#define KEY0 0x12345678
#define KEY1 0x23456789
#define KEY2 0x34567890
#define ZIP_ENCRYPTION_HEADER_LENGTH 12

static inline
uint32_t pow2(uint32_t p)
{
    return (1 << p);
}

static inline
uint32_t mask_msb(uint32_t v)
{
    return (v & 0xff000000);
}

static inline
uint32_t mask_lsb(uint32_t v)
{
    return (v & 0x000000ff);
}

static inline
uint8_t msb(uint32_t v)
{
    return (v >> 24);
}

static inline
uint8_t lsb(uint32_t v)
{
    return (v & 0xff);
}

static inline
void update_keys(char c, struct zc_key *ksrc, struct zc_key *kdst)
{
    kdst->key0 = crc32(ksrc->key0, c);
    kdst->key1 = (ksrc->key1 + (kdst->key0 & 0xff)) * MULT + 1;
    kdst->key2 = crc32(ksrc->key2, kdst->key1 >> 24);
}

static inline
void set_default_encryption_keys(struct zc_key *k)
{
    k->key0 = KEY0;
    k->key1 = KEY1;
    k->key2 = KEY2;
}

/* zip file */
struct header {
    uint16_t version_needed;
    uint16_t gen_bit_flag;
    uint16_t comp_method;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint32_t crc32;
    uint32_t comp_size;
    uint32_t uncomp_size;
    uint16_t filename_length;
    uint16_t extra_field_length;
    char *filename;
};

struct zc_info {
    uint8_t enc_header[ZIP_ENCRYPTION_HEADER_LENGTH];
    uint8_t magic;
    int idx;
    long enc_header_offset;
    long begin_offset;
    long end_offset;
    struct header header;
    struct list_head header_list;
};

/* TODO: Do we really need a vdata_array? We could directly use the
 * list... */
size_t read_validation_data(struct zc_ctx *ctx,
                            const char *filename,
                            struct zc_validation_data **vdata_array,
                            size_t *nmemb);
struct list_head * zc_file_get_info_head(struct zc_file *file);

/* key array helper */
struct ka {
    uint32_t *array;
    size_t size;
    size_t capacity;
};
int ka_alloc(struct ka **a, size_t init_size);
void ka_free(struct ka *a);
void ka_append(struct ka *a, uint32_t key);
void ka_uniq(struct ka *a);
void ka_squeeze(struct ka *a);
void ka_empty(struct ka *a);
#ifdef ENABLE_DEBUG
#include <stdio.h>
void ka_print(struct ka *a, FILE *stream);
#endif

static inline
uint32_t ka_at(const struct ka *a, uint32_t index)
{
    return a->array[index];
}

static inline
void ka_swap(struct ka **a1, struct ka **a2)
{
    struct ka *t = *a1;
    *a1 = *a2;
    *a2 = t;
}

#endif
