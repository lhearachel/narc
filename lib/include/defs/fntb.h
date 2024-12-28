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
#ifndef NARC_DEFS_FNTB_H
#define NARC_DEFS_FNTB_H

#include <stdint.h>

struct fntb_meta {
    uint32_t magic;
    uint32_t size;
};

struct fntb_main_entry {
    uint32_t subt_offset;
    uint16_t first_file;
    uint16_t directory;
};

#endif // NARC_DEFS_FNTB_H
