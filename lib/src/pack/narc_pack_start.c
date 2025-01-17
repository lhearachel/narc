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
#include <narc/defs/fatb.h>
#include <narc/defs/fimg.h>
#include <narc/defs/fntb.h>
#include <narc/defs/narc.h>

#include <stdlib.h>

struct vfs_pack_ctx *narc_pack_start(void)
{
    struct vfs_pack_ctx *ctx = calloc(1, sizeof(struct vfs_pack_ctx));
    if (ctx == NULL) {
        return NULL;
    }

    // Start with an initial size that can be updated as files are consumed.
    // Every valid NARC has:
    //   - A NARC header
    //   - A FATB header
    //   - A FNTB header
    //   - At least one FNTB main entry
    //   - A FIMG header
    ctx->size = sizeof(struct narc)
        + sizeof(struct fatb_meta)
        + sizeof(struct fntb_meta)
        + sizeof(struct fntb_main_entry)
        + sizeof(struct fimg_meta);

    return ctx;
}
