
// Local private includes
#include "attributes/covariance.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "helper.hpp"
#include "io/file_io_factory.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:covariance large non trival read & write using fileio","[integration,panacea]"){

  GIVEN("Some large non trivial descriptor wrapper."){
    test::ArrayDataNonTrivial array_data;

    DescriptorWrapper<double***> dwrapper(
        &(array_data.data),
        array_data.rows,
        array_data.cols);

    WHEN("The covariance matrix is correlated.") {
      auto cov_ptr = Covariance::create(
          dwrapper,
          settings::KernelCorrelation::Correlated,
          CovarianceOption::Flexible);

      auto & cov = *cov_ptr;

      cov.print();

      io::FileIOFactory file_io_factory;
      auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

      restart_file->write(&cov, "test_covariance_correlated_full.restart");

      auto cov_ptr2 = Covariance::create(
          settings::KernelCorrelation::Correlated
          );

      auto & cov2 = *cov_ptr2;

      restart_file->read(&cov2, "test_covariance_correlated_full.restart");

      cov2.print();

      REQUIRE(cov.rows() == cov2.rows());
      REQUIRE(cov.cols() == cov2.cols());
      REQUIRE(cov.getMean(0) == Approx(cov2.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov2.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov2.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov2.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov2(0,0)));
      REQUIRE(cov(1,0) == Approx(cov2(1,0)));
      REQUIRE(cov(2,0) == Approx(cov2(2,0)));

      REQUIRE(cov(0,1) == Approx(cov2(0,1)));
      REQUIRE(cov(1,1) == Approx(cov2(1,1)));
      REQUIRE(cov(2,1) == Approx(cov2(2,1)));

      REQUIRE(cov(0,2) == Approx(cov2(0,2)));
      REQUIRE(cov(1,2) == Approx(cov2(1,2)));
      REQUIRE(cov(2,2) == Approx(cov2(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov2.getCummulativeDescPoints());

      restart_file->write(&cov2, "test_covariance_correlated_full2.restart");

      auto cov_ptr3 = Covariance::create(
          settings::KernelCorrelation::Correlated
          );

      auto & cov3 = *cov_ptr3;

      restart_file->read(&cov3, "test_covariance_correlated_full2.restart");

      REQUIRE(cov.rows() == cov3.rows());
      REQUIRE(cov.cols() == cov3.cols());
      REQUIRE(cov.getMean(0) == Approx(cov3.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov3.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov3.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov3.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov3(0,0)));
      REQUIRE(cov(1,0) == Approx(cov3(1,0)));
      REQUIRE(cov(2,0) == Approx(cov3(2,0)));

      REQUIRE(cov(0,1) == Approx(cov3(0,1)));
      REQUIRE(cov(1,1) == Approx(cov3(1,1)));
      REQUIRE(cov(2,1) == Approx(cov3(2,1)));

      REQUIRE(cov(0,2) == Approx(cov3(0,2)));
      REQUIRE(cov(1,2) == Approx(cov3(1,2)));
      REQUIRE(cov(2,2) == Approx(cov3(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov3.getCummulativeDescPoints());
    }

    WHEN("The covariance matrix is uncorrelated.") {
      auto cov_ptr = Covariance::create(
          dwrapper,
          settings::KernelCorrelation::Uncorrelated,
          CovarianceOption::Flexible);

      auto & cov = *cov_ptr;

      cov.print();

      io::FileIOFactory file_io_factory;
      auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

      restart_file->write(&cov, "test_covariance_uncorrelated_full.restart");

      auto cov_ptr2 = Covariance::create(
          settings::KernelCorrelation::Uncorrelated
          );

      auto & cov2 = *cov_ptr2;

      restart_file->read(&cov2, "test_covariance_uncorrelated_full.restart");

      cov2.print();

      REQUIRE(cov.rows() == cov2.rows());
      REQUIRE(cov.cols() == cov2.cols());
      REQUIRE(cov.getMean(0) == Approx(cov2.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov2.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov2.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov2.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov2(0,0)));
      REQUIRE(cov(1,0) == Approx(cov2(1,0)));
      REQUIRE(cov(2,0) == Approx(cov2(2,0)));

      REQUIRE(cov(0,1) == Approx(cov2(0,1)));
      REQUIRE(cov(1,1) == Approx(cov2(1,1)));
      REQUIRE(cov(2,1) == Approx(cov2(2,1)));

      REQUIRE(cov(0,2) == Approx(cov2(0,2)));
      REQUIRE(cov(1,2) == Approx(cov2(1,2)));
      REQUIRE(cov(2,2) == Approx(cov2(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov2.getCummulativeDescPoints());
    }

  }
}

TEST_CASE("Testing:covariance read & write using fileio","[integration,panacea]"){

  GIVEN("Some small non trivial descriptor wrapper."){
    std::vector<std::vector<double>> data {
      {7.3,  1.9,  4.9},
      {0.3,  3.2,  1.8},
      {2.9,  4.3,  9.2},
      {2.3,  1.8,  8.9},
      {1.2,  1.3,  4.1},
      {0.3,  3.3,  5.9}};

    DescriptorWrapper<std::vector<std::vector<double>>*>
      dwrapper(&data, data.size(), data.at(0).size());

    WHEN("The covariance matrix is correlated.") {
      auto cov_ptr = Covariance::create(
          dwrapper,
          settings::KernelCorrelation::Correlated,
          CovarianceOption::Flexible);

      auto & cov = *cov_ptr;

      cov.print();

      io::FileIOFactory file_io_factory;
      auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

      restart_file->write(&cov, "test_covariance_correlated_full.restart");

      auto cov_ptr2 = Covariance::create(
          settings::KernelCorrelation::Correlated
          );

      auto & cov2 = *cov_ptr2;

      restart_file->read(&cov2, "test_covariance_correlated_full.restart");

      cov2.print();

      REQUIRE(cov.rows() == cov2.rows());
      REQUIRE(cov.cols() == cov2.cols());
      REQUIRE(cov.getMean(0) == Approx(cov2.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov2.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov2.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov2.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov2(0,0)));
      REQUIRE(cov(1,0) == Approx(cov2(1,0)));
      REQUIRE(cov(2,0) == Approx(cov2(2,0)));

      REQUIRE(cov(0,1) == Approx(cov2(0,1)));
      REQUIRE(cov(1,1) == Approx(cov2(1,1)));
      REQUIRE(cov(2,1) == Approx(cov2(2,1)));

      REQUIRE(cov(0,2) == Approx(cov2(0,2)));
      REQUIRE(cov(1,2) == Approx(cov2(1,2)));
      REQUIRE(cov(2,2) == Approx(cov2(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov2.getCummulativeDescPoints());

      restart_file->write(&cov2, "test_covariance_correlated_full2.restart");

      auto cov_ptr3 = Covariance::create(
          settings::KernelCorrelation::Correlated
          );

      auto & cov3 = *cov_ptr3;

      restart_file->read(&cov3, "test_covariance_correlated_full2.restart");

      REQUIRE(cov.rows() == cov3.rows());
      REQUIRE(cov.cols() == cov3.cols());
      REQUIRE(cov.getMean(0) == Approx(cov3.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov3.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov3.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov3.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov3(0,0)));
      REQUIRE(cov(1,0) == Approx(cov3(1,0)));
      REQUIRE(cov(2,0) == Approx(cov3(2,0)));

      REQUIRE(cov(0,1) == Approx(cov3(0,1)));
      REQUIRE(cov(1,1) == Approx(cov3(1,1)));
      REQUIRE(cov(2,1) == Approx(cov3(2,1)));

      REQUIRE(cov(0,2) == Approx(cov3(0,2)));
      REQUIRE(cov(1,2) == Approx(cov3(1,2)));
      REQUIRE(cov(2,2) == Approx(cov3(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov3.getCummulativeDescPoints());
    }

    WHEN("The covariance matrix is uncorrelated.") {
      auto cov_ptr = Covariance::create(
          dwrapper,
          settings::KernelCorrelation::Uncorrelated,
          CovarianceOption::Flexible);

      auto & cov = *cov_ptr;

      cov.print();

      io::FileIOFactory file_io_factory;
      auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

      restart_file->write(&cov, "test_covariance_uncorrelated_full.restart");

      auto cov_ptr2 = Covariance::create(
          settings::KernelCorrelation::Uncorrelated
          );

      auto & cov2 = *cov_ptr2;

      restart_file->read(&cov2, "test_covariance_uncorrelated_full.restart");

      cov2.print();

      REQUIRE(cov.rows() == cov2.rows());
      REQUIRE(cov.cols() == cov2.cols());
      REQUIRE(cov.getMean(0) == Approx(cov2.getMean(0)));
      REQUIRE(cov.getMean(1) == Approx(cov2.getMean(1)));
      REQUIRE(cov.getMean(2) == Approx(cov2.getMean(2)));
      REQUIRE(cov.getNormalizationState() == cov2.getNormalizationState());
      REQUIRE(cov(0,0) == Approx(cov2(0,0)));
      REQUIRE(cov(1,0) == Approx(cov2(1,0)));
      REQUIRE(cov(2,0) == Approx(cov2(2,0)));

      REQUIRE(cov(0,1) == Approx(cov2(0,1)));
      REQUIRE(cov(1,1) == Approx(cov2(1,1)));
      REQUIRE(cov(2,1) == Approx(cov2(2,1)));

      REQUIRE(cov(0,2) == Approx(cov2(0,2)));
      REQUIRE(cov(1,2) == Approx(cov2(1,2)));
      REQUIRE(cov(2,2) == Approx(cov2(2,2)));

      REQUIRE(cov.getCummulativeDescPoints() == cov2.getCummulativeDescPoints());
    }

  }
}
