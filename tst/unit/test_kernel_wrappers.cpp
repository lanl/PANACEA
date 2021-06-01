
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

TEST_CASE("Testing:kernel_wrapper creation","[unit,panacea]") {

  std::vector<std::vector<double>> data = {{1.0},{2.0},{6.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,1); 


  WHEN("Testing mean kernel wrapper"){
    MeanKernelWrapper mean_kwrapper(test::Test::key(), &dwrapper);
    REQUIRE(mean_kwrapper.rows() == 1);
    REQUIRE(mean_kwrapper.cols() == 1);
    REQUIRE(mean_kwrapper.getNumberDimensions() == 1);
    REQUIRE(mean_kwrapper.getNumberPoints() == 3);
    REQUIRE(mean_kwrapper.at(0,0) == Approx(3.0));
    auto type_ind_data = std::type_index(typeid(vector<double>));
    REQUIRE(mean_kwrapper.getTypeIndex() == type_ind_data);
  }

  WHEN("Testing median kernel wrapper"){
    MedianKernelWrapper median_kwrapper(test::Test::key(), &dwrapper);
    REQUIRE(median_kwrapper.getNumberDimensions() == 1);
    REQUIRE(median_kwrapper.getNumberPoints() == 3);
    REQUIRE(median_kwrapper.rows() == 1);
    REQUIRE(median_kwrapper.cols() == 1);
    REQUIRE(median_kwrapper.at(0,0) == Approx(2.0));
    auto type_ind_data = std::type_index(typeid(vector<double>));
    REQUIRE(median_kwrapper.getTypeIndex() == type_ind_data);
  }
  WHEN("Testing template kernel wrapper with vector<vector<double>> *"){
    auto type_ind_data = std::type_index(typeid(vector<vector<double>> *));
    KernelWrapper<vector<vector<double>>*> kwrapper(test::Test::key(), &dwrapper);
    REQUIRE(kwrapper.getNumberDimensions() == 1);
    REQUIRE(kwrapper.getNumberPoints() == 3);
    REQUIRE(kwrapper.rows() == 3);
    REQUIRE(kwrapper.cols() == 1);
     
    REQUIRE(kwrapper.at(0,0) == Approx(1.0));
    REQUIRE(kwrapper.at(1,0) == Approx(2.0));
    REQUIRE(kwrapper.at(2,0) == Approx(6.0));
    REQUIRE(kwrapper.getTypeIndex() == type_ind_data);
  }
  WHEN("Testing template kernel wrapper with vector<vector<double>>"){

    // Here dwrapper2 will own the data
    DescriptorWrapper<vector<vector<double>>> dwrapper2(data,3,1); 
    auto type_ind_data = std::type_index(typeid(vector<vector<double>>));
    KernelWrapper<vector<vector<double>>> kwrapper(test::Test::key(), &dwrapper2);

    REQUIRE(kwrapper.getNumberDimensions() == 1);
    REQUIRE(kwrapper.getNumberPoints() == 3);
    REQUIRE(kwrapper.rows() == 3);
    REQUIRE(kwrapper.cols() == 1);
     
    REQUIRE(kwrapper.at(0,0) == Approx(1.0));
    REQUIRE(kwrapper.at(1,0) == Approx(2.0));
    REQUIRE(kwrapper.at(2,0) == Approx(6.0));
    REQUIRE(kwrapper.getTypeIndex() == type_ind_data);
  }
  WHEN("Testing mean kernel wrapper when a raw vector is used"){
    std::vector<double> mean = {1.0,2.0,6.0};
    MeanKernelWrapper mean_kwrapper(test::Test::key(), mean);
    REQUIRE(mean_kwrapper.rows() == 1);
    REQUIRE(mean_kwrapper.cols() == 3);
    REQUIRE(mean_kwrapper.getNumberDimensions() == 3);
    REQUIRE(mean_kwrapper.getNumberPoints() == 1);
    REQUIRE(mean_kwrapper.at(0,0) == Approx(1.0));
    REQUIRE(mean_kwrapper.at(0,1) == Approx(2.0));
    REQUIRE(mean_kwrapper.at(0,2) == Approx(6.0));
    auto type_ind_data = std::type_index(typeid(vector<double>));
    REQUIRE(mean_kwrapper.getTypeIndex() == type_ind_data);
  }
  WHEN("Testing median kernel wrapper when a raw vector is used"){
    std::vector<double> median = {1.0,2.0,6.0};
    MeanKernelWrapper median_kwrapper(test::Test::key(), median);
    REQUIRE(median_kwrapper.rows() == 1);
    REQUIRE(median_kwrapper.cols() == 3);
    REQUIRE(median_kwrapper.getNumberDimensions() == 3);
    REQUIRE(median_kwrapper.getNumberPoints() == 1);
    REQUIRE(median_kwrapper.at(0,0) == Approx(1.0));
    REQUIRE(median_kwrapper.at(0,1) == Approx(2.0));
    REQUIRE(median_kwrapper.at(0,2) == Approx(6.0));
    auto type_ind_data = std::type_index(typeid(vector<double>));
    REQUIRE(median_kwrapper.getTypeIndex() == type_ind_data);
  }
}

TEST_CASE("Testing:mean kernel_wrapper write & read","[unit,panacea]") {

  std::vector<std::vector<double>> data = {{1.0, 4.0},{2.0, 9.0},{6.0, 2.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,2); 

  MeanKernelWrapper mean_kwrapper(test::Test::key(), &dwrapper);
  BaseKernelWrapper * kwrapper_ptr = &mean_kwrapper;
  
  std::fstream fs;
  fs.open("mean_kernel_wrapper_restart.txt", std::fstream::out );
  BaseKernelWrapper::write(settings::FileType::TXTRestart, fs, kwrapper_ptr);
  fs.close();

  MeanKernelWrapper mean_kwrapper2(test::Test::key());
  kwrapper_ptr = &mean_kwrapper2;
  std::fstream fs2;
  fs2.open("mean_kernel_wrapper_restart.txt", std::fstream::in );
  BaseKernelWrapper::read(settings::FileType::TXTRestart, fs2, kwrapper_ptr);
  fs2.close();
  REQUIRE(mean_kwrapper2.rows() == 1);
  REQUIRE(mean_kwrapper2.cols() == 2);
  REQUIRE(mean_kwrapper2.getNumberDimensions() == 2);
  REQUIRE(mean_kwrapper2.getNumberPoints() == 3);
  REQUIRE(mean_kwrapper2.at(0,0) == Approx(3.0));
  REQUIRE(mean_kwrapper2.at(0,1) == Approx(5.0));
  auto type_ind_data = std::type_index(typeid(vector<double>));
  REQUIRE(mean_kwrapper2.getTypeIndex() == type_ind_data);
}

TEST_CASE("Testing:mean kernel_wrapper write & read using fileio","[integration,panacea]") {

  std::vector<std::vector<double>> data = {{1.0, 4.0},{2.0, 9.0},{6.0, 2.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,2); 

  MeanKernelWrapper mean_kwrapper(test::Test::key(), &dwrapper);
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

TEST_CASE("Testing:median kernel_wrapper write & read","[unit,panacea]") {

  std::vector<std::vector<double>> data = {{1.0, 4.0},{2.0, 9.0},{6.0, 2.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,2); 

  MedianKernelWrapper median_kwrapper(test::Test::key(), &dwrapper);
  BaseKernelWrapper * kwrapper_ptr = &median_kwrapper;

  std::fstream fs;
  fs.open("median_kernel_wrapper_restart.txt", std::fstream::out );
  BaseKernelWrapper::write(settings::FileType::TXTRestart, fs, kwrapper_ptr);
  fs.close();

  MedianKernelWrapper median_kwrapper2(test::Test::key());
  kwrapper_ptr = &median_kwrapper2;
  std::fstream fs2;
  fs2.open("median_kernel_wrapper_restart.txt", std::fstream::in );
  BaseKernelWrapper::read(settings::FileType::TXTRestart, fs2, kwrapper_ptr);
  fs2.close();

  REQUIRE(median_kwrapper2.getNumberDimensions() == 2);
  REQUIRE(median_kwrapper2.getNumberPoints() == 3);
  REQUIRE(median_kwrapper2.rows() == 1);
  REQUIRE(median_kwrapper2.cols() == 2);
  REQUIRE(median_kwrapper2.at(0,0) == Approx(2.0));
  REQUIRE(median_kwrapper2.at(0,1) == Approx(4.0));
  auto type_ind_data = std::type_index(typeid(vector<double>));
  REQUIRE(median_kwrapper2.getTypeIndex() == type_ind_data);
}

TEST_CASE("Testing:template kernel_wrapper write & read","[unit,panacea]") {

  std::vector<std::vector<double>> data = {{1.0, 4.0},{2.0, 9.0},{6.0, 2.0}};
  DescriptorWrapper<vector<vector<double>>*> dwrapper(&data,3,2); 

  auto type_ind_data = std::type_index(typeid(vector<vector<double>>));
  KernelWrapper<vector<vector<double>> *> kwrapper(test::Test::key(), &dwrapper);
  BaseKernelWrapper * kwrapper_ptr = &kwrapper;

  std::fstream fs;
  fs.open("template_kernel_wrapper_restart.txt", std::fstream::out );
  BaseKernelWrapper::write(settings::FileType::TXTRestart, fs, kwrapper_ptr);
  fs.close();

  KernelWrapper<vector<vector<double>>> kwrapper2(test::Test::key());
  kwrapper_ptr = &kwrapper2;
  std::fstream fs2;
  fs2.open("template_kernel_wrapper_restart.txt", std::fstream::in );
  BaseKernelWrapper::read(settings::FileType::TXTRestart, fs2, kwrapper_ptr);
  fs2.close();

  REQUIRE(kwrapper2.getNumberDimensions() == 2);
  REQUIRE(kwrapper2.getNumberPoints() == 3);
  REQUIRE(kwrapper2.rows() == 3);
  REQUIRE(kwrapper2.cols() == 2);

  REQUIRE(kwrapper2.at(0,0) == Approx(1.0));
  REQUIRE(kwrapper2.at(1,0) == Approx(2.0));
  REQUIRE(kwrapper2.at(2,0) == Approx(6.0));
  REQUIRE(kwrapper2.at(0,1) == Approx(4.0));
  REQUIRE(kwrapper2.at(1,1) == Approx(9.0));
  REQUIRE(kwrapper2.at(2,1) == Approx(2.0));
  REQUIRE(kwrapper2.getTypeIndex() == type_ind_data);
}

TEST_CASE("Testing:kernel_wrapper creation with factory","[unit,panacea]") {

  std::vector<std::vector<double>> data = {{1.0},{2.0},{6.0}};

  auto data_type_index = type_index(typeid(vector<vector<double>> *));
  KernelWrapperFactory kern_factory;
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,1); 
  WHEN("Testing kernel wrapper creation one to one gaussian"){
    KernelSpecification specs(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible);

    auto kwrapper = kern_factory.create(&dwrapper, specs);
  }

}

TEST_CASE("Testing:kernel_wrappers with update","[unit,panacea]") {

  // 1.0 + 2.0 + 6.0 = 9.0
  std::vector<std::vector<double>> data = {{1.0},{2.0},{6.0}};
  
  DescriptorWrapper<vector<vector<double>> *> dwrapper(&data,3,1); 
  
  std::vector<std::vector<double>> data2 = {{5.0},{8.0},{3.0}, {9.0}, {6.0}};
  DescriptorWrapper<vector<vector<double>> *> dwrapper2(&data2,5,1); 

  WHEN("Testing mean kernel wrapper"){
    MeanKernelWrapper mean_kwrapper(test::Test::key(), &dwrapper);
    mean_kwrapper.update(&dwrapper2);
    REQUIRE(mean_kwrapper.getNumberDimensions() == 1);
    REQUIRE(mean_kwrapper.getNumberPoints() == 8);
    //
    // data     1.0 + 2.0 + 6.0 = 9.0
    // data2    5.0 + 8.0 + 3.0 + 9.0 + 6.0 = 31.0
    //
    // 9.0 + 31.0 = 40.0 
    //
    // 40.0 / 8.0 = 5.0
    REQUIRE(mean_kwrapper.at(0,0) == Approx(5.0)); 
  }

  WHEN("Testing median kernel wrapper"){
    MedianKernelWrapper median_kwrapper(test::Test::key(), &dwrapper);
    median_kwrapper.update(&dwrapper2);
    REQUIRE(median_kwrapper.getNumberDimensions() == 1);
    REQUIRE(median_kwrapper.getNumberPoints() == 8);
    // 
    // 1.0, 2.0, 3.0, 5.0, 6.0, 6.0, 8.0, 9.0
    //
    // (5.0 + 6.0) / 2.0 = 5.5
    REQUIRE(median_kwrapper.at(0,0) == Approx(5.5));
  }

  WHEN("Testing one to one kernel wrapper"){

    KernelWrapper<vector<vector<double>> *> kwrapper(
        test::Test::key(),
        std::any_cast<vector<vector<double>> *>(dwrapper.getPointerToRawData()),
        3,
        1);

    kwrapper.update(&dwrapper2);
    REQUIRE(kwrapper.getNumberDimensions() == 1);
    // Unlike mean and median the new points will
    // simply overwrite the existing
    REQUIRE(kwrapper.getNumberPoints() == 5);

    REQUIRE(kwrapper.at(0,0) == Approx(5.0));
    REQUIRE(kwrapper.at(1,0) == Approx(8.0));
    REQUIRE(kwrapper.at(2,0) == Approx(3.0));
    REQUIRE(kwrapper.at(3,0) == Approx(9.0));
    REQUIRE(kwrapper.at(4,0) == Approx(6.0));

  }
}

TEST_CASE("Testing:kernel_wrapper_constructor1","[unit,panacea]"){

  std::vector<std::vector<double>> data;
  KernelWrapper<std::vector<std::vector<double>>*> 
    kwrapper(test::Test::key(), &data,0,0);

}

TEST_CASE("Testing:kernel_wrapper_constructor2","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  KernelWrapper<std::vector<std::vector<double>>*> 
    kwrapper(test::Test::key(), &data,2, 3);

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

  KernelWrapper<std::vector<std::vector<double>>*> 
    kwrapper(test::Test::key(),&data,2,3);

  REQUIRE(kwrapper.at(0,0) == 1.0);
  REQUIRE(kwrapper.at(1,0) == 1.0);

  REQUIRE(kwrapper.at(0,1) == 2.0);
  REQUIRE(kwrapper.at(1,1) == 2.0);

  REQUIRE(kwrapper.at(0,2) == 3.0);
  REQUIRE(kwrapper.at(1,2) == 3.0);
}

TEST_CASE("Testing:kernel_arrangements","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  KernelWrapper<std::vector<std::vector<double>>*> 
    kwrapper(test::Test::key(),&data,2,3);

  // Flip how the rows and columns are being interpreted
  kwrapper.set(Arrangement::DimensionsAlongRowsPointsAlongCols);

  // Rows and columns don't change
  REQUIRE(kwrapper.rows() == 2);
  REQUIRE(kwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(kwrapper.getNumberDimensions() == 2);
  REQUIRE(kwrapper.getNumberPoints() == 3);

  // At gets with respect to rows and columns so should not change
  REQUIRE(kwrapper.at(0,0) == 1.0);
  REQUIRE(kwrapper.at(1,0) == 1.0);

  REQUIRE(kwrapper.at(0,1) == 2.0);
  REQUIRE(kwrapper.at(1,1) == 2.0);

  REQUIRE(kwrapper.at(0,2) == 3.0);
  REQUIRE(kwrapper.at(1,2) == 3.0);
}

