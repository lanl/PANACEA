
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

// Public local PANACEA includes
#include "panacea/vector.hpp"

// Standard includes
#include <cmath>
#include <string>
#include <vector>

namespace panacea {

  void GaussCorrelated::reset(PrimitiveAttributes & attributes) {
    assert(attributes.kernel_wrapper != nullptr);
    attributes_ = std::move(attributes); 
    double determinant = attributes_.reduced_covariance->getDeterminant();
    if( determinant <= 0.0 ) {
      std::string error_msg = "Determinant is less than 0 value: " + std::to_string(determinant);
      PANACEA_FAIL(error_msg);
    }
    pre_factor_ = 1.0/(std::pow(determinant,0.5) * 
        std::pow(constants::PI_SQRT*constants::SQRT_2,
          static_cast<double>(attributes_.reduced_covariance->getNumberDimensions())));
  }

  double GaussCorrelated::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int descriptor_ind) const {

    assert(descriptor_wrapper != nullptr);
    assert(descriptor_ind > -1);
    assert(descriptor_ind < descriptor_wrapper->getNumberPoints() );
    assert(attributes_.kernel_wrapper != nullptr);
    assert(kernel_index_ > -1);
    assert(kernel_index_ < attributes_.kernel_wrapper->getNumberPoints());
    assert(attributes_.reduced_inv_covariance != nullptr);
    assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
    assert(attributes_.reduced_inv_covariance->is(NormalizationState::Normalized));

    auto & descs  = *(descriptor_wrapper);
    auto & kerns = *(attributes_.kernel_wrapper);
    const auto & norm_coeffs = attributes_.normalizer.getNormalizationCoeffs();
    auto & red_inv_cov = *(attributes_.reduced_inv_covariance); 
    const auto & chosen_dims = red_inv_cov.getChosenDimensionIndices();

    std::vector<double> diff;
    const int red_ndim = attributes_.reduced_inv_covariance->getNumberDimensions();
    diff.reserve(red_ndim);
    int index = 0;
    for ( const int dim : chosen_dims ) {
      diff.push_back((descs(descriptor_ind, dim) - 
          kerns(kernel_index_,dim)) *
          norm_coeffs.at(dim));
    }

    std::vector<double> MxV;
    MxV.reserve(red_ndim);
    for( int j=0; j<red_ndim; ++j) {
      double val = 0.0;
      for( int k=0; k<red_ndim; ++k) {
        val += red_inv_cov(j,k) * diff.at(k);
      }
      MxV.push_back(val);
    }

    double VxMxV = 0.0;
    for(int i=0; i<red_ndim; ++i) {
      VxMxV += diff.at(i) * MxV.at(i);
    }
    return pre_factor_ * std::exp(-0.5 * VxMxV);
  }

  std::vector<double> GaussCorrelated::compute_grad(
      const BaseDescriptorWrapper * descriptors,
      const int descriptor_ind,
      const settings::EquationSetting & prim_settings, 
      const settings::GradSetting & grad_setting) const {

    assert(descriptors != nullptr);
    assert(descriptor_ind > -1);
    assert(descriptor_ind < descriptors->getNumberPoints() );
    assert(attributes_.kernel_wrapper != nullptr);
    assert(kernel_index_ > -1);
    assert(kernel_index_ < attributes_.kernel_wrapper->getNumberPoints());
    assert(attributes_.reduced_inv_covariance != nullptr);
    assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
    assert(attributes_.reduced_inv_covariance->is(NormalizationState::Normalized));

    auto & descs  = *(descriptors);
    const auto & norm_coeffs = attributes_.normalizer.getNormalizationCoeffs();
    auto & kerns = *(attributes_.kernel_wrapper);
    const int ndim = descs.getNumberDimensions();
    auto & red_inv_cov = *(attributes_.reduced_inv_covariance); 
    const auto & chosen_dims = red_inv_cov.getChosenDimensionIndices();

    const double exp_term = [&]{
      if (prim_settings == settings::EquationSetting::IgnoreExp) {
        return 1.0;
      } else {
        return compute(descriptors, descriptor_ind);
      }
    }();

    std::vector diff(ndim, 0.0);
    for ( const int dim : chosen_dims ) {
      // ( a_i * (d_x_i - d_mu_i) )
      diff.at(dim) = (descs(descriptor_ind,dim) - 
          kerns(kernel_index_,dim));
    } 

    std::vector<double> grad(ndim,0.0);

    int index1 = 0;
    for ( const int dim : chosen_dims ) {
      int index2 = 0;
      for ( const int dim2 : chosen_dims ) { 
        grad.at(dim) += exp_term * diff.at(dim2) * norm_coeffs.at(dim) * norm_coeffs.at(dim2) * red_inv_cov(index1, index2);
        ++index2;
      }
      ++index1;
    }

    if ( grad_setting == settings::GradSetting::WRTDescriptor ) {
      for ( const int & dim : chosen_dims ) {
        grad.at(dim) *= -1.0;
      }
    }

    return grad;
  }

}

