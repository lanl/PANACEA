
// Public PANACeA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Private local PANACEA includes
#include "gaussian_uncorrelated.hpp"

#include "attribute_manipulators/normalizer.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "primitive_attributes.hpp"
#include "private_settings.hpp"

// Standard includes
#include <cmath>
#include <iostream>
#include <vector>

namespace panacea {

void GaussUncorrelated::update(PrimitiveAttributes &&attributes) {
  assert(attributes.normalizer->getNormalizationCoeffs().size() > 0);
  assert(attributes.kernel_wrapper != nullptr);
  attributes_ = std::move(attributes);
  double determinant = attributes_.reduced_covariance->getDeterminant();
  std::cout << "Determinant is " << determinant << std::endl;
  std::cout << "Reduced covar" << std::endl;
  attributes_.reduced_covariance->print();
  if (determinant <= 0.0) {
    std::string error_msg =
        "Determinant is less than 0 value: " + std::to_string(determinant);
    PANACEA_FAIL(error_msg);
  }
  pre_factor_ =
      1.0 /
      (std::pow(determinant, 0.5) *
       std::pow(constants::PI_SQRT * constants::SQRT_2,
                static_cast<double>(
                    attributes_.reduced_covariance->getNumberDimensions())));
  std::cout << "Number of dimensions "
            << attributes_.reduced_covariance->getNumberDimensions()
            << std::endl;
}

const settings::KernelPrimitive GaussUncorrelated::type() const noexcept {
  return settings::KernelPrimitive::Gaussian;
}

const settings::KernelCorrelation GaussUncorrelated::correlation() const
    noexcept {
  return settings::KernelCorrelation::Uncorrelated;
}

double GaussUncorrelated::compute(
    const BaseDescriptorWrapper &descriptor_wrapper, const int descriptor_ind,
    const settings::EquationSetting &prim_settings) const {

  assert(descriptor_ind > -1);
  assert(descriptor_ind < descriptor_wrapper.getNumberPoints());
  assert(attributes_.kernel_wrapper != nullptr);
  assert(kernel_index_ > -1);
  assert(kernel_index_ < attributes_.kernel_wrapper->rows());
  assert(attributes_.reduced_inv_covariance != nullptr);
  assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
  assert(attributes_.normalizer != nullptr && "Normalizer is a nullptr");

  if (prim_settings == settings::EquationSetting::IgnoreExpAndPrefactor) {
    return 1.0;
  }
  const std::vector<double> norm_coeffs =
      attributes_.normalizer->getNormalizationCoeffs();
  const int num_dims =
      attributes_.reduced_inv_covariance->getNumberDimensions();
  assert(norm_coeffs.size() >= num_dims);

  double exponent = 0.0;
  int index = 0;
  for (const int dim :
       attributes_.reduced_inv_covariance->getChosenDimensionIndices()) {

    double diff = (descriptor_wrapper(descriptor_ind, dim) -
                   attributes_.kernel_wrapper->at(kernel_index_, dim)) /
                  norm_coeffs.at(dim);
    exponent += diff * diff *
                attributes_.reduced_inv_covariance->operator()(index, index);
    ++index;
  }
  exponent *= -0.5;

  double result = pre_factor_ * std::exp(exponent);
  if (result == 0.0) {
    return std::numeric_limits<double>::min();
  }
  return result;
}

std::vector<double> GaussUncorrelated::compute_grad(
    const BaseDescriptorWrapper &descriptors, const int descriptor_ind,
    const settings::EquationSetting &prim_settings,
    const settings::GradSetting &grad_setting) const {

  assert(attributes_.kernel_wrapper != nullptr);
  assert(attributes_.reduced_inv_covariance != nullptr);
  assert(grad_setting != settings::GradSetting::WRTBoth &&
         "Terms will cancel should avoid calling grad method at all");
  assert(attributes_.normalizer != nullptr && "Normalizer is a nullptr");
  const auto &norm_coeffs = attributes_.normalizer->getNormalizationCoeffs();
  const double exp_term = compute(descriptors, descriptor_ind, prim_settings);

  std::vector<double> grad(descriptors.getNumberDimensions(), 0.0);

  const auto &chosen_dims =
      attributes_.reduced_inv_covariance->getChosenDimensionIndices();
  // const auto & norm_coeffs = attributes_.normalizer.getNormalizationCoeffs();

  int index = 0;
  for (const int &dim : chosen_dims) {
    const double diff = (descriptors(descriptor_ind, dim) -
                         attributes_.kernel_wrapper->at(kernel_index_, dim)) /
                        (norm_coeffs.at(dim) * norm_coeffs.at(dim));
    grad.at(dim) =
        diff * attributes_.reduced_inv_covariance->operator()(index, index) *
        exp_term;
    ++index;
  }

  if (grad_setting == settings::GradSetting::WRTDescriptor) {
    // Multiply the gradient by negative 1
    for (const int &dim : chosen_dims) {
      grad.at(dim) *= -1.0;
    }
  }

  return grad;
}

} // namespace panacea
