
// Private local PANACEA includes
#include "gaussian_uncorrelated.hpp"

#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "error.hpp"
#include "primitive_attributes.hpp"

// Standard includes
#include <cmath>
#include <iostream>
#include <vector>

namespace panacea {

    void GaussUncorrelated::reset(const & PrimitiveAttributes attributes) const {
      attributes_ = attributes; 
      double determinant = attributes_.reduced_covariance.getDeterminant();
      if( determinant <= 0.0 ) {
        std::cout << "Determinant value: " << determinant << std::endl;
        PANACEA_FAIL("Determinant is less than 0");
      }
      pre_factor_ = 1.0/(std::pow(determinant),0.5) * 
        std::pow(constants::PI_SQRT*constants::SQRT_2,
            static_cast<double>(attributes_.reduced_number_dimensions));
    }

      double GaussUncorrelated::compute(
          const DescriptorWrapper & descriptor_wrapper,
          const int sample_ind,
          const int kernel_ind) const {


        double exponent = 0.0;   

        int index = 0;
        for ( const int dim : descriptor_wrapper.getReducedDimensions() ) {
          double diff = descriptor_wrapper(sample_ind, dim) - kernel_wrapper(kernel_ind,dim);
          exponent += diff * diff * reduced_inv_covariance(index, index);
          ++index;
        }
        exponent *= -0.5;
        return pre_factor_ * std::exp(exponent);
      }

      std::vector<double> GaussUncorrelated::compute_grad(
          const DescriptorWrapper & descriptors,
          const int descriptor_ind,
          const int kernel_ind,
          const int point_target) const {

        std::vector<double> grad;
        grad.reserve(descriptors.getNumberReducedDimensions());
       
        // How to check if the descriptor and kernel data are 
        // equivalent, when you don't have kernels you can compare 
        return grad;
      }

  };
}

