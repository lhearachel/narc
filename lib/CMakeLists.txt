cmake_minimum_required(VERSION 3.15)

set(LIB_SOURCES
    # check API
    "src/check/narc_check.c"
    "src/check/narc_check_fatb.c"
    "src/check/narc_check_fimg.c"
    "src/check/narc_check_fntb.c"
    "src/check/narc_check_header.c"
    "src/check/narc_check_header_fsize.c"
    "src/check/narc_check_vfs.c"

    # dump API
    "src/dump/narc_dump.c"

    # error API
    "src/error/narc_strerror.c"

    # files API
    "src/files/narc_files_getext.c"
    "src/files/narc_files_getimg.c"

    # load API
    "src/load/narc_load.c"

    # pack API
    "src/pack/narc_pack.c"
    "src/pack/narc_pack_file.c"
    "src/pack/narc_pack_file_copy.c"
    "src/pack/narc_pack_halt.c"
    "src/pack/narc_pack_start.c")

add_library(libnarc SHARED ${LIB_SOURCES})
add_library(libnarc_static STATIC ${LIB_SOURCES})
target_include_directories(libnarc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(libnarc_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if (WIN32)
    target_compile_definitions(libnarc PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_definitions(libnarc_static PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
