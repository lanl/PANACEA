
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"
#include "io/file_io_factory.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "kernels/mean_kernel_wrapper.hpp"
#include "kernels/median_kernel_wrapper.hpp"

#include "helper.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:mean kernel_wrapper write & read using fileio","[integration,panacea]") {

  std::vector<std::vector<double>> data = {{1.0, 4.0},{2.0, 9.0},{6.0, 2.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,2);

  MeanKernelWrapper mean_kwrapper(test::Test::key(), dwrapper);
  BaseKernelWrapper * kwrapper_ptr = &mean_kwrapper;

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

  restart_file->write(kwrapper_ptr, "test_kernel_mean_full.restart");

  MeanKernelWrapper mean_kwrapper2(test::Test::key());
  kwrapper_ptr = &mean_kwrapper2;
  restart_file->read(kwrapper_ptr, "test_kernel_mean_full.restart");

  REQUIRE(mean_kwrapper2.rows() == 1);
  REQUIRE(mean_kwrapper2.cols() == 2);
  REQUIRE(mean_kwrapper2.getNumberDimensions() == 2);
  REQUIRE(mean_kwrapper2.getNumberPoints() == 3);
  REQUIRE(mean_kwrapper2.at(0,0) == Approx(3.0));
  REQUIRE(mean_kwrapper2.at(0,1) == Approx(5.0));
  auto type_ind_data = std::type_index(typeid(vector<double>));
  REQUIRE(mean_kwrapper2.getTypeIndex() == type_ind_data);
}
