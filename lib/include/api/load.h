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
#ifndef NARC_API_LOAD_H
#define NARC_API_LOAD_H

#include <defs/error.h>
#include <defs/narc.h>
#include <defs/vfs.h>

/*
 * Load a NARC from an existing file at the given path. If the file contents
 * validate as a NARC, then `out_narc` will be set to an address with sufficient
 * memory allocation to hold the entire file. The calling client is responsible
 * for freeing this allocation. Additionally, `out_vfs_ctx` will be populated
 * as in `narc_check_vfs`.
 *
 * This routine may emit any error which would also be emitted by `narc_check`.
 */
enum narc_error narc_load(const char *file_path, struct narc **out_narc, struct vfs_ctx *out_vfs_ctx);

#endif // NARC_API_LOAD_H
