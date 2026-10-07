add_executable(test_hello hello.c)
add_executable(stress_test stress_test.c)

target_link_libraries(test_hello PRIVATE uthreads)
target_link_libraries(stress_test PRIVATE uthreads)

add_test(NAME hello COMMAND test_hello)
add_test(NAME stress_test COMMAND stress_test)
