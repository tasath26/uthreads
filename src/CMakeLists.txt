add_library(uthreads STATIC
    impl/threads.c
    impl/sem.c
    impl/chan.c
    common/structures/queue.c
    common/structures/table.c
    sig/handler.c
)

target_include_directories(uthreads
    PUBLIC
        ${PROJECT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ARCH "x86_64" CACHE STRING "Target Architecture")

set(SUPPORTED_ARCHS
    x86_64
    x86_i386
    arm64
    arm32
    riscv
    mips
    sparc 
    powerpc
)

if(NOT ARCH IN_LIST SUPPORTED_ARCHS)
    message(FATAL_ERROR "Unsupported architecture: ${ARCH}")
endif()

string(TOUPPER "${ARCH}" ARCH_DEFINE)

target_compile_definitions(uthreads PRIVATE ${ARCH_DEFINE})

target_sources(uthreads PRIVATE
    arch/${ARCH}/context.c
    arch/${ARCH}/switch.s
    arch/${ARCH}/start.s
)

target_sources(uthreads PRIVATE
    wrap/alloc.c
    wrap/stdio.c
    wrap/io.c
)

