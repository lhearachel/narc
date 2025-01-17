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
#include <narc/api/files.h>
#include <narc/defs/fatb.h>
#include <narc/defs/fimg.h>

enum narc_error narc_files_getimg(const struct narc *narc, const struct vfs_ctx *vfs_ctx, const uint16_t file_idx, const unsigned char **out_image, uint32_t *out_size)
{
    struct fatb_meta *fatb_meta = (struct fatb_meta *)(narc->vfs + vfs_ctx->fatb_ofs);
    if (fatb_meta->num_files == 0 || file_idx > fatb_meta->num_files - 1) {
        return NARCERR_FILES_OUT_OF_RANGE;
    }

    const unsigned char *fatb_st = narc->vfs + vfs_ctx->fatb_ofs + sizeof(*fatb_meta);
    struct fatb_entry *fatb_idx = (struct fatb_entry *)(fatb_st + (file_idx * sizeof(struct fatb_entry)));

    *out_image = narc->vfs + vfs_ctx->fimg_ofs + sizeof(struct fimg_meta) + fatb_idx->start;
    *out_size = fatb_idx->end - fatb_idx->start;
    return NARCERR_NONE;
}
