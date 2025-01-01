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
#ifndef NARC_API_DUMP_H
#define NARC_API_DUMP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <defs/error.h>
#include <defs/narc.h>
#include <defs/vfs.h>

/*
 * Write the contents of a NARC's virtual filesystem (VFS) to the specified
 * directory-path on disk. Any existing file-name table within the VFS will be
 * ignored. Files written to disk will be assigned names derived from their
 * VFS ID.
 */
enum narc_error narc_dump(const struct narc *narc, const struct vfs_ctx *vfs_ctx, const char *dst_dir);

#ifdef __cplusplus
}
#endif

#endif // NARC_API_DUMP_H
