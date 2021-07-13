
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"

#include "io/file_io_factory.hpp"
#include "helper.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:descriptor_wrapper file io with write and read","[integration,panacea]"){

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

  BaseDescriptorWrapper * dwrapper_ptr = &dwrapper;

  panacea::io::FileIOFactory file_factory;
  std::unique_ptr<panacea::io::FileIO> file_io = file_factory.create(settings::FileType::TXTDescriptors);
  std::fstream fs;
  fs.open("wrapper.descriptors", std::fstream::out );
  file_io->write(dwrapper_ptr, fs);
  fs.close();


  std::vector<std::vector<double>> empty_data;
  DescriptorWrapper<std::vector<std::vector<double>>> dwrapper2(empty_data, 0, 0);
  BaseDescriptorWrapper * dwrapper_ptr2 = &dwrapper2;

  std::fstream fs_in;
  fs_in.open("wrapper.descriptors", std::fstream::in);
  file_io->read(dwrapper_ptr2, fs_in);
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
