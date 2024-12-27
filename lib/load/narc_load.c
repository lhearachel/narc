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
#include <api/error.h>
#include <api/load.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NARC_MAGIC     0x4352414E
#define LE_BOM         0xFFFE
#define VERSION_MARKER 0x0100
#define HEADER_SIZE    16
#define NUM_SECTIONS   3

#define ERROR_NEQ(expect, actual, err, file) \
    {                                        \
        if ((expect) != (actual)) {          \
            fclose(file);                    \
            return err;                      \
        }                                    \
    }

enum narc_error narc_load(const char *file_path, struct narc **out_narc)
{
    FILE *f = fopen(file_path, "rb");
    if (f == NULL) {
        return NARCERR_ERRNO;
    }

    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Validate the header.
    struct narc header;
    fread(&header, sizeof(char), HEADER_SIZE, f);

    ERROR_NEQ(NARC_MAGIC, header.magic, NARCERR_MAGIC, f);
    ERROR_NEQ(LE_BOM, header.bom, NARCERR_BOM, f);
    ERROR_NEQ(VERSION_MARKER, header.version, NARCERR_VERSION, f);
    ERROR_NEQ(HEADER_SIZE, header.header_size, NARCERR_HEADER_SIZE, f);
    ERROR_NEQ(NUM_SECTIONS, header.num_sections, NARCERR_NUM_SECTIONS, f);

    // Header is good; allocate enough memory to hold the whole file.
    *out_narc = malloc(fsize);
    if (*out_narc == NULL) {
        fclose(f);
        return NARCERR_ERRNO;
    }

    memcpy(*out_narc, &header, sizeof(header));
    fread((*out_narc)->data, sizeof(char), fsize - sizeof(header), f);
    fclose(f);
    return NARCERR_NONE;
}
