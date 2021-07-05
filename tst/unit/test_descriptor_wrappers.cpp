
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"
#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:descriptor_wrapper_constructor1","[unit,panacea]"){

  std::vector<std::vector<double>> data;
  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,0,0);

}

TEST_CASE("Testing:descriptor_wrapper_constructor2","[unit,panacea]"){

  std::vector<std::vector<double>> data = test::generateVecData();

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2, 3);
  auto data_type_index = type_index(typeid(vector<vector<double>> *));
  REQUIRE(data_type_index == dwrapper.getTypeIndex());
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);
}

TEST_CASE("Testing:descriptor_wrapper_access","[unit,panacea]"){

  std::vector<std::vector<double>> data = test::generateVecData();

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2,3);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(1,0) == 1.0);

  REQUIRE(dwrapper(0,1) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(0,2) == 3.0);
  REQUIRE(dwrapper(1,2) == 3.0);
}

TEST_CASE("Testing:descriptor_arrangements","[unit,panacea]"){


  std::vector<std::vector<double>> data = test::generateVecData();

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2,3);

  // Flip how the rows and columns are being interpreted
  dwrapper.set(Arrangement::DimensionsAlongRowsPointsAlongCols);

  // Rows and columns don't change
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(dwrapper.getNumberPoints() == 3);
  REQUIRE(dwrapper.getNumberDimensions() == 2);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(0,1) == 1.0);

  REQUIRE(dwrapper(1,0) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(2,0) == 3.0);
  REQUIRE(dwrapper(2,1) == 3.0);
}

TEST_CASE("Testing:descriptor_different_template_type","[unit,panacea]"){

  test::ArrayData array_data;

  DescriptorWrapper<double ***> dwrapper(&array_data.data,2,3);

  // Rows and columns don't change
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(1,0) == 1.0);

  REQUIRE(dwrapper(0,1) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(0,2) == 3.0);
  REQUIRE(dwrapper(1,2) == 3.0);

}

TEST_CASE("Testing:descriptor_wrapper write and read","[unit,panacea]"){

  test::ArrayData array_data;

  DescriptorWrapper<double ***> dwrapper(&array_data.data,2,3);

  // Rows and columns don't change
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(1,0) == 1.0);

  REQUIRE(dwrapper(0,1) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(0,2) == 3.0);
  REQUIRE(dwrapper(1,2) == 3.0);

  std::fstream fs;
  fs.open("descriptors.txt", std::fstream::out );

  BaseDescriptorWrapper * dwrapper_ptr = &dwrapper;
  BaseDescriptorWrapper::write(
      settings::FileType::TXTDescriptors, fs, dwrapper_ptr);

  fs.close();

  std::fstream fs_in;
  fs_in.open("descriptors.txt", std::fstream::in);

  std::vector<std::vector<double>> empty_data;
  DescriptorWrapper<std::vector<std::vector<double>>> dwrapper2(empty_data, 0, 0);
  BaseDescriptorWrapper * dwrapper_ptr2 = &dwrapper2;

  BaseDescriptorWrapper::read(
      settings::FileType::TXTDescriptors, fs_in, dwrapper_ptr2);

  fs_in.close();

  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);

  REQUIRE(dwrapper2(0,0) == 1.0);
  REQUIRE(dwrapper2(1,0) == 1.0);

  REQUIRE(dwrapper2(0,1) == 2.0);
  REQUIRE(dwrapper2(1,1) == 2.0);

  REQUIRE(dwrapper2(0,2) == 3.0);
  REQUIRE(dwrapper2(1,2) == 3.0);
}
