
// Local private PANACEA includes
#include "normalization_method_variance.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "variance.hpp"

// Standard includes
#include <iostream>
#include <vector>

namespace panacea {

  /**
   * It can be problematic to use data points that are stacked directly on top of 
   * one another when using the variance as the normalization coefficients. This
   * method checks that the data is of good quality, if not it will spit out a 
   * warning. 
   **/
  static bool checkDataQuality(BaseDescriptorWrapper * desc_wrapper) {

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


 std::vector<double> NormalizationMethodVariance::generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const {
   
   assert(checkDataQuality(desc_wrapper)); 
   Variance variance;
   return variance.calculate<BaseDescriptorWrapper *>(desc_wrapper);
  }

  const settings::KernelNormalization NormalizationMethodVariance::type = 
        settings::KernelNormalization::Variance;
}
