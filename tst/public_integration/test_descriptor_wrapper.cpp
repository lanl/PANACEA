
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/entropy_term.hpp"
#include "panacea/panacea.hpp"

// Local private includes
#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;
using namespace panacea::settings;

TEST_CASE("Testing:panacea-descriptor_wrapper","[end-to-end,panacea]"){

  // pi - public interface
  PANACEA panacea_pi;

  WHEN("Wrapping double ***"){
    // Data has the following form
    //
    //         col1   col2   col3
    // row1    1.0     2.0    3.0
    // row2    1.0     2.0    3.0
    test::ArrayData array_data;
    const int rows = 2;
    const int cols = 3;
    auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);
    REQUIRE(dwrapper->operator()(0,0) == 1.0);
    REQUIRE(dwrapper->operator()(1,0) == 1.0);
    REQUIRE(dwrapper->operator()(0,1) == 2.0);
    REQUIRE(dwrapper->operator()(1,1) == 2.0);
    REQUIRE(dwrapper->operator()(0,2) == 3.0);
    REQUIRE(dwrapper->operator()(1,2) == 3.0);

    // If we change the original data the data in the wrapper should
    // also change
    array_data.data[0][0] = 12.0;
    REQUIRE(dwrapper->operator()(0,0) == 12.0);
  }

  WHEN("Wrapping vector<vector<double>>*"){
    // Data has the following form
    //
    //         col1   col2   col3
    // row1    1.0     2.0    3.0
    // row2    1.0     2.0    3.0
    std::vector<std::vector<double>> vec_data = test::generateVecData();;
    const int rows = 2;
    const int cols = 3;
    auto dwrapper = panacea_pi.wrap(&vec_data, rows, cols);
    REQUIRE(dwrapper->operator()(0,0) == 1.0);
    REQUIRE(dwrapper->operator()(1,0) == 1.0);
    REQUIRE(dwrapper->operator()(0,1) == 2.0);
    REQUIRE(dwrapper->operator()(1,1) == 2.0);
    REQUIRE(dwrapper->operator()(0,2) == 3.0);
    REQUIRE(dwrapper->operator()(1,2) == 3.0);

    vec_data.at(0).at(0) = 12.0;
    REQUIRE(dwrapper->operator()(0,0) == 12.0);
  }
}


