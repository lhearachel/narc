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
#ifndef NARC_DEFS_FIMG_H
#define NARC_DEFS_FIMG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define FIMG_MAGIC 0x46494D47

struct fimg_meta {
    uint32_t magic;
    uint32_t size;
};

struct fimg {
    uint32_t size;
    unsigned char image[];
};

#ifdef __cplusplus
}
#endif

#endif // NARC_DEFS_FIMG_H
