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
#ifndef NARC_H
#define NARC_H

#include <stdint.h>

enum narc_error {
    NARCERR_NONE = 0,

    NARCERR_MAGIC,
    NARCERR_BOM,
    NARCERR_VERSION,
    NARCERR_HEADER_SIZE,
    NARCERR_NUM_SECTIONS,

    NARCERR_ERRNO = 0xFF,
};

struct narc {
    uint32_t magic;
    uint16_t bom;
    uint16_t version;
    uint32_t size;
    uint16_t header_size;
    uint16_t num_sections;
    unsigned char data[];
};

/*
 * Convert a `narc_error` value into a corresponding human-readable message.
 */
const char *narc_strerror(enum narc_error error);

/*
 * Load a NARC from an existing file at the given path. If the file contents
 * validate as a NARC, then `out_narc` will be set to an address with sufficient
 * memory allocation to hold the entire file. The calling client is responsible
 * for freeing this allocation.
 *
 * Any of the following error codes may be emitted:
 *
 *  - `NARCERR_NONE`         - No error; `narc` contains good data.
 *  - `NARCERR_MAGIC`        - The magic ID in the file's header did not match
 *                             expectations.
 *  - `NARCERR_BOM`          - The byte-order marker in the file's header did
 *                             not match expectations.
 *  - `NARCERR_VERSION`      - The version marker in the file's header did not
 *                             match expectations.
 *  - `NARCERR_HEADER_SIZE`  - The file's header reports an invalid size for
 *                             itself.
 *  - `NARCERR_NUM_SECTIONS` - The number of sections expected by the file's
 *                             header did not match expectations.
 *  - `NARCERR_ERRNO`        - A system-level error occurred; consult `errno`.
 */
enum narc_error narc_load(const char *file_path, struct narc **out_narc);

#endif // NARC_H
