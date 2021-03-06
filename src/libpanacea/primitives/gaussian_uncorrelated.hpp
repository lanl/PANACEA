
#ifndef PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
#define PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
#pragma once

// Private local includes
#include "primitive.hpp"

#include "attributes/reduced_covariance.hpp"
#include "constants.hpp"
#include "primitive_attributes.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

namespace panacea {

class BaseDescriptorWrapper;
class PrimitiveAttributes;
class PrimitiveFactory;

namespace settings {
enum class EquationSetting;
enum class GradSetting;
} // namespace settings

namespace test {
class Test;
}
class GaussUncorrelated : public Primitive {
private:
  const int kernel_index_ = -1;
  PrimitiveAttributes attributes_;
  double pre_factor_ = 0.0;

public:
  GaussUncorrelated(const PassKey<PrimitiveFactory> &, const int &kernel_index)
      : kernel_index_(kernel_index){};

  GaussUncorrelated(const PassKey<test::Test> &, const int &kernel_index)
      : kernel_index_(kernel_index){};

  GaussUncorrelated(const PassKey<PrimitiveFactory> &,
                    PrimitiveAttributes prim_att, const int &kernel_index)
      : kernel_index_(kernel_index), attributes_(std::move(prim_att)),
        pre_factor_(
            1.0 /
            (std::pow(attributes_.reduced_covariance->getDeterminant(), 0.5) *
             std::pow(
                 constants::PI_SQRT * constants::SQRT_2,
                 static_cast<double>(
                     attributes_.reduced_covariance->getNumberDimensions())))){

        };

  virtual const settings::KernelPrimitive type() const noexcept final;
  virtual const settings::KernelCorrelation correlation() const noexcept final;

  virtual int getId() const noexcept final { return kernel_index_; }

  virtual double getPreFactor() const noexcept final { return pre_factor_; }

  // Do not make const reference
  virtual void update(PrimitiveAttributes &&) final;

  virtual double
  compute(const BaseDescriptorWrapper &descriptor_wrapper, const int sample_ind,
          const settings::EquationSetting &prim_settings) const final;

  /*
   * Compute the gradient of the primitive
   *
   * provide:
   *
   * 1. descriptors - the descriptors
   * 2. descriptor_ind - the descriptor index calculating the gradient at
   * with respect too ( not needed should either be the point or the descriptor
   *otherwise the gradient is 0.0)
   * 5. EquationSetting - determines if things should be added or removed from
   *gradient
   * 6. GradSetting - whether the gradient is with respect to the descriptors or
   *kernels or both
   **/
  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper &descriptors,
               const int descriptor_ind,
               const settings::EquationSetting &prim_settings,
               const settings::GradSetting &grad_setting) const final;

  static std::unique_ptr<Primitive> create(const PassKey<PrimitiveFactory> &key,
                                           PrimitiveAttributes prim_att,
                                           const int &kernel_index);
};

inline std::unique_ptr<Primitive>
GaussUncorrelated::create(const PassKey<PrimitiveFactory> &key,
                          PrimitiveAttributes prim_att,
                          const int &kernel_index) {
  return std::make_unique<GaussUncorrelated>(key, prim_att, kernel_index);
}
} // namespace panacea

#endif // PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
