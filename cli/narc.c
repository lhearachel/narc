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

#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <narc.h>

int main(void)
{
    struct narc *narc = NULL;
    struct vfs_ctx vfs_ctx;

    enum narc_error err = narc_load("test.narc", &narc, &vfs_ctx);
    if (err == NARCERR_ERRNO) {
        fprintf(stderr, "System error: %s\n", strerror(errno));
        fflush(stderr);
        return EXIT_FAILURE;
    } else if (err != NARCERR_NONE) {
        fprintf(stderr, "NARC error: %s\n", narc_strerror(err));
        fflush(stderr);
        return EXIT_FAILURE;
    }

    printf("Loaded NARC successfully!\n");
    printf("NARC size: %d\n", narc->size);
    printf("NARC data sample:");
    for (int i = 0; i < 16; i++) {
        printf(" %02x", narc->vfs[i]);
    }
    printf("\n");
    printf("FATB offset: 0x%02x\n", vfs_ctx.fatb_ofs);
    printf("FNTB offset: 0x%02x\n", vfs_ctx.fntb_ofs);
    printf("FIMG offset: 0x%02x\n", vfs_ctx.fimg_ofs);
    printf("VFS size:    0x%02x\n", vfs_ctx.vfs_size);

    printf("Trying to dump contents...\n");
    err = narc_dump(narc, &vfs_ctx, "test.narc.d");
    if (err != NARCERR_NONE) {
        fprintf(stderr, "NARC error: %s\n", narc_strerror(err));
        fflush(stderr);
        return EXIT_FAILURE;
    }

    free(narc);
    return EXIT_SUCCESS;
}
