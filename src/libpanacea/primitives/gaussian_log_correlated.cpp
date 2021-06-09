
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Private local PANACEA includes
#include "gaussian_log_correlated.hpp"

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

void GaussLogCorrelated::update(PrimitiveAttributes &&attributes) {
  assert(attributes.kernel_wrapper != nullptr);
  std::cout << "WARNING Multivariate Log normal distribution/Gaussian Log "
               "primitive has not yet been vetted."
            << std::endl;
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

const settings::KernelPrimitive GaussLogCorrelated::type() const noexcept {
  return settings::KernelPrimitive::GaussianLog;
}

const settings::KernelCorrelation GaussLogCorrelated::correlation() const
    noexcept {
  return settings::KernelCorrelation::Correlated;
}

double
GaussLogCorrelated::compute(const BaseDescriptorWrapper *descriptor_wrapper,
                            const int descriptor_ind) const {

  assert(descriptor_wrapper != nullptr);
  assert(descriptor_ind > -1);
  assert(descriptor_ind < descriptor_wrapper->getNumberPoints());
  assert(attributes_.kernel_wrapper != nullptr);
  assert(kernel_index_ > -1);
  assert(kernel_index_ < attributes_.kernel_wrapper->rows());
  assert(attributes_.reduced_inv_covariance != nullptr);
  assert(attributes_.reduced_inv_covariance->getNumberDimensions() > 0);
  assert(
      attributes_.reduced_inv_covariance->is(NormalizationState::Normalized));
  std::cout << "WARNING Multivariate Log normal distribution/Gaussian Log "
               "primitive has not yet been vetted."
            << std::endl;

  auto &descs = *(descriptor_wrapper);
  auto &kerns = *(attributes_.kernel_wrapper);
  const auto &norm_coeffs = attributes_.normalizer.getNormalizationCoeffs();
  auto &red_inv_cov = *(attributes_.reduced_inv_covariance);
  const auto &chosen_dims = red_inv_cov.getChosenDimensionIndices();

  std::vector<double> diff;
  const int red_ndim =
      attributes_.reduced_inv_covariance->getNumberDimensions();
  diff.reserve(red_ndim);
  int index = 0;
  for (const int dim : chosen_dims) {
    diff.push_back(
        std::log(descs(descriptor_ind, dim) * norm_coeffs.at(dim)) -
        std::log(kerns.at(kernel_index_, dim) * norm_coeffs.at(dim)));
  }

  std::vector<double> MxV;
  MxV.reserve(red_ndim);
  for (int j = 0; j < red_ndim; ++j) {
    double val = 0.0;
    for (int k = 0; k < red_ndim; ++k) {
      val += red_inv_cov(j, k) * diff.at(k);
    }
    MxV.push_back(val);
  }

  double VxMxV = 0.0;
  for (int i = 0; i < red_ndim; ++i) {
    VxMxV += diff.at(i) * MxV.at(i);
  }
  double result = pre_factor_ * std::exp(-0.5 * VxMxV);
  if (result == 0.0) {
    return std::numeric_limits<double>::min();
  }
  return result;
}

std::vector<double> GaussLogCorrelated::compute_grad(
    const BaseDescriptorWrapper *descriptors, const int descriptor_ind,
    const settings::EquationSetting &prim_settings,
    const settings::GradSetting &grad_setting) const {

  throw std::runtime_error(
      "Analytical gradiant method for Multivariate Log normal "
      "distribution/GaussLog has not yet been implemented.");

  auto &descs = *(descriptors);
  const int ndim = descs.getNumberDimensions();

  std::vector<double> grad(ndim, 0.0);
  return grad;
}

} // namespace panacea
