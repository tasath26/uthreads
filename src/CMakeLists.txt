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
