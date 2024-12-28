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

#include <api/pack.h>
#include <defs/fatb.h>

#include <stdlib.h>

void narc_pack_file(struct vfs_pack_ctx *ctx, unsigned char *image, const uint32_t size)
{
    struct vfs_file *file = malloc(sizeof(struct vfs_file));
    file->next = NULL;
    file->image = image;
    file->size = size;

    // Individual file images are padded to word-alignment
    ctx->size += sizeof(struct fatb_entry) + size + ((4 - (size & 3)) & 3);

    if (ctx->count == 0) {
        ctx->head = file;
        ctx->tail = file;
        ctx->count = 1;
        return;
    }

    ctx->tail->next = file;
    ctx->tail = ctx->tail->next;
    ctx->count++;
}
