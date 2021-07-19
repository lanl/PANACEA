
// Local private includes
#include "data_point_template.hpp"
#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <fstream>
#include <iostream>
#include <typeindex>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:data_point_template constructors", "[unit,panacea]") {
  WHEN("Using default memory layout") {
    DataPointTemplate<std::vector<std::vector<double>> *> data1;
    REQUIRE(data1.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data1.layout() == MemoryLayout::Default);
    DataPointTemplate<std::vector<std::vector<double>>> data2;
    REQUIRE(data2.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data2.layout() == MemoryLayout::Default);
    DataPointTemplate<std::vector<double> *> data3;
    REQUIRE(data3.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data3.layout() == MemoryLayout::Default);
    DataPointTemplate<std::vector<double>> data4;
    REQUIRE(data4.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data4.layout() == MemoryLayout::Default);
    DataPointTemplate<double ***> data5;
    REQUIRE(data5.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data5.layout() == MemoryLayout::Default);
  }

  WHEN("Using ColumnMajor memory layout") {
    DataPointTemplate<std::vector<double>, MemoryLayout::ColumnMajor> data1;
    REQUIRE(data1.arrangement() ==
            Arrangement::PointsAlongRowsDimensionsAlongCols);
    REQUIRE(data1.layout() == MemoryLayout::ColumnMajor);
  }
}

TEST_CASE("Testing:data template wrapper test with nested vector",
          "[unit,panacea]") {

  std::vector<std::vector<double>> data = test::generateVecData();

  DataPointTemplate<std::vector<std::vector<double>> *> data_template(&data, 2,
                                                                      3);
  auto data_type_index = type_index(typeid(vector<vector<double>> *));
  REQUIRE(data_type_index ==
          std::type_index(data_template.getPointerToRawData().type()));
  REQUIRE(data_template.rows() == 2);
  REQUIRE(data_template.cols() == 3);

  REQUIRE(data_template.getNumberPoints() == 2);
  REQUIRE(data_template.getNumberDimensions() == 3);

  DataPointTemplate<std::vector<std::vector<double>>> data_template2(data, 2,
                                                                     3);
  auto data_type_index2 =
      type_index(typeid(const std::vector<std::vector<double>> *));
  REQUIRE(data_type_index2 ==
          std::type_index(data_template2.getPointerToRawData().type()));
  REQUIRE(data_template2.rows() == 2);
  REQUIRE(data_template2.cols() == 3);

  REQUIRE(data_template2.getNumberPoints() == 2);
  REQUIRE(data_template2.getNumberDimensions() == 3);
}

TEST_CASE("Testing:data template wrapper test with nested double ***",
          "[unit,panacea]") {

  test::ArrayData2 arr;
  // 1.0 4.0
  // 2.0 5.0
  // 3.0 6.0

  DataPointTemplate<double ***> data_template(&arr.data, 3, 2);
  auto data_type_index = type_index(typeid(double ***));
  REQUIRE(data_type_index ==
          std::type_index(data_template.getPointerToRawData().type()));
  REQUIRE(data_template.rows() == 3);
  REQUIRE(data_template.cols() == 2);

  REQUIRE(data_template.getNumberPoints() == 3);
  REQUIRE(data_template.getNumberDimensions() == 2);

  REQUIRE(data_template.at(0, 0) == 1.0);
  REQUIRE(data_template.at(1, 0) == 2.0);
  REQUIRE(data_template.at(2, 0) == 3.0);
  REQUIRE(data_template.at(0, 1) == 4.0);
  REQUIRE(data_template.at(1, 1) == 5.0);
  REQUIRE(data_template.at(2, 1) == 6.0);
}
