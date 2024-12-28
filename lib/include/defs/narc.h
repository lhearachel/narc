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
#ifndef NARC_DEFS_NARC_H
#define NARC_DEFS_NARC_H

#include <stdint.h>

#define HEADER_MAGIC          0x4352414E
#define HEADER_LE_BOM         0xFFFE
#define HEADER_VERSION_MARKER 0x0100
#define HEADER_NUM_SECTIONS   3

struct narc {
    uint32_t magic;
    uint16_t bom;
    uint16_t version;
    uint32_t size;
    uint16_t header_size;
    uint16_t num_sections;
    unsigned char vfs[];
};

#endif // NARC_DEFS_NARC_H
