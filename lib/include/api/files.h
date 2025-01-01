/*
 * Copyright 2024 <lhearachel@proton.me>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef NARC_API_FILES_H
#define NARC_API_FILES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <defs/error.h>
#include <defs/narc.h>
#include <defs/vfs.h>

/*
 * The maximum string length of any extension which may be returned by
 * `narc_files_getext`.
 */
#define MAX_EXT_LEN 8

/*
 * Return the extension of a file image. This routine makes a best-guess
 * effort; if the file image's magic marker is not recognized, then the
 * returned extension will be “.bin”. The returned value is returned as a new
 * string allocation, and the calling client is responsible for freeing this
 * allocation.
 */
char *narc_files_getext(const char *data);

/*
 * Return a pointer to the file image corresponding to the input index, if any
 * exists. On success, `out_image` will be assigned to the location of said file
 * image, and `out_size` will be assigned to the size of that image (in bytes).
 */
enum narc_error narc_files_getimg(const struct narc *narc, const struct vfs_ctx *vfs_ctx, const uint16_t file_idx, const unsigned char **out_image, uint32_t *out_size);

#ifdef __cplusplus
}
#endif

#endif // NARC_API_FILES_H
