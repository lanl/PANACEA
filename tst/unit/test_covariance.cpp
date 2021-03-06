
// Local private includes
#include "attributes/covariance.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "io/file_io_factory.hpp"

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

/**
 * Jupyter notebooks depecting the logic behind the update calls can be found
 * in PANACEA/jupyter_notebooks/Covariance
 **/
TEST_CASE("Testing:covariance test trivial constructor", "[unit,panacea]") {

  GIVEN("A descriptor wrapper consisting of three points.") {
    // 3 points 2 dimensions
    std::vector<std::vector<double>> data{{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};

    DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(&data, 3, 2);

    WHEN("Creating a correlated covariance matrix.") {
      auto cov_ptr =
          Covariance::create(dwrapper, settings::KernelCorrelation::Correlated);

      Covariance &cov = *cov_ptr;

      REQUIRE(cov.rows() == 2);
      REQUIRE(cov.cols() == 2);

      REQUIRE(cov.getMean(0) == Approx(2.0));
      REQUIRE(cov.getMean(1) == Approx(5.0));

      REQUIRE(cov.getCummulativeDescPoints() == 3);

      REQUIRE(cov(0, 0) == Approx(1.0));
      REQUIRE(cov(0, 1) == Approx(1.0));
      REQUIRE(cov(1, 0) == Approx(1.0));
      REQUIRE(cov(1, 1) == Approx(1.0));

      REQUIRE(cov.getDeterminant() == Approx(0.0));
    }
    WHEN("Creating an uncorrelated covariance matrix.") {
      auto cov_ptr = Covariance::create(
          dwrapper, settings::KernelCorrelation::Uncorrelated);

      Covariance &cov = *cov_ptr;

      REQUIRE(cov.rows() == 2);
      REQUIRE(cov.cols() == 2);

      REQUIRE(cov.getMean(0) == Approx(2.0));
      REQUIRE(cov.getMean(1) == Approx(5.0));

      REQUIRE(cov.getCummulativeDescPoints() == 3);

      // Off diagonal matrix elements should be 0.0 because
      // it is not correlated
      REQUIRE(cov(0, 0) == Approx(1.0));
      REQUIRE(cov(0, 1) == Approx(0.0));
      REQUIRE(cov(1, 0) == Approx(0.0));
      REQUIRE(cov(1, 1) == Approx(1.0));

      // Determinant should also be 0.0
      REQUIRE(cov.getDeterminant() == Approx(0.0));
    }
  }
}

TEST_CASE("Testing:covariance non-trivial", "[unit,panacea]") {

  GIVEN("A non trivial set of points to create a descriptor wrapper.") {
    std::vector<std::vector<double>> data{
        {0.6787, 0.6948, 0.7094}, {0.7577, 0.3171, 0.7547},
        {0.7431, 0.9502, 0.2760}, {0.3922, 0.0344, 0.6797},
        {0.6555, 0.4387, 0.6551}, {0.1712, 0.3816, 0.1626}};

    DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(
        &data, data.size(), data.at(0).size());

    auto cov_ptr =
        Covariance::create(dwrapper, settings::KernelCorrelation::Correlated);

    auto &cov = *cov_ptr;

    WHEN("Testing after initialization") {
      const int number_of_dimensions = 3;
      REQUIRE(cov.rows() == number_of_dimensions);
      REQUIRE(cov.cols() == number_of_dimensions);

      REQUIRE(cov.getMean(0) == Approx(0.5664).margin(1e-3));
      REQUIRE(cov.getMean(1) == Approx(0.4695).margin(1e-3));
      REQUIRE(cov.getMean(2) == Approx(0.5396).margin(1e-3));

      REQUIRE(cov.getCummulativeDescPoints() == data.size());

      REQUIRE(cov(0, 0) == Approx(0.055).margin(1e-3));
      REQUIRE(cov(0, 1) == Approx(0.0378).margin(1e-3));
      REQUIRE(cov(0, 2) == Approx(0.0297).margin(1e-3));
      REQUIRE(cov(1, 0) == Approx(0.0378).margin(1e-3));
      REQUIRE(cov(1, 1) == Approx(0.1006).margin(1e-3));
      REQUIRE(cov(1, 2) == Approx(-0.0305).margin(1e-3));
      REQUIRE(cov(2, 0) == Approx(0.0297).margin(1e-3));
      REQUIRE(cov(2, 1) == Approx(-0.0305).margin(1e-3));
      REQUIRE(cov(2, 2) == Approx(0.0639).margin(1e-3));
    }

    std::vector<std::vector<double>> update_data{{0.0318, 0.7952, 0.4984},
                                                 {0.2769, 0.1869, 0.9597},
                                                 {0.0462, 0.4898, 0.3404},
                                                 {0.0971, 0.4456, 0.5853},
                                                 {0.8235, 0.6463, 0.2238}};

    DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper2(
        &update_data, update_data.size(), update_data.at(0).size());

    cov.update(dwrapper2);

    WHEN("Testing after update") {
      const int number_of_dimensions = 3;
      REQUIRE(cov.rows() == number_of_dimensions);
      REQUIRE(cov.cols() == number_of_dimensions);

      REQUIRE(cov.getMean(0) == Approx(0.4249).margin(1e-3));
      REQUIRE(cov.getMean(1) == Approx(0.4891).margin(1e-3));
      REQUIRE(cov.getMean(2) == Approx(0.5314).margin(1e-3));

      REQUIRE(cov.getCummulativeDescPoints() ==
              (data.size() + update_data.size()));

      REQUIRE(cov(0, 0) == Approx(0.0981).margin(1e-3));
      REQUIRE(cov(0, 1) == Approx(0.0173).margin(1e-3));
      REQUIRE(cov(0, 2) == Approx(0.0037).margin(1e-3));
      REQUIRE(cov(1, 0) == Approx(0.0173).margin(1e-3));
      REQUIRE(cov(1, 1) == Approx(0.0717).margin(1e-3));
      REQUIRE(cov(1, 2) == Approx(-0.0344).margin(1e-3));
      REQUIRE(cov(2, 0) == Approx(0.0037).margin(1e-3));
      REQUIRE(cov(2, 1) == Approx(-0.0344).margin(1e-3));
      REQUIRE(cov(2, 2) == Approx(0.0639).margin(1e-3));
    }
  }
}

TEST_CASE("Testing:covariance stacked data", "[unit,panacea]") {
  std::vector<std::vector<double>> data{{2.3, 1.3, 4.9}, {2.3, 1.3, 4.9},
                                        {2.3, 1.3, 4.9}, {2.3, 1.3, 4.9},
                                        {2.3, 1.3, 4.9}, {2.3, 1.3, 4.9}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(
      &data, data.size(), data.at(0).size());

  CHECK_THROWS(Covariance::create(dwrapper,
                                  settings::KernelCorrelation::Correlated,
                                  CovarianceOption::Strict));

  Covariance::create(dwrapper, settings::KernelCorrelation::Correlated,
                     CovarianceOption::Flexible);
}

TEST_CASE("Testing:covariance read & write", "[unit,panacea]") {
  std::vector<std::vector<double>> data{{7.3, 1.9, 4.9}, {0.3, 3.2, 1.8},
                                        {2.9, 4.3, 9.2}, {2.3, 1.8, 8.9},
                                        {1.2, 1.3, 4.1}, {0.3, 3.3, 5.9}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(
      &data, data.size(), data.at(0).size());

  auto cov_ptr =
      Covariance::create(dwrapper, settings::KernelCorrelation::Correlated,
                         CovarianceOption::Flexible);

  auto &cov = *cov_ptr;

  REQUIRE(cov.getCummulativeDescPoints() == 6);
  REQUIRE(cov.getNormalizationState() == NormalizationState::Unnormalized);
  std::fstream fs;
  fs.open("test_covariance.restart", std::fstream::out);
  auto data_out = cov.write(settings::FileType::TXTRestart, fs, &cov);
  fs.close();

  auto cov_ptr2 = Covariance::create(settings::KernelCorrelation::Correlated);

  auto &cov2 = *cov_ptr2;

  // The internal matrix and vectors will not be read in from this call
  std::fstream fs2;
  fs2.open("test_covariance.restart", std::fstream::in);
  auto data_out2 = cov.read(settings::FileType::TXTRestart, fs2, &cov2);
  fs2.close();

  REQUIRE(cov2.getCummulativeDescPoints() == 6);
  REQUIRE(cov2.getNormalizationState() == NormalizationState::Unnormalized);
}
