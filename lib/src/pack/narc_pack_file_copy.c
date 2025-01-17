/*
 * Copyright 2025 <lhearachel@proton.me>
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

#include <stdlib.h>
#include <string.h>

void narc_pack_file_copy(struct vfs_pack_ctx *ctx, unsigned char *image, const uint32_t size)
{
    unsigned char *copy = malloc(size);
    memcpy(copy, image, size);

    narc_pack_file(ctx, copy, size);
}
