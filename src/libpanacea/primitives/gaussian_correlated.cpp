
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Private local PANACEA includes
#include "gaussian_correlated.hpp"

#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "primitive_attributes.hpp"
#include "private_settings.hpp"
#include "vector/vector.hpp"

// Standard includes
#include <cmath>
#include <limits>
#include <string>
#include <vector>

namespace panacea {

void GaussCorrelated::update(PrimitiveAttributes &&attributes) {
  assert(attributes.kernel_wrapper != nullptr);
  attributes_ = std::move(attributes);
  double determinant = attributes_.reduced_covariance->getDeterminant();
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
}

const settings::KernelPrimitive GaussCorrelated::type() const noexcept {
  return settings::KernelPrimitive::Gaussian;
}

const settings::KernelCorrelation GaussCorrelated::correlation() const
    noexcept {
  return settings::KernelCorrelation::Correlated;
}

double
GaussCorrelated::compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const int descriptor_ind,
                         const settings::EquationSetting &prim_settings) const {

  assert(descriptor_ind > -1);
  assert(descriptor_ind < descriptor_wrapper.getNumberPoints());
  assert(attributes_.kernel_wrapper != nullptr);
  assert(kernel_index_ > -1);
  assert(kernel_index_ < attributes_.kernel_wrapper->rows());
  assert(attributes_.reduced_inv_covariance != nullptr);
  assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
  assert(
      attributes_.reduced_inv_covariance->is(NormalizationState::Normalized));
  assert(attributes_.normalizer != nullptr && "Normalizer is a nullptr");

  if (prim_settings == settings::EquationSetting::IgnoreExpAndPrefactor) {
    return 1.0;
  }

  auto &descs = (descriptor_wrapper);
  auto &kerns = *(attributes_.kernel_wrapper);
  const std::vector<double> &norm_coeffs =
      attributes_.normalizer->getNormalizationCoeffs();
  auto &red_inv_cov = *(attributes_.reduced_inv_covariance);
  const auto &chosen_dims = red_inv_cov.getChosenDimensionIndices();

  std::vector<double> diff;
  const int red_ndim =
      attributes_.reduced_inv_covariance->getNumberDimensions();
  diff.reserve(red_ndim);
  int index = 0;
  for (const int dim : chosen_dims) {
    diff.push_back((descs(descriptor_ind, dim) - kerns.at(kernel_index_, dim)) /
                   norm_coeffs.at(dim));
    std::cout << "dim " << dim << " diff " << diff.back() << " norm_coeff "
              << norm_coeffs.at(dim) << std::endl;
  }

  std::cout << "Red Inv Cov" << std::endl;
  std::vector<double> MxV;
  MxV.reserve(red_ndim);
  for (int j = 0; j < red_ndim; ++j) {
    double val = 0.0;
    for (int k = 0; k < red_ndim; ++k) {
      std::cout << red_inv_cov(j, k) << " ";
      val += red_inv_cov(j, k) * diff.at(k);
    }
    MxV.push_back(val);
    std::cout << std::endl;
  }

  double VxMxV = 0.0;
  for (int i = 0; i < red_ndim; ++i) {
    VxMxV += diff.at(i) * MxV.at(i);
  }
  std::cout << "VxMxV " << VxMxV << std::endl;
  double result = pre_factor_ * std::exp(-0.5 * VxMxV);
  std::cout << "result " << result << " pre_factor_ " << pre_factor_
            << std::endl;
  if (result == 0.0) {
    return std::numeric_limits<double>::min();
  }
  return result;
}

std::vector<double>
GaussCorrelated::compute_grad(const BaseDescriptorWrapper &descriptors,
                              const int descriptor_ind,
                              const settings::EquationSetting &prim_settings,
                              const settings::GradSetting &grad_setting) const {

  assert(descriptor_ind > -1);
  assert(descriptor_ind < descriptors.getNumberPoints());
  assert(attributes_.kernel_wrapper != nullptr);
  assert(kernel_index_ > -1);
  assert(kernel_index_ < attributes_.kernel_wrapper->rows());
  assert(attributes_.reduced_inv_covariance != nullptr);
  assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
  assert(
      attributes_.reduced_inv_covariance->is(NormalizationState::Normalized));
  assert(attributes_.normalizer != nullptr && "Normalizer is a nullptr");

  auto &descs = (descriptors);
  const auto &norm_coeffs = attributes_.normalizer->getNormalizationCoeffs();
  auto &kerns = *(attributes_.kernel_wrapper);
  const int ndim = descs.getNumberDimensions();
  auto &red_inv_cov = *(attributes_.reduced_inv_covariance);
  const auto &chosen_dims = red_inv_cov.getChosenDimensionIndices();

  const double exp_term = compute(descriptors, descriptor_ind, prim_settings);

  std::vector diff(ndim, 0.0);
  for (const int dim : chosen_dims) {
    // ( a_i * (d_x_i - d_mu_i) )
    diff.at(dim) =
        ((descs(descriptor_ind, dim) - kerns.at(kernel_index_, dim))) /
        norm_coeffs.at(dim);
  }

  std::vector<double> grad(ndim, 0.0);

  int index1 = 0;
  for (const int dim : chosen_dims) {
    int index2 = 0;
    for (const int dim2 : chosen_dims) {
      grad.at(dim) += exp_term * diff.at(dim2) * red_inv_cov(index1, index2) *
                      1.0 / norm_coeffs.at(dim);

      ++index2;
    }
    ++index1;
  }

  if (grad_setting == settings::GradSetting::WRTDescriptor) {
    for (const int &dim : chosen_dims) {
      grad.at(dim) *= -1.0;
    }
  }

  return grad;
}

} // namespace panacea
