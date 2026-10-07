cmake_minimum_required(VERSION 3.10)

project(uthreads LANGUAGES C ASM)

option(BUILD_TESTS "Build tests" OFF)
option(DEBUG "Enable debug mode" OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(DEBUG)
    add_compile_definitions(DEBUG=ON)
endif()

add_subdirectory(src)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
