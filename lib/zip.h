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

#ifndef _ZIP_H_
#define _ZIP_H_

#include <stdio.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ZIP_ENCRYPTION_HEADER_LENGTH 12

struct zip_header;
int zip_header_new(struct zip_header **header);
void zip_header_free(struct zip_header *header);
int zip_header_read(FILE *fd, struct zip_header *header);
bool zip_header_has_encryption_bit(const struct zip_header *header);
unsigned char zip_header_encryption_magic(const struct zip_header *header);
int zip_encryption_header_read(FILE *fd, unsigned char *enc_header);
int zip_skip_to_next_header(FILE *fd, const struct zip_header *header);
unsigned int zip_header_comp_size(const struct zip_header *header);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
