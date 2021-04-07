
// Private local PANACEA includes
#include "gaussian_uncorrelated.hpp"

#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/normalizer.hpp"
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

  void GaussUncorrelated::reset(PrimitiveAttributes & attributes) {
    assert(attributes.normalizer != nullptr);
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

  double GaussUncorrelated::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int descriptor_ind) const {

    assert(descriptor_wrapper != nullptr);
    assert(descriptor_ind > -1);
    assert(descriptor_ind < descriptor_wrapper->getNumberPoints() );
    assert(attributes_.kernel_wrapper != nullptr);
    assert(kernel_index_ > -1);
    assert(kernel_index_ < attributes_.kernel_wrapper->getNumberPoints());

    double exponent = 0.0;   
    int index = 0;
    for ( const int dim : descriptor_wrapper->getReducedDimensions() ) {
      double diff = (*descriptor_wrapper)(descriptor_ind, dim) - (*attributes_.kernel_wrapper)(kernel_index_,dim);
      exponent += diff * diff * attributes_.reduced_inv_covariance->operator()(index, index);
      ++index;
    }
    exponent *= -0.5;
    return pre_factor_ * std::exp(exponent);
  }

  std::vector<double> GaussUncorrelated::compute_grad(
      const BaseDescriptorWrapper * descriptors,
      const int descriptor_ind,
//      const int point_target, 
      const std::vector<settings::EquationSetting> & prim_settings, 
      const settings::GradSetting & grad_setting) const {

    assert(descriptors != nullptr);
    assert(attributes_.kernel_wrapper != nullptr);

      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    double exp_term;
    if (std::find(prim_settings.begin(), 
          prim_settings.end(), 
          settings::EquationSetting::IgnoreExp) != prim_settings.end()) {
      exp_term = 1.0;
    } else {
      exp_term = compute(descriptors, descriptor_ind);
    }
  
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    std::vector<double> grad(descriptors->getNumberDimensions(),0.0);

      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    const auto & chosen_dims = attributes_.reduced_inv_covariance->getChosenDimensionIndices();
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    const auto & norm_coeffs = attributes_.normalizer->getNormalizationCoeffs();

      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    int index = 0;
    for ( const int & dim : chosen_dims ) {
      
      std::cout << "Cycling dimension " << dim << std::endl;
      const double diff =  descriptors->operator()(descriptor_ind,dim) - 
        attributes_.kernel_wrapper->operator()(kernel_index_,dim);
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      std::cout << "Normalization coefficients size " << norm_coeffs.size() << std::endl;
      grad.at(dim) = (diff * norm_coeffs.at(dim)* norm_coeffs.at(dim)) * 
        attributes_.reduced_inv_covariance->operator()(index,index) * exp_term;

      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      ++index;
    }

    if ( grad_setting == settings::GradSetting::WRTKernel ) {
      // Multiply the gradient by negative 1
      for ( const int & dim : chosen_dims ) {
        grad.at(dim) *= -1.0;
      }
    }

    return grad;
  }

}

