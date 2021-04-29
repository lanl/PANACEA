
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

TEST_CASE("Testing:panacea-descriptor_wrapper","[integration,panacea]"){

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

TEST_CASE("Testing:panacea self entropy","[integration,panacea]"){

  // Creating settings for generating a self entropy term where the 
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  PANACEASettings panacea_settings = PANACEASettings::make()
                                        .set(EntropyType::Self)
                                        .set(PANACEAAlgorithm::Flexible)
                                        .distributionType(kernel)
                                            .set(KernelPrimitive::Gaussian)
                                            .set(KernelCount::OneToOne)
                                            .set(KernelCorrelation::Uncorrelated)
                                            .set(KernelCenterCalculation::None)
                                            .set(KernelNormalization::None);

  // pi - public interface
  PANACEA panacea_pi;

  WHEN("Testing self entropy term with single point.") {
    std::vector<std::vector<double>> data = {{ 1.0, 2.0 }};
    const int rows = 1;
    const int cols = 2;

    auto dwrapper = panacea_pi.wrap(&(data), rows, cols);
    std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(dwrapper.get(), panacea_settings);


    double self_ent_val1 = self_ent->compute(dwrapper.get());

    // Because we are dealing with a single point and the self entropy term 
    // shares the data with the descriptor changing the location of the 
    // descriptors should not change anything
    dwrapper->operator()(0,0) = 2.0;

    double self_ent_val2 = self_ent->compute(dwrapper.get());
    REQUIRE(self_ent_val1 ==  Approx(self_ent_val2) );

  }
  WHEN("Testing self entropy term with two points.") {
    // Data has the following form, where it is stacked
    //
    //         col1   col2   col3
    // row1    1.0     2.0    3.0  Point 1
    // row2    1.0     2.0    3.0  Point 2
    test::ArrayData array_data;
    const int rows = 2;
    const int cols = 3;
    auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);
    std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(dwrapper.get(), panacea_settings);
    double self_ent_val_stacked = self_ent->compute(dwrapper.get());

    // move the data points apart and self entropy should increase
    dwrapper->operator()(0,0) = 0.0;
    dwrapper->operator()(1,0) = 2.0;

    double self_ent_val_spread = self_ent->compute(dwrapper.get());
    REQUIRE(self_ent_val_stacked < self_ent_val_spread );
  }
}

TEST_CASE("Testing:panacea self entropy update","[integration,panacea]"){

  // Creating settings for generating a self entropy term where the 
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  PANACEASettings panacea_settings = PANACEASettings::make()
                                        .set(EntropyType::Self)
                                        .set(PANACEAAlgorithm::Flexible)
                                        .distributionType(kernel)
                                            .set(KernelPrimitive::Gaussian)
                                            .set(KernelCount::OneToOne)
                                            .set(KernelCorrelation::Uncorrelated)
                                            .set(KernelCenterCalculation::None)
                                            .set(KernelNormalization::None);

  // pi - public interface
  PANACEA panacea_pi;

  // Start with just two points
  std::vector<std::vector<double>> data = {
    { 0.0, 0.0 },
    { 10.0, 10.0 }};
  int rows = 2;
  const int cols = 2;

  auto dwrapper = panacea_pi.wrap(&(data), rows, cols);
  std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(dwrapper.get(), panacea_settings);

  double self_ent_two_pts = self_ent->compute(dwrapper.get());

  // Let's try shrinking the data to a single point
  data.resize(1);
 
  rows = 1; 
  // We have to rewrap the data
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(dwrapper.get());

  double self_ent_single_pt = self_ent->compute(dwrapper.get());
 
  std::cout << "self entropy should decrease when we move from two points spread apart";
  std::cout << " to a single point." << std::endl;
  std::cout << "Two points " << self_ent_two_pts << " single point " << self_ent_single_pt << std::endl; 
  REQUIRE( self_ent_single_pt < self_ent_two_pts );

  // Increase the size and diversity the self entropy should increase
  data.emplace_back(10.0,10.0);
  data.emplace_back(20.0,20.0);
  rows = 3;
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(dwrapper.get());
  double self_ent_three_pts = self_ent->compute(dwrapper.get());
  std::cout << "self entropy should increase when we move from two points spread apart";
  std::cout << " to three poitns spread apart." << std::endl;
  std::cout << "Two points " << self_ent_two_pts << " three points " << self_ent_three_pts << std::endl;
  REQUIRE( self_ent_two_pts < self_ent_three_pts );

}

TEST_CASE("Testing:panacea cross entropy single mean","[integration,panacea]"){

  // Creating settings for generating a self entropy term where the 
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  PANACEASettings panacea_settings = PANACEASettings::make()
                                        .set(EntropyType::Cross)
                                        .set(PANACEAAlgorithm::Flexible)
                                        .distributionType(kernel)
                                            .set(KernelPrimitive::Gaussian)
                                            .set(KernelCount::Single)
                                            .set(KernelCorrelation::Correlated)
                                            .set(KernelCenterCalculation::Mean)
                                            .set(KernelNormalization::None);

  // pi - public interface
  PANACEA panacea_pi;

  // Data has the following form, where it is stacked
  //
  //         col1   col2   col3
  // row1    1.0     0.0    0.0  Point 1
  // row2    3.0     0.0    0.0  Point 2
  test::ArrayData array_data;
  const int rows = 2;
  const int cols = 3;
  array_data.data[0][0] = 1.0;
  array_data.data[1][0] = 3.0;
  auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);

  // Will create a cross entropy term centered at
  // 
  //         col1   col2   col3
  // row1    2.0     0.0    0.0  Point 1
  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(dwrapper.get(), panacea_settings);

  double cross_ent_val_spread = cross_ent->compute(dwrapper.get());

  // move the data points to the mean and cross entropy should decrease
  dwrapper->operator()(0,0) = 2.0;
  dwrapper->operator()(1,0) = 2.0;

  double cross_ent_val_stacked = cross_ent->compute(dwrapper.get());
  REQUIRE(cross_ent_val_stacked < cross_ent_val_spread );
}
