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

#define FIMG_MAGIC 0x46494D47

enum narc_error narc_check_fimg(const unsigned char vfs[], size_t *out_size)
{
    uint32_t *magic = (uint32_t *)vfs;
    uint32_t *size = (uint32_t *)vfs + 1;

    if (FIMG_MAGIC != *magic) {
        return NARCERR_FIMG_MAGIC;
    }

    *out_size = *size;
    return NARCERR_NONE;
}
