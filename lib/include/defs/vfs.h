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
#ifndef NARC_DEFS_VFS_H
#define NARC_DEFS_VFS_H

#include <stdint.h>

struct vfs_ctx {
    uint32_t fatb_ofs;
    uint32_t fntb_ofs;
    uint32_t fimg_ofs;
    uint32_t vfs_size;
};

#endif // NARC_DEFS_VFS_H
