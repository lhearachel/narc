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

#define NARC_MAGIC     0x4352414E
#define LE_BOM         0xFFFE
#define VERSION_MARKER 0x0100
#define HEADER_SIZE    16
#define NUM_SECTIONS   3

#define ERROR_NEQ(expect, actual, err) \
    {                                  \
        if ((expect) != (actual)) {    \
            return err;                \
        }                              \
    }

enum narc_error narc_check_header(const struct narc *narc)
{
    ERROR_NEQ(NARC_MAGIC, narc->magic, NARCERR_MAGIC);
    ERROR_NEQ(LE_BOM, narc->bom, NARCERR_BOM);
    ERROR_NEQ(VERSION_MARKER, narc->version, NARCERR_VERSION);
    ERROR_NEQ(HEADER_SIZE, narc->header_size, NARCERR_HEADER_SIZE);
    ERROR_NEQ(NUM_SECTIONS, narc->num_sections, NARCERR_NUM_SECTIONS);

    return NARCERR_NONE;
}
