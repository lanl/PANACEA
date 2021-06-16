list( APPEND HELPER_SOURCES
  helper/helper.cpp
  )

add_library(helper ${HELPER_SOURCES})
set_target_properties(helper PROPERTIES LINKER_LANGUAGE CXX)

list( APPEND UNIT_TEST_SOURCES
    unit/test_covariance.cpp
    unit/test_cross_entropy.cpp
    unit/test_data_point_template.cpp
    unit/test_descriptor_wrappers.cpp
    unit/test_distributions.cpp
    unit/test_gaussian_correlated.cpp
    unit/test_gaussian_uncorrelated.cpp
    unit/test_inverter.cpp
    unit/test_kernel_wrappers.cpp
    unit/test_kernel_specifications.cpp
    unit/test_matrix.cpp
    unit/test_mean.cpp
    unit/test_median.cpp
    unit/test_normalization_methods.cpp
    unit/test_normalizer.cpp
    unit/test_primitive_factory.cpp
    unit/test_primitive_group.cpp
    unit/test_reducer.cpp
    unit/test_row_echelon.cpp
    unit/test_self_entropy.cpp
    unit/test_settings.cpp
    unit/test_variance.cpp
    )

  list( APPEND PUBLIC_INTEGRATION_TEST_SOURCES
    public_integration/test_panacea.cpp
    )

include_directories(helper)
add_executable(panacea_tests catch_main.cpp ${UNIT_TEST_SOURCES} ${PUBLIC_INTEGRATION_TEST_SOURCES})
add_dependencies(panacea_tests helper panacea)
target_link_libraries(panacea_tests PUBLIC panacea helper Catch2::Catch2)

ParseAndAddCatchTests(panacea_tests)

get_property(ALL_TESTS DIRECTORY . PROPERTY TESTS)

