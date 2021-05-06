
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

TEST_CASE("Testing:panacea self entropy read & write with fileio","[integration,panacea]"){

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


    double self_ent_val = self_ent->compute(dwrapper.get());
    auto restart_file = panacea_pi.create(settings::FileType::TXTRestart);
    restart_file->write(self_ent.get(),"self_entropy_restart.txt"); 

    std::unique_ptr<EntropyTerm> self_ent2 = panacea_pi.create(panacea_settings);

    restart_file->read(self_ent2.get(),"self_entropy_restart.txt");
    double self_ent_val2 = self_ent2->compute(dwrapper.get());

    REQUIRE(self_ent_val == Approx(self_ent_val2));
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

TEST_CASE("Testing:panacea cross entropy single mean update","[integration,panacea]"){

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
  // row1   10.0     0.0    0.0  Point 1
  // row2   10.0     0.0    0.0  Point 2
  int rows = 2;
  int cols = 3;

  auto kern_init_data = new double*[rows];
  kern_init_data[0] = new double[cols]; 
  kern_init_data[1] = new double[cols]; 

  kern_init_data[0][0] = 1.0;
  kern_init_data[1][0] = 1.0;
  kern_init_data[0][1] =  0.0;
  kern_init_data[1][1] =  0.0;
  kern_init_data[0][2] =  0.0;
  kern_init_data[1][2] =  0.0;

  auto dwrapper_init = panacea_pi.wrap(&(kern_init_data), rows, cols);

  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(dwrapper_init.get(), panacea_settings);

  std::vector<std::vector<double>> sample_pts = {
    {0.95, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.05, 0.0, 0.0}};
  auto dwrapper_sample = panacea_pi.wrap(&sample_pts, 3, 3);

  // Show that cross entropy is lowest at 1.0 0.0 0.0
  double lower_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 0); 
  double center_cross_ent = cross_ent->compute(dwrapper_sample.get(), 1); 
  double upper_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 2); 

  REQUIRE(lower_cross_ent > center_cross_ent );
  REQUIRE(lower_cross_ent == Approx(upper_cross_ent));
 
  // Because we are calculating the mean the memory will not be shared
  // it is ok at this point to delete the initial data

  delete[] kern_init_data[0];
  delete[] kern_init_data[1];
  delete[] kern_init_data;

  auto desc_data = new double*[1];
  desc_data[0] = new double[cols]; 
  desc_data[0][0] = 7.0; // The mean of three points 7 and 1 and 1 = 9/3 = 3
  desc_data[0][1] = 0.0; 
  desc_data[0][2] = 0.0; 

  // Using only a single row
  auto dwrapper = panacea_pi.wrap(&(desc_data), 1, cols);
  
  cross_ent->update(dwrapper.get());

  // move the sample points 
  sample_pts.at(0).at(0) += 2.0; // Now at 2.95
  sample_pts.at(1).at(0) += 2.0; // Now at 3.0 
  sample_pts.at(2).at(0) += 2.0; // Now at 3.05

  lower_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 0); 
  center_cross_ent = cross_ent->compute(dwrapper_sample.get(), 1); 
  upper_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 2); 

  REQUIRE(lower_cross_ent > center_cross_ent );
  REQUIRE(lower_cross_ent == Approx(upper_cross_ent));

  delete[] desc_data[0];
  delete[] desc_data;

  auto desc_data2 = new double*[2];
  desc_data2[0] = new double[cols]; 
  desc_data2[1] = new double[cols]; 
  desc_data2[0][0] = 1.0; 
  desc_data2[0][1] = 0.0; 
  desc_data2[0][2] = 0.0; 
  desc_data2[1][0] = 10.0; 
  desc_data2[1][1] = 0.0; 
  desc_data2[1][2] = 0.0; 

  // Right now the mean is at 3 
  dwrapper = panacea_pi.wrap(&(desc_data2), 2, cols);

  cross_ent->update(dwrapper.get());
  // The new mean will have moved from 3 to 20/5 = 4
  // move the sample points 
  sample_pts.at(0).at(0) += 1.0; // Now at 3.95
  sample_pts.at(1).at(0) += 1.0; // Now at 4.0 
  sample_pts.at(2).at(0) += 1.0; // Now at 4.05

  lower_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 0); 
  center_cross_ent = cross_ent->compute(dwrapper_sample.get(), 1); 
  upper_cross_ent  = cross_ent->compute(dwrapper_sample.get(), 2); 

  REQUIRE(lower_cross_ent > center_cross_ent );
  REQUIRE(lower_cross_ent == Approx(upper_cross_ent));

  delete[] desc_data2[0];
  delete[] desc_data2[1];
  delete[] desc_data2;

}

TEST_CASE("Testing:panacea cross entropy single median update","[integration,panacea]"){

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
                                            .set(KernelCenterCalculation::Median)
                                            .set(KernelNormalization::Variance);

  // pi - public interface
  PANACEA panacea_pi;

  // Data has the following form, where it is stacked
  //
  //         col1   col2   col3
  // row1   10.0     0.0    0.0  Point 1
  // row2   10.0     0.0    0.0  Point 2
  int rows = 2;
  int cols = 3;

  auto kern_init_data = new double*[rows];
  kern_init_data[0] = new double[cols]; 
  kern_init_data[1] = new double[cols]; 

  kern_init_data[0][0] = 10.0;
  kern_init_data[1][0] = 10.0;
  kern_init_data[0][1] =  0.0;
  kern_init_data[1][1] =  0.0;
  kern_init_data[0][2] =  0.0;
  kern_init_data[1][2] =  0.0;

  auto dwrapper_init = panacea_pi.wrap(&(kern_init_data), rows, cols);

  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(dwrapper_init.get(), panacea_settings);

  // Because we are calculating the median the memory will not be shared
  // it is ok at this point to delete the initial data

  delete[] kern_init_data[0];
  delete[] kern_init_data[1];
  delete[] kern_init_data;

  auto desc_data = new double*[1];
  desc_data[0] = new double[cols]; 
  desc_data[0][0] = 70.0; // The median of three points 70 and 10 and 10 = 10
  desc_data[0][1] = 0.0; // The median of three points 70 and 10 and 10 = 10
  desc_data[0][2] = 0.0; // The median of three points 70 and 10 and 10 = 10
  
  // Using only a single row
  auto dwrapper = panacea_pi.wrap(&(desc_data), 1, cols);
  // The mean is currently at 10 and this new point is at 70 the cross entropy should be large
  double cross_ent_val_single_pt_before_update = cross_ent->compute(dwrapper.get());
  cross_ent->update(dwrapper.get());
  // After update the mean is at 10 and this new point is at 70 the
  // cross entropy should remain the same 
  double cross_ent_val_single_pt_after_update = cross_ent->compute(dwrapper.get());
  REQUIRE(cross_ent_val_single_pt_after_update == Approx(cross_ent_val_single_pt_before_update));

  delete[] desc_data[0];
  delete[] desc_data;

}

TEST_CASE("Testing:panacea cross entropy single median weighted","[integration,panacea]"){

  PANACEASettings panacea_settings1 = PANACEASettings::make()
                                        .set(EntropyType::Cross)
                                        .set(PANACEAAlgorithm::Flexible)
                                        .distributionType(kernel)
                                            .set(KernelPrimitive::Gaussian)
                                            .set(KernelCount::Single)
                                            .set(KernelCorrelation::Correlated)
                                            .set(KernelCenterCalculation::Median)
                                            .set(KernelNormalization::Variance)
                                            .weightEntropTermBy(1.0);

  PANACEASettings panacea_settings2 = PANACEASettings::make()
                                        .set(EntropyType::Cross)
                                        .set(PANACEAAlgorithm::Flexible)
                                        .distributionType(kernel)
                                            .set(KernelPrimitive::Gaussian)
                                            .set(KernelCount::Single)
                                            .set(KernelCorrelation::Correlated)
                                            .set(KernelCenterCalculation::Median)
                                            .set(KernelNormalization::Variance)
                                            .weightEntropTermBy(-1.0);

  // pi - public interface
  PANACEA panacea_pi;

  // Data has the following form, where it is stacked
  //
  //         col1   col2   col3
  // row1   1.0     0.0    0.0  Point 1
  // row2   3.0     0.0    0.0  Point 2
  int rows = 2;
  int cols = 3;

  auto kern_init_data = new double*[rows];
  kern_init_data[0] = new double[cols]; 
  kern_init_data[1] = new double[cols]; 

  kern_init_data[0][0] = 1.0;
  kern_init_data[1][0] = 3.0;
  kern_init_data[0][1] = 0.0;
  kern_init_data[1][1] = 0.0;
  kern_init_data[0][2] = 0.0;
  kern_init_data[1][2] = 0.0;

  auto dwrapper_init = panacea_pi.wrap(&(kern_init_data), rows, cols);

  std::unique_ptr<EntropyTerm> cross_ent1 = panacea_pi.create(dwrapper_init.get(), panacea_settings1);
  std::unique_ptr<EntropyTerm> cross_ent2 = panacea_pi.create(dwrapper_init.get(), panacea_settings2);

  // Because we are calculating the median the memory will not be shared
  // it is ok at this point to delete the initial data

  delete[] kern_init_data[0];
  delete[] kern_init_data[1];
  delete[] kern_init_data;

  auto desc_data = new double*[1];
  desc_data[0] = new double[cols]; 
  desc_data[0][0] = 2.0;
  desc_data[0][1] = 0.0;
  desc_data[0][2] = 0.0;
  
  // Using only a single row
  auto dwrapper = panacea_pi.wrap(&(desc_data), 1, cols);
  // The mean is currently at 10 and this new point is at 70 the cross entropy should be large
  double val1 = cross_ent1->compute(dwrapper.get());
  double val2 = cross_ent2->compute(dwrapper.get());
 
  REQUIRE(val1 > 0.0 );
  REQUIRE(val2 < 0.0 );
  REQUIRE(std::abs(val2) == Approx(val1));

  delete[] desc_data[0];
  delete[] desc_data;

}


