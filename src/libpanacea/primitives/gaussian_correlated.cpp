
// Private local PANACEA includes
#include "gaussian_correlated.hpp"

#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "error.hpp"
#include "primitive_attributes.hpp"
#include "settings.hpp"

// Standard includes
#include <cmath>
#include <iostream>
#include <vector>

namespace panacea {

  void GaussCorrelated::reset(PrimitiveAttributes & attributes) {
    assert(attributes.kernel_wrapper != nullptr);
    attributes_ = std::move(attributes); 
    double determinant = attributes_.reduced_covariance->getDeterminant();
    if( determinant <= 0.0 ) {
      std::cout << "Determinant value: " << determinant << std::endl;
      PANACEA_FAIL("Determinant is less than 0");
    }
    pre_factor_ = 1.0/(std::pow(determinant,0.5) * 
        std::pow(constants::PI_SQRT*constants::SQRT_2,
          static_cast<double>(attributes_.reduced_covariance->getNumberDimensions())));
  }

  double GaussCorrelated::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int sample_ind) const {

    assert(attributes_.kernel_wrapper != nullptr);

    double exponent = 0.0;   
    int index = 0;
    for ( const int dim : descriptor_wrapper->getReducedDimensions() ) {
      double diff = (*descriptor_wrapper)(sample_ind, dim) - (*attributes_.kernel_wrapper)(kernel_index_,dim);
      exponent += diff * diff * attributes_.reduced_inv_covariance->operator()(index, index);
      ++index;
    }
    exponent *= -0.5;
    return pre_factor_ * std::exp(exponent);
  }

  std::vector<double> GaussCorrelated::compute_grad(
      const BaseDescriptorWrapper * descriptors,
      const int descriptor_ind,
      const int point_target, 
      const std::vector<settings::EquationSetting> & prim_settings, 
      const settings::GradSetting & grad_setting) const {

    std::vector<double> grad;
    grad.reserve(descriptors->getNumberReducedDimensions());

    // How to check if the descriptor and kernel data are 
    // equivalent, when you don't have kernels you can compare 
    return grad;
  }

}

