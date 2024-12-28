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

#include <stdio.h>
#include <api/check.h>

#define FNTB_MAGIC 0x464E5442

enum narc_error narc_check_fntb(const unsigned char vfs[], size_t *out_size)
{
    uint32_t *magic = (uint32_t *)vfs;
    uint32_t *size = (uint32_t *)vfs + 1;

    if (FNTB_MAGIC != *magic) {
        return NARCERR_FNTB_MAGIC;
    }

    // The FNTB section is not guaranteed to be word-aligned; the specification
    // absolves this by tail-padding the section with 0xFF to align the read
    // cursor onto the start of a word, if necessary. We do the same here to
    // make the validation code easier to read and shunt the responsibility of
    // ignoring any padding content onto interpretation routines.
    *out_size = *size + ((4 - (*size & 3)) & 3);
    return NARCERR_NONE;
}
