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

project(
    'narc',
    'c',
    license: 'Apache-2.0',
    license_files: 'LICENSE',
    meson_version: '>=1.5.0',
    version: '0.1.0',
    default_options: {
        'buildtype': 'release', # O3
        'warning_level': '3', # all, extra, pedantic
        'c_std': 'c99',
    },
)

native = get_option('native')
install = native and meson.is_cross_build() ? false : true

subdir('lib')
subdir('cli')

libnarc = shared_library(
    'narc',
    lib_sources,
    include_directories: lib_includes,
    install: true,
    install_mode: 'rwxr-xr-x',
    version: meson.project_version(),
)

libnarc_dep = declare_dependency(
    include_directories: lib_includes,
    link_with: libnarc,
)

narc_exe = executable(
    'narc',
    sources: [
        cli_sources,
        lib_sources,
    ],
    include_directories: [
        cli_includes,
        lib_includes,
    ],
    install: true,
    install_mode: 'rwxr-xr-x',
)

meson.override_find_program('narc', narc_exe)
