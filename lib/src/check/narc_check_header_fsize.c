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

enum narc_error narc_check_header_fsize(const struct narc *narc, const size_t fsize)
{
    enum narc_error err = narc_check_header(narc);
    return err != NARCERR_NONE
        ? err
        : narc->size != fsize ? NARCERR_FILE_SIZE
                              : NARCERR_NONE;
}
