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
#ifndef NARC_API_PACK_H
#define NARC_API_PACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include <defs/error.h>
#include <defs/vfs.h>

#include <stdint.h>

/*
 * Start a packing context. The returned structure should be passed to
 * successive calls to `narc_pack_file` while building the virtual filesystem.
 */
struct vfs_pack_ctx *narc_pack_start(void);

/*
 * Prematurely halt a packing context. All owned allocations will be cleaned up
 * and freed.
 */
void narc_pack_halt(struct vfs_pack_ctx *ctx);

/*
 * Add a file image to the packing context. The context will claim ownership of
 * the image to be packed. Invoking `narc_pack_halt` after this routine will thus
 * destroy the underlying file image.
 */
void narc_pack_file(struct vfs_pack_ctx *ctx, unsigned char *image, const uint32_t size);

/*
 * Add a copy of the input file image to the packing context. The context will
 * claim ownership of the image to be packed. Invoking `narc_pack_halt` after
 * this routine will thus destroy the underlying file image.
 */
void narc_pack_file_copy(struct vfs_pack_ctx *ctx, unsigned char *image, const uint32_t size);

/*
 * Construct a NARC in-memory from the given packing context. As a side effect,
 * the packing context structure will be destroyed and freed.
 */
struct narc *narc_pack(struct vfs_pack_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif // NARC_API_PACK_H
