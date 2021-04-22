
list( APPEND TEST_SOURCES
    catch_main.cpp
    test_covariance.cpp
    test_descriptor_wrappers.cpp
    test_distributions.cpp
    test_gaussian_uncorrelated.cpp
    test_inverter.cpp
    test_kernel_wrappers.cpp
    test_mean.cpp
    test_memory_manager.cpp
    test_normalization_methods.cpp
    test_normalizer.cpp
    test_reducer.cpp
    test_row_echelon.cpp
    test_self_entropy.cpp
    test_variance.cpp
    )

include_directories(../helper)
add_executable(unit_tests ${TEST_SOURCES})
add_dependencies(unit_tests libhelper libpanacea)
target_link_libraries(unit_tests PUBLIC libpanacea libhelper Catch2::Catch2)
ParseAndAddCatchTests(unit_tests)

get_property(ALL_TESTS DIRECTORY . PROPERTY TESTS)
 
