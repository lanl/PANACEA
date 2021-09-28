
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/entropy_term.hpp"
#include "panacea/panacea.hpp"

// Local private includes
#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <cmath>
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;
using namespace panacea::settings;

TEST_CASE("Testing:panacea self entropy", "[end-to-end,panacea]") {

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
    std::vector<std::vector<double>> data = {{1.0, 2.0}};
    const int rows = 1;
    const int cols = 2;

    auto dwrapper = panacea_pi.wrap(&(data), rows, cols);
    std::unique_ptr<EntropyTerm> self_ent =
        panacea_pi.create(*dwrapper, panacea_settings);

    double self_ent_val1 = self_ent->compute(*dwrapper, panacea_settings);

    // Because we are dealing with a single point and the self entropy term
    // shares the data with the descriptor changing the location of the
    // descriptors should not change anything
    dwrapper->operator()(0, 0) = 2.0;

    double self_ent_val2 = self_ent->compute(*dwrapper, panacea_settings);
    REQUIRE(self_ent_val1 == Approx(self_ent_val2));
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
    std::unique_ptr<EntropyTerm> self_ent =
        panacea_pi.create(*dwrapper, panacea_settings);
    double self_ent_val_stacked =
        self_ent->compute(*dwrapper, panacea_settings);

    // move the data points apart and self entropy should increase
    dwrapper->operator()(0, 0) = 0.0;
    dwrapper->operator()(1, 0) = 2.0;

    double self_ent_val_spread = self_ent->compute(*dwrapper, panacea_settings);
    REQUIRE(self_ent_val_stacked < self_ent_val_spread);
  }
}

TEST_CASE("Testing:panacea randomize dimensions non trivial self entropy",
          "[end-to-end,panacea]") {

  // pi - public interface
  PANACEA panacea_pi;
  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.

  PANACEASettings panacea_settings = PANACEASettings::make()
                                         .set(EntropyType::Self)
                                         .set(PANACEAAlgorithm::Flexible)
                                         .distributionType(kernel)
                                         .set(RandomizeDimensions::Yes)
                                         .set(KernelPrimitive::Gaussian)
                                         .set(KernelCount::OneToOne)
                                         .set(KernelCorrelation::Uncorrelated)
                                         .set(KernelCenterCalculation::None)
                                         .set(KernelNormalization::None);

  // Using data generated for an atomic configuration of
  // 21 atoms and 30 SNAP descriptors
  test::ArrayDataNonTrivial array_data;
  auto dwrapper =
      panacea_pi.wrap(&(array_data.data), array_data.rows, array_data.cols);

  std::unique_ptr<EntropyTerm> self_ent =
      panacea_pi.create(*dwrapper, panacea_settings);

  auto dimensions = self_ent->getDimensions();

  // There should be enough dimensions that the order is always
  // different
  bool in_order = true;
  int index = 0;
  for (auto &dim : dimensions) {
    if (dim != index) {
      in_order = false;
      break;
    }
    ++index;
  }

  REQUIRE(in_order == false);
}

TEST_CASE("Testing:panacea non trivial self entropy with strict alg",
          "[end-to-end,panacea]") {

  // pi - public interface
  PANACEA panacea_pi;
  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  WHEN("positively vs negatively weighted.") {

    PANACEASettings panacea_settings = PANACEASettings::make()
                                           .set(EntropyType::Self)
                                           .set(PANACEAAlgorithm::Strict)
                                           .distributionType(kernel)
                                           .weightEntropyTermBy(1.0)
                                           .set(KernelPrimitive::Gaussian)
                                           .set(KernelCount::OneToOne)
                                           .set(KernelCorrelation::Uncorrelated)
                                           .set(KernelCenterCalculation::None)
                                           .set(KernelNormalization::None);

    // Using data generated for an atomic configuration of
    // 21 atoms and 30 SNAP descriptors
    test::ArrayDataNonTrivial array_data;
    auto dwrapper =
        panacea_pi.wrap(&(array_data.data), array_data.rows, array_data.cols);

    std::unique_ptr<EntropyTerm> self_ent =
        panacea_pi.create(*dwrapper, panacea_settings);
    double positive_entropy = self_ent->compute(*dwrapper, panacea_settings);

    REQUIRE(positive_entropy > 0);

    // Switch the weight
    self_ent->set(settings::EntropyOption::Weight, -1.0);

    double negative_entropy = self_ent->compute(*dwrapper, panacea_settings);

    REQUIRE(negative_entropy < 0);
    REQUIRE(std::fabs(negative_entropy) == Approx(positive_entropy));
  }
}

TEST_CASE("Testing:panacea self entropy shell + initialize",
          "[end-to-end,panacea]") {

  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  WHEN("Using OneToOne mapping.") {
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

      std::unique_ptr<EntropyTerm> self_ent =
          panacea_pi.create(panacea_settings);

      std::vector<std::vector<double>> data = {{1.0, 2.0}};
      const int rows = 1;
      const int cols = 2;

      auto dwrapper = panacea_pi.wrap(&(data), rows, cols);

      self_ent->initialize(*dwrapper);
      double self_ent_val1 = self_ent->compute(*dwrapper, panacea_settings);

      // Because we are dealing with a single point and the self entropy term
      // shares the data with the descriptor changing the location of the
      // descriptors should not change anything,
      //
      // E.g. because there was initially a peak at position x = 1.0, y = 2.0
      // and by default the entropy of the peaks is calculated moving the peak
      // to x = 2.0 and y = 2.0 will not change anything because we will simply
      // be calculating th entropy at the new peak location. If the entropy
      // term did not share the data then these values would be different.
      //
      dwrapper->operator()(0, 0) = 2.0;

      double self_ent_val2 = self_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(self_ent_val1 == Approx(self_ent_val2));
    }
    WHEN("Testing self entropy term with two points.") {
      std::unique_ptr<EntropyTerm> self_ent =
          panacea_pi.create(panacea_settings);
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

      double self_ent_val_stacked =
          self_ent->compute(*dwrapper, panacea_settings);

      // move the data points apart and self entropy should increase
      dwrapper->operator()(0, 0) = 0.0;
      dwrapper->operator()(1, 0) = 2.0;

      double self_ent_val_spread =
          self_ent->compute(*dwrapper, panacea_settings);
      REQUIRE(self_ent_val_stacked < self_ent_val_spread);
    }
  }
  WHEN("Using Single mapping.") {
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

      std::unique_ptr<EntropyTerm> self_ent =
          panacea_pi.create(panacea_settings);

      std::vector<std::vector<double>> data = {{1.0, 2.0}};
      const int rows = 1;
      const int cols = 2;

      auto dwrapper = panacea_pi.wrap(&(data), rows, cols);

      // It should not be possible to create a self entropy term that is not
      // using OneToOne mapping, at least yet, calculating the gradiant for
      // that if the descriptors are being changed would prove difficult. E.g.
      // if you are using the median, what would calculate_grad look like?

      CHECK_THROWS(self_ent->initialize(*dwrapper));
    }
  }
}

TEST_CASE("Testing:panacea self entropy read & write with fileio",
          "[end-to-end,panacea]") {

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

  GIVEN("A self entropy term") {
    // Creating settings for generating a self entropy term where the
    // underlying distribution is using a kernel estimator
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

    std::unique_ptr<EntropyTerm> self_ent =
        panacea_pi.create(*dwrapper, panacea_settings);
    double self_ent_val_stacked =
        self_ent->compute(*dwrapper, panacea_settings);

    double self_ent_val = self_ent->compute(*dwrapper, panacea_settings);
    auto restart_file = panacea_pi.create(settings::FileType::TXTRestart);

    WHEN("provided with file_name") {
      restart_file->write(self_ent.get(), "self_entropy_restart.txt");

      std::unique_ptr<EntropyTerm> self_ent2 =
          panacea_pi.create(panacea_settings);

      restart_file->read(self_ent2.get(), "self_entropy_restart.txt");
      double self_ent_val2 = self_ent2->compute(*dwrapper, panacea_settings);

      REQUIRE(self_ent_val == Approx(self_ent_val2));
    }
    WHEN("provided with file stream") {

      ofstream restart_out;
      restart_out.open("self_entropy_restart2.txt");
      restart_file->write(self_ent.get(), restart_out);
      restart_out.close();
      std::unique_ptr<EntropyTerm> self_ent2 =
          panacea_pi.create(panacea_settings);

      ifstream restart_in;
      restart_in.open("self_entropy_restart2.txt");
      restart_file->read(self_ent2.get(), restart_in);
      restart_in.close();
      double self_ent_val2 = self_ent2->compute(*dwrapper, panacea_settings);

      REQUIRE(self_ent_val == Approx(self_ent_val2));
    }
  }

  GIVEN("A weighted self entropy term") {
    // Creating settings for generating a self entropy term where the
    // underlying distribution is using a kernel estimator
    // that is a guassian kernel.
    PANACEASettings panacea_settings = PANACEASettings::make()
                                           .set(EntropyType::Self)
                                           .weightEntropyTermBy(2.0)
                                           .set(PANACEAAlgorithm::Flexible)
                                           .distributionType(kernel)
                                           .set(KernelPrimitive::Gaussian)
                                           .set(KernelCount::OneToOne)
                                           .set(KernelCorrelation::Uncorrelated)
                                           .set(KernelCenterCalculation::None)
                                           .set(KernelNormalization::None);

    std::unique_ptr<EntropyTerm> self_ent =
        panacea_pi.create(*dwrapper, panacea_settings);
    double self_ent_val_stacked =
        self_ent->compute(*dwrapper, panacea_settings);

    double self_ent_val = self_ent->compute(*dwrapper, panacea_settings);
    auto restart_file = panacea_pi.create(settings::FileType::TXTRestart);

    WHEN("provided with file_name") {
      restart_file->write(self_ent.get(), "weighted_self_entropy_restart.txt");

      std::unique_ptr<EntropyTerm> self_ent2 =
          panacea_pi.create(panacea_settings);

      restart_file->read(self_ent2.get(), "weighted_self_entropy_restart.txt");
      double self_ent_val2 = self_ent2->compute(*dwrapper, panacea_settings);

      REQUIRE(self_ent_val == Approx(self_ent_val2));
    }
    WHEN("provided with file stream") {

      ofstream restart_out;
      restart_out.open("weighted_self_entropy_restart2.txt");
      restart_file->write(self_ent.get(), restart_out);
      restart_out.close();
      std::unique_ptr<EntropyTerm> self_ent2 =
          panacea_pi.create(panacea_settings);

      ifstream restart_in;
      restart_in.open("weighted_self_entropy_restart2.txt");
      restart_file->read(self_ent2.get(), restart_in);
      restart_in.close();
      double self_ent_val2 = self_ent2->compute(*dwrapper, panacea_settings);

      REQUIRE(self_ent_val == Approx(self_ent_val2));
    }
  }
}

TEST_CASE("Testing:panacea self entropy update", "[end-to-end,panacea]") {

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
  std::vector<std::vector<double>> data = {{0.0, 0.0}, {10.0, 10.0}};
  int rows = 2;
  const int cols = 2;

  auto dwrapper = panacea_pi.wrap(&(data), rows, cols);
  std::unique_ptr<EntropyTerm> self_ent =
      panacea_pi.create(*dwrapper, panacea_settings);

  double self_ent_two_pts = self_ent->compute(*dwrapper, panacea_settings);

  // Let's try shrinking the data to a single point
  data.resize(1);

  rows = 1;
  // We have to rewrap the data
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(*dwrapper);

  double self_ent_single_pt = self_ent->compute(*dwrapper, panacea_settings);

  std::cout << "self entropy should decrease when we move from two points "
               "spread apart";
  std::cout << " to a single point." << std::endl;
  std::cout << "Two points " << self_ent_two_pts << " single point "
            << self_ent_single_pt << std::endl;
  REQUIRE(self_ent_single_pt < self_ent_two_pts);

  // Increase the size and diversity the self entropy should increase
  data.emplace_back(10.0, 10.0);
  data.emplace_back(20.0, 20.0);
  rows = 3;
  dwrapper = panacea_pi.wrap(&(data), rows, cols);
  self_ent->update(*dwrapper);
  double self_ent_three_pts = self_ent->compute(*dwrapper, panacea_settings);
  std::cout << "self entropy should increase when we move from two points "
               "spread apart";
  std::cout << " to three poitns spread apart." << std::endl;
  std::cout << "Two points " << self_ent_two_pts << " three points "
            << self_ent_three_pts << std::endl;
  REQUIRE(self_ent_two_pts < self_ent_three_pts);
}
