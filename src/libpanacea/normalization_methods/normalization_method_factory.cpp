
// Local private includes
#include "normalization_method_factory.hpp"

#include "error.hpp"
#include "normalization_method_base.hpp"
#include "mean.hpp"
#include "median.hpp"
#include "passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  namespace {
    /**
     * It can be problematic to use data points that are stacked directly on top of 
     * one another when using the variance as the normalization coefficients. This
     * method checks that the data is of good quality, if not it will spit out a 
     * warning. 
     **/
    static bool checkDataQuality(const BaseDescriptorWrapper * desc_wrapper) {

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
          return true;
        }
      }

      // Return true in both cases, because we do not want to crash just send out
      // an warning message if assertions are on. 
      return true;
    }
    std::vector<double> normalization_method_variance(
        const BaseDescriptorWrapper * desc_wrapper){

      assert(checkDataQuality(desc_wrapper)); 
      Variance variance;
      return variance.calculate<BaseDescriptorWrapper *>(desc_wrapper);
    }

    std::vector<double> normalization_method_none(
        const BaseDescriptorWrapper * desc_wrapper){
      std::vector<double> coeffs(desc_wrapper->getNumberReducedDimensions(), 1.0);
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

  NormalizationMethod NormalizationMethodFactory::create(
          const settings::KernelNormalization & norm_method) const {

    if( normalization_methods_.count(norm_method) == 0) {
      std::string error_msg = "Normalization Method is not supported.";
      PANACEA_FAIL(error_msg);
    }
   
    return normalization_methods_[norm_method];
  }
}
