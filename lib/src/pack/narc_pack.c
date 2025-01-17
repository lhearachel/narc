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
#include <narc/defs/vfs.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void fill_narc_header(struct narc *narc, const struct vfs_pack_ctx *ctx);
static void fill_fatb_header(unsigned char *dest, const uint32_t size, const uint16_t num_files);
static void fill_fntb_header(unsigned char *dest, const uint32_t size);
static void fill_fimg_header(unsigned char *dest, const uint32_t size);
static void fill_fntb_main(unsigned char *dest);

struct narc *narc_pack(struct vfs_pack_ctx *ctx)
{
    struct narc *narc = malloc(ctx->size);
    if (narc == NULL) {
        return NULL;
    }

    uint32_t fatb_size = sizeof(struct fatb_meta) + (ctx->count * sizeof(struct fatb_entry));
    uint32_t fntb_size = sizeof(struct fntb_meta) + sizeof(struct fntb_main_entry);
    struct vfs_ctx vfs_ctx = {
        .fatb_ofs = 0,
        .fntb_ofs = fatb_size,
        .fimg_ofs = fatb_size + fntb_size,
        .vfs_size = ctx->size - sizeof(struct narc),
    };

    fill_narc_header(narc, ctx);
    fill_fatb_header(narc->vfs, fatb_size, ctx->count);
    fill_fntb_header(narc->vfs + vfs_ctx.fntb_ofs, fntb_size);
    fill_fntb_main(narc->vfs + vfs_ctx.fntb_ofs + sizeof(struct fntb_meta));

    unsigned char *fatb_dst = narc->vfs + sizeof(struct fatb_meta);
    unsigned char *fimg_src = narc->vfs + vfs_ctx.fimg_ofs + sizeof(struct fimg_meta);
    unsigned char *fimg_dst = fimg_src;
    struct vfs_file *curr = ctx->head;

    while (curr != NULL) {
        int fimg_len_pad = ((4 - (curr->size & 3)) & 3);
        struct fatb_entry fatb = {
            .start = fimg_dst - fimg_src,
            .end = (fimg_dst - fimg_src) + curr->size,
        };

        memcpy(fatb_dst, &fatb, sizeof(fatb));
        memcpy(fimg_dst, curr->image, curr->size);
        memset(fimg_dst + curr->size, 0xFF, fimg_len_pad);

        fatb_dst += sizeof(fatb);
        fimg_dst += curr->size + fimg_len_pad;
        curr = curr->next;
    }

    fill_fimg_header(narc->vfs + vfs_ctx.fimg_ofs, fimg_dst - fimg_src + sizeof(struct fimg_meta));
    narc_pack_halt(ctx); // clean up the packing context
    return narc;
}

static void fill_narc_header(struct narc *narc, const struct vfs_pack_ctx *ctx)
{
    narc->magic = HEADER_MAGIC;
    narc->bom = HEADER_LE_BOM;
    narc->version = HEADER_VERSION_MARKER;
    narc->size = ctx->size;
    narc->header_size = sizeof(struct narc);
    narc->num_sections = HEADER_NUM_SECTIONS;
}

static void fill_fatb_header(unsigned char *dest, const uint32_t size, const uint16_t num_files)
{
    struct fatb_meta fatb = {
        .magic = FATB_MAGIC,
        .size = size,
        .num_files = num_files,
        ._reserved = 0,
    };
    memcpy(dest, &fatb, sizeof(fatb));
}

static void fill_fntb_header(unsigned char *dest, const uint32_t size)
{
    struct fntb_meta fntb = {
        .magic = FNTB_MAGIC,
        .size = size,
    };
    memcpy(dest, &fntb, sizeof(fntb));
}

static void fill_fimg_header(unsigned char *dest, const uint32_t size)
{
    struct fimg_meta fimg = {
        .magic = FIMG_MAGIC,
        .size = size,
    };
    memcpy(dest, &fimg, sizeof(fimg));
}

static void fill_fntb_main(unsigned char *dest)
{
    struct fntb_main_entry main = {
        .subt_offset = 4,
        .first_file = 0,
        .directory = 1,
    };
    memcpy(dest, &main, sizeof(main));
}
