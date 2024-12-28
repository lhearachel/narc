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

const char *narc_strerror(enum narc_error error)
{
    switch (error) {
    default:
    case NARCERR_NONE:
        return "(null)";
    case NARCERR_ERRNO:
        return "Standard error; refer to errno for details";
    case NARCERR_MAGIC:
        return "Invalid magic marker in NARC header";
    case NARCERR_BOM:
        return "Invalid byte-order marker in NARC header";
    case NARCERR_VERSION:
        return "Invalid version marker in NARC header";
    case NARCERR_FILE_SIZE:
        return "Invalid file size as reported in NARC header";
    case NARCERR_HEADER_SIZE:
        return "Invalid NARC header size";
    case NARCERR_NUM_SECTIONS:
        return "Invalid section count in NARC header";
    case NARCERR_FATB_MAGIC:
        return "Invalid magic marker in FATB section";
    case NARCERR_FATB_SIZE:
        return "FATB section size is not proportional to the number of files";
    case NARCERR_FATB_RESERVED:
        return "Reserved FATB field contains unexpected data";
    case NARCERR_FNTB_MAGIC:
        return "Invalid magic marker in FNTB section";
    case NARCERR_FIMG_MAGIC:
        return "Invalid magic marker in FIMG section";
    }
}
