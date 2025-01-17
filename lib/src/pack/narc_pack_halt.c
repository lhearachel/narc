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

#include <narc/api/pack.h>

#include <stdlib.h>

void narc_pack_halt(struct vfs_pack_ctx *ctx)
{
    if (ctx == NULL) {
        return;
    }

    struct vfs_file *next, *curr = ctx->head;

    while (curr != NULL) {
        next = curr->next;
        free(curr->image);
        free(curr);
        curr = next;
    }

    free(ctx);
}
