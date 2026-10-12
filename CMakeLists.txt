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

set(UTHREAD_STACK_SIZE 64 CACHE STRING "Thread stack size in KB")
set(UTHREAD_PRMT_INTERVAL_US 10000 CACHE STRING "Preemption interval in microseconds")

# Validate stack size
if(NOT UTHREAD_STACK_SIZE MATCHES "^[0-9]+$")
    message(FATAL_ERROR "UTHREAD_STACK_SIZE must be a positive integer")
endif()

if(UTHREAD_STACK_SIZE LESS 1 OR UTHREAD_STACK_SIZE GREATER 8192)
    message(FATAL_ERROR
        "UTHREAD_STACK_SIZE must be between 1 and 8192 KB"
    )
endif()

# Validate preemption interval
if(NOT UTHREAD_PRMT_INTERVAL_US MATCHES "^[0-9]+$")
    message(FATAL_ERROR "UTHREAD_PRMT_INTERVAL_US must be a positive integer")
endif()

if(UTHREAD_PRMT_INTERVAL_US LESS 1)
    message(FATAL_ERROR "UTHREAD_PRMT_INTERVAL_US must be greater than 0")
endif()

# Pass definitions to the compiler
target_compile_definitions(uthreads PRIVATE
    UTHREAD_STACK_SIZE=${UTHREAD_STACK_SIZE}
    UTHREAD_PRMT_INTERVAL_US=${UTHREAD_PRMT_INTERVAL_US}
)


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
