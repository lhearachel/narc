# Copyright 2024 <lhearachel@proton.me>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

lib_sources = files(
    # check API
    'src/check/narc_check.c',
    'src/check/narc_check_fatb.c',
    'src/check/narc_check_fimg.c',
    'src/check/narc_check_fntb.c',
    'src/check/narc_check_header.c',
    'src/check/narc_check_header_fsize.c',
    'src/check/narc_check_vfs.c',

    # dump API
    'src/dump/narc_dump.c',

    # error API
    'src/error/narc_strerror.c',

    # files API
    'src/files/narc_files_getext.c',
    'src/files/narc_files_getimg.c',

    # load API
    'src/load/narc_load.c',

    # pack API
    'src/pack/narc_pack.c',
    'src/pack/narc_pack_file.c',
    'src/pack/narc_pack_halt.c',
    'src/pack/narc_pack_start.c',
)

lib_includes = include_directories('include')
