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

#include <api/check.h>

#define FATB_MAGIC  0x46415442 // Reversed due to cast during validation
#define HEADER_SIZE 12
#define ENTRY_SIZE  8

#define ERROR_NEQ(expect, actual, err) \
    {                                  \
        if ((expect) != (actual)) {    \
            return err;                \
        }                              \
    }

enum narc_error narc_check_fatb(const unsigned char vfs[], uint32_t *out_size)
{
    uint32_t *magic = (uint32_t *)vfs;
    uint32_t *size = (uint32_t *)vfs + 1;
    uint32_t *num_files = (uint32_t *)vfs + 2;

    ERROR_NEQ(FATB_MAGIC, *magic, NARCERR_FATB_MAGIC);
    ERROR_NEQ(0, *num_files >> 16, NARCERR_FATB_RESERVED);
    ERROR_NEQ((*num_files * ENTRY_SIZE) + HEADER_SIZE, *size, NARCERR_FATB_SIZE);

    *out_size = *size;
    return NARCERR_NONE;
}
