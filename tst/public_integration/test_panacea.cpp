
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
    std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(*dwrapper, panacea_settings);


    double self_ent_val1 = self_ent->compute(*dwrapper, panacea_settings);

    // Because we are dealing with a single point and the self entropy term
    // shares the data with the descriptor changing the location of the
    // descriptors should not change anything
    dwrapper->operator()(0,0) = 2.0;

    double self_ent_val2 = self_ent->compute(*dwrapper, panacea_settings);
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
    std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(*dwrapper, panacea_settings);
    double self_ent_val_stacked = self_ent->compute(*dwrapper, panacea_settings);

    // move the data points apart and self entropy should increase
    dwrapper->operator()(0,0) = 0.0;
    dwrapper->operator()(1,0) = 2.0;

    double self_ent_val_spread = self_ent->compute(*dwrapper, panacea_settings);
    REQUIRE(self_ent_val_stacked < self_ent_val_spread );
  }
}

TEST_CASE("Testing:panacea self entropy shell + initialize","[integration,panacea]"){

  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  WHEN("Using OneToOne mapping."){
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

      std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(panacea_settings);

      std::vector<std::vector<double>> data = {{ 1.0, 2.0 }};
      const int rows = 1;
      const int cols = 2;

      auto dwrapper = panacea_pi.wrap(&(data), rows, cols);

      self_ent->initialize(*dwrapper);
      double self_ent_val1 = self_ent->compute(*dwrapper, panacea_settings);

      // Because we are dealing with a single point and the self entropy term
      // shares the data with the descriptor changing the location of the
      // descriptors should not change anything,
      //
      // E.g. because there was initially a peak at position x = 1.0, y = 2.0 and by
      // default the entropy of the peaks is calculated moving the peak to x = 2.0 and y = 2.0
      // will not change anything because we will simply be calculating th entropy at the new
      // peak location. If the entropy term did not share the data then these values would
      // be different.
      dwrapper->operator()(0,0) = 2.0;

      double self_ent_val2 = self_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(self_ent_val1 ==  Approx(self_ent_val2) );

    }
    WHEN("Testing self entropy term with two points.") {
      std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(panacea_settings);
      // Data has the following form, where it is stacked
      //
      //         col1   col2   col3
      // row1    1.0     2.0    3.0  Point 1
      // row2    1.0     2.0    3.0  Point 2
      test::ArrayData array_data;
      const int rows = 2;
      const int cols = 3;
      auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);

      self_ent->initialize(*dwrapper);

      double self_ent_val_stacked = self_ent->compute(*dwrapper, panacea_settings);

      // move the data points apart and self entropy should increase
      dwrapper->operator()(0,0) = 0.0;
      dwrapper->operator()(1,0) = 2.0;

      double self_ent_val_spread = self_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(self_ent_val_stacked < self_ent_val_spread );
    }
  }
  WHEN("Using Single mapping."){
    PANACEASettings panacea_settings = PANACEASettings::make()
      .set(EntropyType::Self)
      .set(PANACEAAlgorithm::Flexible)
      .distributionType(kernel)
      .set(KernelPrimitive::Gaussian)
      .set(KernelCount::Single)
      .set(KernelCorrelation::Uncorrelated)
      .set(KernelCenterCalculation::Mean)
      .set(KernelNormalization::None);

    // pi - public interface
    PANACEA panacea_pi;

    WHEN("Testing self entropy term with single point.") {

      std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(panacea_settings);

      std::vector<std::vector<double>> data = {{ 1.0, 2.0 }};
      const int rows = 1;
      const int cols = 2;

      auto dwrapper = panacea_pi.wrap(&(data), rows, cols);

      // It should not be possible to create a self entropy term that is not
      // using OneToOne mapping, at least yet, calculating the gradiant for that
      // if the descriptors are being changed would prove difficult. E.g. if you are
      // using the median, what would calculate_grad look like?
      CHECK_THROWS(self_ent->initialize(*dwrapper));
    }

  }
}

TEST_CASE("Testing:panacea cross entropy shell + initialize","[integration,panacea]"){

  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  GIVEN("OneToOne mapping."){
    PANACEASettings panacea_settings = PANACEASettings::make()
      .set(EntropyType::Cross)
      .set(PANACEAAlgorithm::Flexible)
      .distributionType(kernel)
      .set(KernelPrimitive::Gaussian)
      .set(KernelCount::OneToOne)
      .set(KernelCorrelation::Uncorrelated)
      .set(KernelCenterCalculation::None)
      .set(KernelNormalization::None);

    // pi - public interface
    PANACEA panacea_pi;

    WHEN("Testing cross entropy term with single point.") {

      std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(panacea_settings);

      std::vector<std::vector<double>> data = {{ 1.0, 2.0 }};
      const int rows = 1;
      const int cols = 2;

      auto dwrapper = panacea_pi.wrap(&(data), rows, cols);

      cross_ent->initialize(*dwrapper);
      double cross_ent_val1 = cross_ent->compute(*dwrapper, panacea_settings);

      // Because we are dealing with a single point and the cross entropy term
      // by default does not share the data with the descriptor changing the location of the
      // descriptors should change the cross entropy value,
      //
      // E.g. because there was initially a peak at position x = 1.0, y = 2.0 and by
      // default the entropy of the peaks is calculated moving the sample point to x = 2.0 and y = 2.0
      // will change the cross entropy. If the entropy term did share the data then these values would
      // be the same.
      //
      // Cross entropy should be larger at a position displaced from the center
      dwrapper->operator()(0,0) = 2.0;

      double cross_ent_val2 = cross_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(cross_ent_val1 < cross_ent_val2 );

    }
    WHEN("Testing cross entropy term with two points.") {
      std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(panacea_settings);
      // Data has the following form, where it is stacked
      //
      //         col1   col2   col3
      // row1    1.0     2.0    3.0  Point 1
      // row2    1.0     2.0    3.0  Point 2
      test::ArrayData array_data;
      const int rows = 2;
      const int cols = 3;
      auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);

      cross_ent->initialize(*dwrapper);

      double cross_ent_val_stacked = cross_ent->compute(*dwrapper, panacea_settings);

      // move the data points apart and cross entropy should increase
      dwrapper->operator()(0,0) = 0.0;
      dwrapper->operator()(1,0) = 2.0;

      double cross_ent_val_spread = cross_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(cross_ent_val_stacked < cross_ent_val_spread );
    }
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
    std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(*dwrapper, panacea_settings);
    double self_ent_val_stacked = self_ent->compute(*dwrapper, panacea_settings);


    double self_ent_val = self_ent->compute(*dwrapper, panacea_settings);
    auto restart_file = panacea_pi.create(settings::FileType::TXTRestart);
    restart_file->write(self_ent.get(),"self_entropy_restart.txt");

    std::unique_ptr<EntropyTerm> self_ent2 = panacea_pi.create(panacea_settings);

    restart_file->read(self_ent2.get(),"self_entropy_restart.txt");
    double self_ent_val2 = self_ent2->compute(*dwrapper, panacea_settings);

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
  std::unique_ptr<EntropyTerm> self_ent = panacea_pi.create(*dwrapper, panacea_settings);

  double self_ent_two_pts = self_ent->compute(*dwrapper, panacea_settings);

  // Let's try shrinking the data to a single point
  data.resize(1);

  rows = 1;
  // We have to rewrap the data
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(*dwrapper);

  double self_ent_single_pt = self_ent->compute(*dwrapper, panacea_settings);

  std::cout << "self entropy should decrease when we move from two points spread apart";
  std::cout << " to a single point." << std::endl;
  std::cout << "Two points " << self_ent_two_pts << " single point " << self_ent_single_pt << std::endl;
  REQUIRE( self_ent_single_pt < self_ent_two_pts );

  // Increase the size and diversity the self entropy should increase
  data.emplace_back(10.0,10.0);
  data.emplace_back(20.0,20.0);
  rows = 3;
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(*dwrapper);
  double self_ent_three_pts = self_ent->compute(*dwrapper, panacea_settings);
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
  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(
      *dwrapper,
      panacea_settings);

  WHEN("Descriptors are spread out vs stacked"){
    double cross_ent_val_spread = cross_ent->compute(*dwrapper, panacea_settings);
    // move the data points to the mean and cross entropy should decrease
    dwrapper->operator()(0,0) = 2.0;
    dwrapper->operator()(1,0) = 2.0;

    double cross_ent_val_stacked = cross_ent->compute(*dwrapper, panacea_settings);
    REQUIRE(cross_ent_val_stacked < cross_ent_val_spread );
  }
  WHEN("Numerical cross entropy is compared with analytical"){
    dwrapper->operator()(0,0) = 1.0;
    dwrapper->operator()(1,0) = 2.0;
    dwrapper->operator()(0,1) = 0.0;
    dwrapper->operator()(1,1) = 0.0;
    dwrapper->operator()(0,2) = 0.0;
    dwrapper->operator()(1,2) = 0.0;

    int index_loc = 0;
    auto grad_center = cross_ent->compute_grad(
        *dwrapper,
        index_loc,
        panacea_settings);

    dwrapper->operator()(0,0) = 0.99;
    const double val_left = cross_ent->compute(*dwrapper,0, panacea_settings);
    dwrapper->operator()(0,0) = 1.01;
    const double val_right = cross_ent->compute(*dwrapper,0, panacea_settings);

    const double diff = 1.01 - 0.99;
    std::cout << "val right " << val_right << std::endl;
    std::cout << "val left  " << val_left << std::endl;

    std::cout << "Numerical grad " << (val_right-val_left)/diff << std::endl;
    std::cout << "Grad analy" << std::endl;
    for(auto val : grad_center ) std::cout << val << " ";
    std::cout << std::endl;
    REQUIRE((val_right-val_left)/diff == Approx(grad_center.at(0)));

  }

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

  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(*dwrapper_init, panacea_settings);

  std::vector<std::vector<double>> sample_pts = {
    {0.95, 0.0, 0.0},
    {1.0, 0.0, 0.0},
    {1.05, 0.0, 0.0}};
  auto dwrapper_sample = panacea_pi.wrap(&sample_pts, 3, 3);

  // Show that cross entropy is lowest at 1.0 0.0 0.0
  double lower_cross_ent  = cross_ent->compute(*dwrapper_sample, 0, panacea_settings);
  double center_cross_ent = cross_ent->compute(*dwrapper_sample, 1, panacea_settings);
  double upper_cross_ent  = cross_ent->compute(*dwrapper_sample, 2, panacea_settings);

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

  cross_ent->update(*dwrapper);

  // move the sample points
  sample_pts.at(0).at(0) += 2.0; // Now at 2.95
  sample_pts.at(1).at(0) += 2.0; // Now at 3.0
  sample_pts.at(2).at(0) += 2.0; // Now at 3.05

  lower_cross_ent  = cross_ent->compute(*dwrapper_sample, 0, panacea_settings);
  center_cross_ent = cross_ent->compute(*dwrapper_sample, 1, panacea_settings);
  upper_cross_ent  = cross_ent->compute(*dwrapper_sample, 2, panacea_settings);

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

  cross_ent->update(*dwrapper);
  // The new mean will have moved from 3 to 20/5 = 4
  // move the sample points
  sample_pts.at(0).at(0) += 1.0; // Now at 3.95
  sample_pts.at(1).at(0) += 1.0; // Now at 4.0
  sample_pts.at(2).at(0) += 1.0; // Now at 4.05

  lower_cross_ent  = cross_ent->compute(*dwrapper_sample, 0, panacea_settings);
  center_cross_ent = cross_ent->compute(*dwrapper_sample, 1, panacea_settings);
  upper_cross_ent  = cross_ent->compute(*dwrapper_sample, 2, panacea_settings);

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

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  auto dwrapper_init = panacea_pi.wrap(&(kern_init_data), rows, cols);
  REQUIRE(dwrapper_init != nullptr);
  REQUIRE(dwrapper_init.get() != nullptr);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;

  std::unique_ptr<EntropyTerm> cross_ent = panacea_pi.create(*dwrapper_init, panacea_settings);

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  cross_ent->compute(*dwrapper_init, panacea_settings);
  // Because we are calculating the median the memory will not be shared
  // it is ok at this point to delete the initial data

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  delete[] kern_init_data[0];
  delete[] kern_init_data[1];
  delete[] kern_init_data;

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  auto desc_data = new double*[1];
  desc_data[0] = new double[cols];
  desc_data[0][0] = 70.0; // The median of three points 70 and 10 and 10 = 10
  desc_data[0][1] = 0.0; // The median of three points 70 and 10 and 10 = 10
  desc_data[0][2] = 0.0; // The median of three points 70 and 10 and 10 = 10

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  // Using only a single row
  auto dwrapper = panacea_pi.wrap(&(desc_data), 1, cols);

  REQUIRE(dwrapper != nullptr);
  REQUIRE(dwrapper.get() != nullptr);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  // The mean is currently at 10 and this new point is at 70 the cross entropy should be large
  double cross_ent_val_single_pt_before_update = cross_ent->compute(*dwrapper, panacea_settings);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  cross_ent->update(*dwrapper);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  // After update the mean is at 10 and this new point is at 70 the
  // cross entropy should remain the same
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  double cross_ent_val_single_pt_after_update = cross_ent->compute(*dwrapper, panacea_settings);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  REQUIRE(cross_ent_val_single_pt_after_update == Approx(cross_ent_val_single_pt_before_update));
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;

  delete[] desc_data[0];
  delete[] desc_data;
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;

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

  std::unique_ptr<EntropyTerm> cross_ent1 = panacea_pi.create(*dwrapper_init, panacea_settings1);
  std::unique_ptr<EntropyTerm> cross_ent2 = panacea_pi.create(*dwrapper_init, panacea_settings2);

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
  double val1 = cross_ent1->compute(*dwrapper, panacea_settings1);
  double val2 = cross_ent2->compute(*dwrapper, panacea_settings2);

  REQUIRE(val1 > 0.0 );
  REQUIRE(val2 < 0.0 );
  REQUIRE(std::abs(val2) == Approx(val1));

  delete[] desc_data[0];
  delete[] desc_data;

}


