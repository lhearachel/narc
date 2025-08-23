cmake_minimum_required(VERSION 3.15)

project(narc LANGUAGES C)
set(CMAKE_C_STANDARD 99)

if (WIN32)
    include(FetchContent)
    FetchContent_Declare(
        dirent
        URL https://github.com/tronkko/dirent/archive/refs/tags/1.26.zip
        DOWNLOAD_EXTRACT_TIMESTAMP ON
    )
    FetchContent_MakeAvailable(dirent)

    add_compile_options(/wd4267 /wd4244)
endif()

add_subdirectory(lib)
add_subdirectory(cli)
