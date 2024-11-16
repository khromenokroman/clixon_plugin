cmake_minimum_required(VERSION 3.16)

project(plug-system)

set(CMAKE_CXX_STANDARD 20)

message(STATUS "CXX Compiler : ${CMAKE_CXX_COMPILER}")
message(STATUS "CXX Compiler path : ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CXX Compiler version : ${CMAKE_CXX_COMPILER_VERSION}")

function(parse_version)
    file(READ Version versionRaw)
    message(STATUS "Полученная версия из файла: ${versionRaw}")
    string(STRIP "${versionRaw}" version)
    string(REPLACE "." " " version ${version})    # Заменяем точки на пробел

    separate_arguments(versionSeparatedArgs NATIVE_COMMAND "${version}")

    list(GET versionSeparatedArgs 0 versionMajor)
    list(GET versionSeparatedArgs 1 versionMinor)
    list(GET versionSeparatedArgs 2 versionPatch)
    list(GET versionSeparatedArgs 3 versionTweak)

    message(STATUS "Мажорная версия: ${versionMajor}")
    message(STATUS "Минорная версия: ${versionMinor}")
    message(STATUS "Патч версия: ${versionPatch}")
    message(STATUS "Твик версия: ${versionTweak}")

    set(VERSION_MAJOR ${versionMajor} CACHE STRING "Major version number" FORCE)
    set(VERSION_MINOR ${versionMinor} CACHE STRING "Minor version number" FORCE)
    set(VERSION_PATCH ${versionPatch} CACHE STRING "Patch version number" FORCE)
    set(VERSION_TWEAK ${versionTweak} CACHE STRING "Tweak version number" FORCE)
endfunction()

parse_version()

add_definitions("-DVERSION_MAJOR=${VERSION_MAJOR}")
add_definitions("-DVERSION_MINOR=${VERSION_MINOR}")
add_definitions("-DVERSION_PATCH=${VERSION_PATCH}")
add_definitions("-DVERSION_TWEAK=${VERSION_TWEAK}")

set(CMAKE_VERBOSE_MAKEFILE ON)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wno-format-truncation
        -Wnon-virtual-dtor
        -Wno-stringop-overflow
        -Wno-stringop-truncation
)

add_link_options(
        -Wno-undefined
)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER "8.0.0" OR CMAKE_C_COMPILER_VERSION VERSION_GREATER "8.0.0")
    add_compile_options(-Wno-stringop-truncation)
endif ()

add_custom_target(version
        -DVERSION_MAJOR="${VERSION_MAJOR}"
        -DVERSION_MINOR="${VERSION_MINOR}"
        -DVERSION_PATCH="${VERSION_PATCH}"
        -DVERSION_TWEAK="${VERSION_TWEAK}"
        )

add_subdirectory(src)