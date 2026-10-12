include(CTest)

file(GLOB TEST_SOURCES "*.c")

foreach(source IN LISTS TEST_SOURCES)
    get_filename_component(test_name ${source} NAME_WE)

    add_executable(${test_name} ${source})
    target_link_libraries(${test_name} PRIVATE uthreads)

    add_test(NAME ${test_name} COMMAND ${test_name})

    set_tests_properties(${test_name} PROPERTIES
        TIMEOUT 30
    )
endforeach()
