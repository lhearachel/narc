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
 * succeed, then `out_sizes` will be populated according to the output result of
 * `narc_check_vfs`.
 */
enum narc_error narc_check(const struct narc *narc, size_t out_sizes[3]);

/*
 * Validate the integrity of a NARC's header. Unlike the section checks, the
 * size of the header -- which is a constant -- is not emitted.
 *
 * This routine will not validate the `size` field from the header; if an
 * expected value for `size` is known, then use `narc_check_header_fsize`
 * instead.
 */
enum narc_error narc_check_header(const struct narc *narc);

/*
 * Validate the integrity of a NARC's header, including the `size` field.
 */
enum narc_error narc_check_header_fsize(const struct narc *narc, const size_t fsize);

/*
 * Validate the integrity of a NARC's virtual filesystem. If all checks succeed,
 * then the 3-element array at `out_sizes` will be populated with the sizes of
 * the individual sections in order (FATB, FNTB, FIMG). The calling client is
 * responsible for providing the memory space for `out_sizes`.
 */
enum narc_error narc_check_vfs(const struct narc *narc, size_t out_sizes[3]);

/*
 * Validate the integrity of a NARC's FATB section. If the FATB section is
 * valid, then `out_size` will contain the total size of the section.
 */
enum narc_error narc_check_fatb(const unsigned char vfs[], size_t *out_size);

/*
 * Validate the integrity of a NARC's FNTB section. If the FNTB section is
 * valid, then `out_size` will contain the total size of the section.
 */
enum narc_error narc_check_fntb(const unsigned char vfs[], size_t *out_size);

/*
 * Validate the integrity of a NARC's FIMG section. If the FIMG section is
 * valid, then `out_size` will contain the total size of the section.
 */
enum narc_error narc_check_fimg(const unsigned char vfs[], size_t *out_size);

#endif // NARC_API_CHECK_H
