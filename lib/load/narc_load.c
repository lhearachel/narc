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
#include <api/error.h>
#include <api/load.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADER_SIZE sizeof(struct narc)

enum narc_error narc_load(const char *file_path, struct narc **out_narc, struct vfs_ctx *out_vfs_ctx)
{
    enum narc_error err;
    struct narc header;
    size_t fsize;

    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        return NARCERR_ERRNO;
    }

    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(&header, sizeof(char), HEADER_SIZE, f);

    if ((err = narc_check_header_fsize(&header, fsize)) != NARCERR_NONE) {
        fclose(f);
        return err;
    }

    if ((*out_narc = malloc(fsize)) == NULL) {
        fclose(f);
        return NARCERR_ERRNO;
    }

    fread((*out_narc)->vfs, sizeof(char), fsize - sizeof(header), f);
    fclose(f);

    if ((err = narc_check_vfs(*out_narc, out_vfs_ctx)) != NARCERR_NONE) {
        free(*out_narc);
        *out_narc = NULL;
        return err;
    }

    memcpy(*out_narc, &header, HEADER_SIZE);
    return NARCERR_NONE;
}
