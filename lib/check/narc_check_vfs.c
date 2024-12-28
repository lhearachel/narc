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

#define ERROR_RET(expect, actual)           \
    {                                       \
        if ((ret = (actual)) != (expect)) { \
            return ret;                     \
        }                                   \
    }

enum narc_error narc_check_vfs(const struct narc *narc, struct vfs_ctx *out_vfs_ctx)
{
    uint32_t size;
    enum narc_error ret;

    out_vfs_ctx->fatb_ofs = 0;
    ERROR_RET(NARCERR_NONE, narc_check_fatb(narc->vfs, &size));
    out_vfs_ctx->fntb_ofs = out_vfs_ctx->fatb_ofs + size;
    ERROR_RET(NARCERR_NONE, narc_check_fntb(narc->vfs + out_vfs_ctx->fntb_ofs, &size));
    out_vfs_ctx->fimg_ofs = out_vfs_ctx->fntb_ofs + size;
    ERROR_RET(NARCERR_NONE, narc_check_fimg(narc->vfs + out_vfs_ctx->fimg_ofs, &size));
    out_vfs_ctx->vfs_size = out_vfs_ctx->fimg_ofs + size;

    return NARCERR_NONE;
}
