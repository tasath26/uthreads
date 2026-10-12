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


set(UTHREADS_WRAPPED_FUNCTIONS
    malloc calloc realloc free
    printf fprintf vprintf vfprintf
    puts fputs putchar fputc
    fread fwrite fflush fopen fclose
    read write 
)

foreach(func IN LISTS UTHREADS_WRAPPED_FUNCTIONS)
    target_link_options(uthreads INTERFACE
        "-Wl,--wrap=${func}"
    )
endforeach()

include(GNUInstallDirs)

set(UTHREADS_WRAP_FLAGS "")
foreach(func IN LISTS UTHREADS_WRAPPED_FUNCTIONS)
    string(APPEND UTHREADS_WRAP_FLAGS " -Wl,--wrap=${func}")
endforeach()

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/uthreads.pc.in"
    "${CMAKE_CURRENT_BINARY_DIR}/uthreads.pc"
    @ONLY
)

install(TARGETS uthreads
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
)

install(DIRECTORY api/
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/uthreads"
    FILES_MATCHING PATTERN "*.h"
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/uthreads.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)
