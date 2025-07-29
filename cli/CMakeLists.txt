cmake_minimum_required(VERSION 3.15)

# Read version from .version file
file(READ "${CMAKE_SOURCE_DIR}/.version" NARC_VERSION)
string(STRIP "${NARC_VERSION}" NARC_VERSION)

# Generate version.h header
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/include/version.h"
    @ONLY
)

set(CLI_SOURCES
    "src/narc.c"
    "src/narc_create.c"
    "src/narc_extract.c"
    "src/narc_help.c"
    "src/narc_info.c"
    "src/narc_yank.c"
    "src/strbuild.c"
    "src/strutil.c"
    "src/strvec.c")

# Add Windows-specific fnmatch implementation
if (WIN32)
    list(APPEND CLI_SOURCES "src/fnmatch_win.c")
endif()

add_executable(narc ${CLI_SOURCES})
target_include_directories(narc PRIVATE
    include
    ${CMAKE_CURRENT_BINARY_DIR}/include
)
target_link_libraries(narc PRIVATE libnarc_static)

# Link Windows Shell API for PathMatchSpecA
if (WIN32)
    target_link_libraries(narc PRIVATE shlwapi)
    target_link_libraries(narc PRIVATE dirent)
    target_compile_definitions(narc PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
