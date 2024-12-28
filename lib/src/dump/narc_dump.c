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

#include <api/dump.h>
#include <defs/fatb.h>
#include <defs/fimg.h>

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>

enum narc_error narc_dump(const struct narc *narc, const struct vfs_ctx *vfs_ctx, const char *dst_dir)
{
    if (mkdir(dst_dir, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == -1) {
        if (errno == ENOTDIR) {
            return NARCERR_DUMP_TARGET_IS_FILE;
        }

        if (errno != EEXIST) {
            return NARCERR_ERRNO;
        }
    }

    // Support only the base case for now: ignore any existing file-name table
    // and write images to disk with ID-derived names.
    struct fatb_meta *fatb_meta = (struct fatb_meta *)(narc->vfs + vfs_ctx->fatb_ofs);
    uint32_t fatb_entry_start = vfs_ctx->fatb_ofs + sizeof(struct fatb_meta);
    uint32_t fimg_entry_start = vfs_ctx->fimg_ofs + sizeof(struct fimg_meta);

    //                                         ------------ path separator
    //                                         |   -------- 6-digits for file ID
    //                                         v   v   v--- null terminator
    char *fname_buf = malloc(strlen(dst_dir) + 1 + 6 + 1);
    if (fname_buf == NULL) {
        return NARCERR_ERRNO;
    }

    strcpy(fname_buf, dst_dir);
    char *fname_bufp = fname_buf + strlen(dst_dir);
    fname_bufp[0] = '/';
    fname_bufp[7] = '\0';

    for (int i = 0; i < fatb_meta->num_files; i++) {
        struct fatb_entry *entry = (struct fatb_entry *)(narc->vfs + fatb_entry_start + (sizeof(struct fatb_entry) * i));
        const unsigned char *fimage = narc->vfs + fimg_entry_start + entry->start;
        uint32_t fimage_len = entry->end - entry->start;
        sprintf(&fname_bufp[1], "%05d", i);

        FILE *fout = fopen(fname_buf, "wb");
        if (fout == NULL) {
            return NARCERR_ERRNO;
        }

        fwrite(fimage, sizeof(unsigned char), fimage_len, fout);
        fclose(fout);
    }

    return NARCERR_NONE;
}
