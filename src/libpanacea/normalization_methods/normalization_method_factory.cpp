// Local private includes
#include "normalization_method_factory.hpp"

#include "attributes/covariance.hpp"
#include "error.hpp"
#include "mean.hpp"
#include "median.hpp"
#include "private_settings.hpp"
#include "variance.hpp"

// Local public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  namespace {

    static std::vector<int> findStackedDimensions(const BaseDescriptorWrapper * desc_wrapper) {
    
      std::vector<int> stacked_dims;
      for( int dim = 0; dim < desc_wrapper->getNumberDimensions(); ++dim ) {
        const double init_pt_val = desc_wrapper->operator()(0,dim);
        double diff = 0.0;
        for( int pt = 1; pt < desc_wrapper->getNumberPoints(); ++pt ) {
          diff += init_pt_val -  desc_wrapper->operator()(pt,dim);
        }
        // if the diff in any of the dimensions is 0.0 it is problematic for counting 
        // the variance in that dimension, because all the points in that dimension are
        // stacked on top of each other. 
        if( diff == 0.0 ) {
          stacked_dims.push_back(dim);
        }
      }
      return stacked_dims;
    }

    /**
     * It can be problematic to use data points that are stacked directly on top of 
     * one another when using the variance as the normalization coefficients. This
     * method checks that the data is of good quality, if not it will spit out a 
     * warning. 
     **/
    static bool printDataQualityMessage1() {
      std::string warning = "WARNING you are attempting to calculate normalization ";
      warning += "coefficients using the variance of your data but your data is not ";
      warning += "well dispersed. Your data set is not ideal because you have points";
      warning += " in at least one dimension that are stacked on top of one another ";
      warning += "meaning the variance is 0.0, PANACEA accounts for this by setting ";
      warning += "these particular coefficients to 1.0, however this is not an optimal";
      warning += " solution because it will make it hard for minimizers to optimize ";
      warning += "your configurations it would be better to have data that is more ";
      warning += "varied.";
      std::cout << warning << std::endl;
      // Return true in both cases, because we do not want to crash just send out
      // an warning message if assertions are on. 
      return true;
    }

    static bool printDataQualityMessage2() {
      std::string warning = "WARNING you are attempting to calculate normalization ";
      warning += "coefficients using the variance of your data but you have only ";
      warning += "provided a single point to calculate the variance, which means the ";
      warning += "variance is essentially 0.0. As a work around PANACEA will simply ";
      warning += "assume the variance is 1.0.";
      std::cout << warning << std::endl;
      // Return true in both cases, because we do not want to crash just send out
      // an warning message if assertions are on. 
      return true;
    }

    /**
     * Note that using this with the covariance matrix assumes the covariance 
     * matrix already up to date with the latest set of descriptor dimensions.
     *
     * The actual coefficients returned are not the variance but the sqrt of the
     * variance. The sqrt is required in order to appropriate calculate the 
     * off diagonal normalization coefficients of the covariance matrix.
     **/
    std::vector<double> normalization_method_variance(
        const BaseDescriptorWrapper * desc_wrapper,
        std::any extra_args){

      if(extra_args.type() != typeid(settings::None) && 
         extra_args.type() != typeid(Covariance *)) {
        std::string error_msg = "Unrecognized types encountered as extra arguments to ";
        error_msg += "variance normalization method.";
        PANACEA_FAIL(error_msg);
      }

      // Check to make sure there is more than one row in the desc wrapper
      // if that's the case then there is no effective way of calculating the
      // variance so we will assume it is one everywhere
      if( extra_args.type() == typeid(settings::None) ) {
        std::vector<int> stacked_dims = findStackedDimensions(desc_wrapper);
        if(stacked_dims.size() != 0) {
          assert(printDataQualityMessage1());
        }
        if(desc_wrapper->rows() == 1) {
          assert(printDataQualityMessage2());
          return std::vector<double>(desc_wrapper->getNumberDimensions(), 1.0);
        }  
        Variance variance;
        auto vec_variance = variance.calculate<const BaseDescriptorWrapper *>(desc_wrapper);
        for( const auto & dim : stacked_dims ) {
          vec_variance.at(dim) = 1.0;
        } 

        // Really we want to return the sqrt of the variance
        for( auto & elem : vec_variance ) {
          elem = std::sqrt(elem);
        }
        return vec_variance;
      }

      Covariance * cov = std::any_cast<Covariance *>(extra_args); 
      assert(cov != nullptr);
      assert(cov->rows() == desc_wrapper->getNumberDimensions() );
      assert(cov->getNormalizationState() == NormalizationState::Unnormalized);
      std::vector<double> cov_diagonal;
      cov_diagonal.reserve(desc_wrapper->getNumberDimensions());
      for( int row = 0; row < cov->rows(); ++row) {
        assert(cov->operator()(row,row)>0.0);
        cov_diagonal.emplace_back(std::sqrt(cov->operator()(row,row)));
      }
      return cov_diagonal;
    }

    std::vector<double> normalization_method_none(
        const BaseDescriptorWrapper * desc_wrapper,
        std::any){

      std::vector<double> coeffs(desc_wrapper->getNumberDimensions(), 1.0);
      return coeffs;
    }
  }

  std::unordered_map<settings::KernelNormalization,
    NormalizationMethodFactory::NormalizationMethod>
      NormalizationMethodFactory::normalization_methods_;

  NormalizationMethodFactory::NormalizationMethodFactory() {
    registerNormalizationMethod<normalization_method_none,
      settings::KernelNormalization::None>();
    registerNormalizationMethod<normalization_method_variance,
      settings::KernelNormalization::Variance>();
  }

  NormalizationMethodFactory::NormalizationMethod NormalizationMethodFactory::create(
          const settings::KernelNormalization & norm_method) const {

    if( normalization_methods_.count(norm_method) == 0) {
      std::string error_msg = "Normalization Method is not supported.";
      PANACEA_FAIL(error_msg);
    }
   
    return normalization_methods_[norm_method];
  }
}
