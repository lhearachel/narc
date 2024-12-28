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

enum narc_error narc_check(const struct narc *narc, struct vfs_ctx *out_vfs_ctx)
{
    enum narc_error ret;
    ERROR_RET(NARCERR_NONE, narc_check_header(narc));
    ERROR_RET(NARCERR_NONE, narc_check_vfs(narc, out_vfs_ctx));

    return NARCERR_NONE;
}
