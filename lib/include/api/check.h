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
#ifndef NARC_API_CHECK_H
#define NARC_API_CHECK_H

#include <defs/error.h>
#include <defs/narc.h>

#include <stddef.h>

/*
 * Validate the integrity of a NARC's header and data sections. If all checks
 * succeed, then `out_vfs_ctx` will be populated as in `narc_check_vfs`.
 *
 * This routine may emit any error which would also be emitted by either of
 * `narc_check_header` and/or `narc_check_vfs`.
 */
enum narc_error narc_check(const struct narc *narc, struct vfs_ctx *out_vfs_ctx);

/*
 * Validate the integrity of a NARC's header. Unlike the section checks, the
 * size of the header - which is a constant - is not emitted.
 *
 * This routine will not validate the `size` field from the header; if an
 * expected value for `size` is known, then `narc_check_header_fsize` should be
 * used instead.
 *
 * This routine may emit any of the following error codes to signal an invalid
 * header:
 * - `NARCERR_MAGIC`
 * - `NARCERR_BOM`
 * - `NARCERR_VERSION`
 * - `NARCERR_HEADER_SIZE`
 * - `NARCERR_NUM_SECTIONS`
 */
enum narc_error narc_check_header(const struct narc *narc);

/*
 * Validate the integrity of a NARC's header, including the `size` field.
 *
 * In addition to any error which would also be emitted by `narc_check_header`,
 * this routine may also emit `NARCERR_FILE_SIZE` to signal an invalid header.
 */
enum narc_error narc_check_header_fsize(const struct narc *narc, const size_t fsize);

/*
 * Validate the integrity of a NARC's virtual filesystem (VFS). If all checks
 * succeed, then `out_vfs_ctx` will be populated with the byte-offsets of each
 * data section (FATB, FNTB, FIMG) and the total size of the VFS. The calling
 * client is responsible for providing the memory space for `out_vfs_ctx`.
 */
enum narc_error narc_check_vfs(const struct narc *narc, struct vfs_ctx *out_vfs_ctx);

/*
 * Validate the integrity of a VFS block as if it were the start of a NARC's
 * FATB section. If the VFS block is a valid FATB section, then `out_size` will
 * contain the total size of the section.
 */
enum narc_error narc_check_fatb(const unsigned char vfs[], uint32_t *out_size);

/*
 * Validate the integrity of a VFS block as if it were the start of a NARC's
 * FNTB section. If the VFS block is a valid FNTB section, then `out_size` will
 * contain the total size of the section.
 */
enum narc_error narc_check_fntb(const unsigned char vfs[], uint32_t *out_size);

/*
 * Validate the integrity of a VFS block as if it were the start of a NARC's
 * FIMG section. If the VFS block is a valid FIMG section, then `out_size` will
 * contain the total size of the section.
 */
enum narc_error narc_check_fimg(const unsigned char vfs[], uint32_t *out_size);

#endif // NARC_API_CHECK_H
