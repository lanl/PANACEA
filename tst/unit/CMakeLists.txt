
list( APPEND TEST_SOURCES
    catch_main.cpp
    test_descriptor_wrappers.cpp
    test_kernel_wrappers.cpp
    test_memory_manager.cpp
    )

add_executable(unit_tests ${TEST_SOURCES})
target_link_libraries(unit_tests PUBLIC libpanacea Catch2::Catch2)
ParseAndAddCatchTests(unit_tests)

get_property(ALL_TESTS DIRECTORY . PROPERTY TESTS)
 
