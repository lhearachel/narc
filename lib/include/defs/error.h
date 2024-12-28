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
#ifndef NARC_DEFS_ERROR_H
#define NARC_DEFS_ERROR_H

enum narc_error {
    NARCERR_NONE = 0,

    NARCERR_MAGIC,
    NARCERR_BOM,
    NARCERR_VERSION,
    NARCERR_FILE_SIZE,
    NARCERR_HEADER_SIZE,
    NARCERR_NUM_SECTIONS,

    NARCERR_FATB_MAGIC,
    NARCERR_FATB_SIZE,
    NARCERR_FATB_RESERVED,

    NARCERR_FNTB_MAGIC,

    NARCERR_FIMG_MAGIC,

    NARCERR_DUMP_TARGET_IS_FILE,

    NARCERR_ERRNO = 0xFF,
};

#endif // NARC_DEFS_ERROR_H
