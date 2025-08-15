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

#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// clang-format off
static const struct { const char *magic; const char *ext; size_t cmp_n; } extensions[] = {
    { "RGCN", "NCGR", 4 },
    { "RLCN", "NCLR", 4 },
    { "RECN", "NCER", 4 },
    { "RNAN", "NANR", 4 },
    { "RCSN", "NSCR", 4 },
    { "BMD0", "BMD0", 4 },
    { "BCA0", "BCA0", 4 },
    { "BTA0", "BTA0", 4 },
    { "BTP0", "BTP0", 4 },
    { "BTX0", "BTX0", 4 },
    { "BMA0", "BMA0", 4 },
    { "BVA0", "BVA0", 4 },
    { " APS", "spa", 4 },
    { 0 }, // must always be last!
};
// clang-format on

// This value must always be equivalent to the maximum value of `cmp_n` for
// any entry in `extensions`, plus 1 character for the `.` prefix.
#define MAX_BASE_EXT_LEN (1 + 4)

char *narc_files_getext(const char *data)
{
    char *ext = malloc(MAX_EXT_LEN + 1);
    strcpy(ext, ".bin");

    if (data[0] == 0x10 && data[4] == 0x00) {
        // This should be an LZ77-encoded file; the first block of such an
        // encoding is always stored in full, which means we should see the
        // extension starting at index 5.
        //
        // If the file looks like it's LZ77-encoded, but we don't recognize
        // the extension, then fall back to the default (".bin").
        char *encoded_ext = narc_files_getext(data + 5);

        // The value returned by the recursed call already has the `.` prefix.
        assert(strlen(encoded_ext) <= MAX_BASE_EXT_LEN);
        if (strcmp(encoded_ext, ".bin") != 0) {
            sprintf(ext, "%s.lz", encoded_ext);
        }

        free(encoded_ext);
        return ext;
    }

    for (size_t i = 0; extensions[i].magic != NULL; i++) {
        if (strncmp(data, extensions[i].magic, extensions[i].cmp_n) == 0) {
            sprintf(ext, ".%s", extensions[i].ext);
            break;
        }
    }

    return ext;
}
