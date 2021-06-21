// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "covariance.hpp"

#include "covariance/covariance_uncorrelated.hpp"
#include "covariance/covariance_correlated.hpp"
#include "error.hpp"
#include "type_map.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

namespace panacea {

  Covariance::~Covariance() {};

  std::vector<std::any> Covariance::write(
      const settings::FileType file_type,
      std::ostream & os,
      std::any cov_instance) {

    std::vector<std::any> nested_objs;

    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      if( std::type_index(cov_instance.type()) == std::type_index(typeid(Covariance *))){
        os << "[Covariance]\n";
        Covariance * cov = std::any_cast<Covariance *>(cov_instance);
        os << cov->correlation() << "\n";
        if( cov->correlation() == settings::KernelCorrelation::Correlated ) {
          nested_objs = CovarianceCorrelated::write(file_type, os, cov);
        }else if( cov->correlation() == settings::KernelCorrelation::Uncorrelated ) {
          nested_objs = CovarianceUncorrelated::write(file_type, os, cov);
        }
      } else {
        std::string error_msg = "Unrecognized covariance matrix type.\n";
        PANACEA_FAIL(error_msg);
      }

    } else {
      std::string error_msg = "Covariance matrix cannot be written to specified file type ";
      error_msg += "not supported.";
      PANACEA_FAIL(error_msg);
    }
    return nested_objs;
  }

  io::ReadInstantiateVector Covariance::read(
      const settings::FileType file_type,
      std::istream & is,
      std::any cov_instance) {

    std::cout << __FILE__ <<":" << __LINE__ << std::endl;
    io::ReadInstantiateVector nested_objs;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      std::string line;
    std::cout << __FILE__ <<":" << __LINE__ << std::endl;
      // Find header tag
      while(line.find("[Covariance]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Covariance] header while trying ";
          error_msg += "to read restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

    std::cout << __FILE__ <<":" << __LINE__ << std::endl;
      // Determine the covariance matrix type
      while(line.find("[Covariance]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Covariance] header while trying ";
          error_msg += "to read restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      settings::KernelCorrelation correlation;
      is >> correlation;

      std::cout << __FILE__ <<":" << __LINE__ << std::endl;
      Covariance * cov;
      if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(Covariance *))){

        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        cov = std::any_cast<Covariance *>(cov_instance);
      } else if(std::type_index(cov_instance.type()) ==
          std::type_index(typeid(std::unique_ptr<Covariance> *))) {
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        cov = (std::any_cast<std::unique_ptr<Covariance> *>(cov_instance))->get();
      } else {
        std::string error_msg = "Unrecognized covariance matrix.\n";
        if(type_map.count(std::type_index(cov_instance.type()))){
          error_msg += "Type identified as: " + type_map.at(std::type_index(cov_instance.type())) + "\n";
        }
        PANACEA_FAIL(error_msg);
      }


      // Check that correlation specified in restart file is consistent with the
      // correlation of the covariance matrix passed into this function
      if(correlation != cov->correlation()) {
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        std::string error_msg = "Correlation matrix store in memory is not ";
        error_msg += "consistent with matrix read in from restart file.";
        PANACEA_FAIL(error_msg);
      }

      if( cov->correlation() == settings::KernelCorrelation::Correlated){
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        nested_objs = CovarianceCorrelated::read(file_type, is, cov);
      }else if( cov->correlation() == settings::KernelCorrelation::Uncorrelated){
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        nested_objs = CovarianceUncorrelated::read(file_type, is, cov);
      } else {
        std::cout << __FILE__ <<":" << __LINE__ << std::endl;
        std::string error_msg = "Unrecognized covariance matrix type, ";
        error_msg += "recognized types that can be written and read are:\n";
        error_msg += "CovarianceCorrelated *\nCovarianceUncorrelated *\n";
        PANACEA_FAIL(error_msg);
      }
      std::cout << __FILE__ <<":" << __LINE__ << std::endl;
    } else {
      std::string error_msg = "Covariance matrix cannot be read from specified file type ";
      error_msg += "not supported.";
      PANACEA_FAIL(error_msg);
    }
    return nested_objs;
  }

  std::unique_ptr<Covariance> Covariance::create(
      const BaseDescriptorWrapper & desc_wrap,
      const settings::KernelCorrelation corr,
      const settings::KernelAlgorithm opt) {

    // Converting KernelAlgorithm to CovarianceOption
    const CovarianceOption opt_cov = [](const settings::KernelAlgorithm opt){
      if(opt == settings::KernelAlgorithm::Strict) {
        return CovarianceOption::Strict;
      } else {
        return CovarianceOption::Flexible;
      }
    }(opt);

    return Covariance::create(desc_wrap, corr, opt_cov);
  }

  std::unique_ptr<Covariance> Covariance::create(
      const BaseDescriptorWrapper & desc_wrap,
      const settings::KernelCorrelation corr,
      const CovarianceOption opt) {

    if( corr == settings::KernelCorrelation::Correlated ) {
      return std::make_unique<CovarianceCorrelated>(desc_wrap, opt);
    } else if( corr == settings::KernelCorrelation::Uncorrelated) {
      return std::make_unique<CovarianceUncorrelated>(desc_wrap, opt);
    }

    std::string error_msg = "Unrecognized correlation type specified, ";
    error_msg += "cannot create correlation matrix.";
    PANACEA_FAIL(error_msg);
    return nullptr;
  }

  std::unique_ptr<Covariance> Covariance::create(
      const settings::KernelCorrelation corr) {

    if( corr == settings::KernelCorrelation::Correlated ) {
      return std::make_unique<CovarianceCorrelated>(CovarianceBuild::Allocate);
    } else if( corr == settings::KernelCorrelation::Uncorrelated) {
      return std::make_unique<CovarianceUncorrelated>(CovarianceBuild::Allocate);
    }

    std::string error_msg = "Unrecognized correlation type specified, ";
    error_msg += "cannot create correlation matrix.";
    PANACEA_FAIL(error_msg);
    return nullptr;
  }

  std::unique_ptr<Covariance> Covariance::create(
          const settings::KernelCorrelation corr,
          std::unique_ptr<Matrix> matrix,
          std::unique_ptr<Vector> mean,
          int total_num_pts,
          const CovarianceOption opt) {

    if( corr == settings::KernelCorrelation::Correlated ) {
      return std::make_unique<CovarianceCorrelated>(
          std::move(matrix),
          std::move(mean),
          total_num_pts);
    } else if( corr == settings::KernelCorrelation::Uncorrelated) {
      return std::make_unique<CovarianceUncorrelated>(
          std::move(matrix),
          std::move(mean),
          total_num_pts);
    }

    std::string error_msg = "Unrecognized correlation type specified, ";
    error_msg += "cannot create correlation matrix.";
    PANACEA_FAIL(error_msg);
    return nullptr;

  }

}
