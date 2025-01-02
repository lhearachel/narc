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
    meson_version: '>=1.3.0',

    # This is a little ugly, but it works... I guess...
    version: run_command('./tools/version.sh', 'VERSION', check: false).stdout().strip(),
    default_options: {
        'buildtype': 'release', # O3
        'warning_level': '3', # all, extra, pedantic
        'c_std': 'c99',
    },
)

native = get_option('native')
install = native and meson.is_cross_build() ? false : true
version = files('VERSION')

subdir('tools')
subdir('lib')
subdir('cli')

libnarc = shared_library(
    'narc',
    lib_sources,
    include_directories: lib_includes,
    install: install,
    install_mode: 'rwxr-xr-x',
    native: native,
    version: meson.project_version(),
)

libnarc_dep = declare_dependency(
    include_directories: lib_includes,
    link_with: libnarc,
)

narc_exe = executable(
    'narc',
    sources: [
        cli_version_h,
        cli_sources,
        lib_sources,
    ],
    include_directories: [
        cli_includes,
        lib_includes,
    ],
    install: install,
    install_mode: 'rwxr-xr-x',
    native: native,
)

meson.override_find_program('narc', narc_exe)
meson.override_dependency('libnarc', libnarc_dep)
