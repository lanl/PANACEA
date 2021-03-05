
// Local private includes
#include "kernels/kernel_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:kernel_wrapper_constructor1","[unit,panacea]"){

  std::vector<std::vector<double>> data;
  KernelWrapper<std::vector<std::vector<double>>*> kwrapper(&data,0,0);

}

TEST_CASE("Testing:kernel_wrapper_constructor2","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  KernelWrapper<std::vector<std::vector<double>>*> kwrapper(&data,2, 3);
  REQUIRE(kwrapper.rows() == 2);
  REQUIRE(kwrapper.cols() == 3);

  REQUIRE(kwrapper.getNumberDimensions() == 3);
  REQUIRE(kwrapper.getNumberPoints() == 2);
}

TEST_CASE("Testing:kernel_wrapper_access","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  KernelWrapper<std::vector<std::vector<double>>*> kwrapper(&data,2,3);

  REQUIRE(kwrapper(0,0) == 1.0);
  REQUIRE(kwrapper(1,0) == 1.0);

  REQUIRE(kwrapper(0,1) == 2.0);
  REQUIRE(kwrapper(1,1) == 2.0);

  REQUIRE(kwrapper(0,2) == 3.0);
  REQUIRE(kwrapper(1,2) == 3.0);
}

TEST_CASE("Testing:kernel_arrangements","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  KernelWrapper<std::vector<std::vector<double>>*> kwrapper(&data,2,3);

  // Flip how the rows and columns are being interpreted
  kwrapper.set(Arrangement::DimensionsAlongRowsPointsAlongCols);

  // Rows and columns don't change
  REQUIRE(kwrapper.rows() == 2);
  REQUIRE(kwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(kwrapper.getNumberDimensions() == 2);
  REQUIRE(kwrapper.getNumberPoints() == 3);

  REQUIRE(kwrapper(0,0) == 1.0);
  REQUIRE(kwrapper(0,1) == 1.0);

  REQUIRE(kwrapper(1,0) == 2.0);
  REQUIRE(kwrapper(1,1) == 2.0);

  REQUIRE(kwrapper(2,0) == 3.0);
  REQUIRE(kwrapper(2,1) == 3.0);
}


