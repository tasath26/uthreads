add_library(uthreads
    impl/threads.c
    impl/sem.c
    impl/chan.c
)

target_include_directories(uthreads
    PUBLIC
        ${PROJECT_SOURCE_DIR}/api
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(ARCH "x86_64" CACHE STRING "Target Architecture")

if(ARCH STREQUAL "x86_64")
  target_compile_definitions(uthreads PRIVATE X86_64)

  target_sources(uthreads PRIVATE 
      arch/x86_64/context.c
      arch/x86_64/switch.s
      arch/x86_64/start.s
  )
else()
  message(FATAL_ERROR "Unsupported Architecture: " ${ARCH})
endif()
