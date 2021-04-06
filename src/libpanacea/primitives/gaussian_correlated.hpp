
#ifndef PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
#define PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
#pragma once

// Private local includes
#include "primitive.hpp"

#include "passkey.hpp"
#include "primitive_attributes.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  enum class EquationSetting;
  enum class GradSetting;
  class BaseDescriptorWrapper;
  class PrimitiveAttributes;
  class PrimitiveFactory;

  namespace test {
    class Test;
  }

  class GaussCorrelated : public Primitive {
    private:
      const int kernel_index_;
      PrimitiveAttributes attributes_; 
      double pre_factor_;
    public:
      GaussCorrelated(const PassKey<PrimitiveFactory> &,
          const int & kernel_index) : kernel_index_(kernel_index) {};

      GaussCorrelated(const PassKey<test::Test> &,
          const int & kernel_index) : kernel_index_(kernel_index) {};

      GaussCorrelated(const PassKey<PrimitiveFactory> &, 
          const PrimitiveAttributes & prim_att,
          const int & kernel_index) : kernel_index_(kernel_index) {};

      // Do not make const reference
      virtual void reset(PrimitiveAttributes) final;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int sample_ind) const final;

      /*
       * Compute the gradient of the primitive
       *
       * provde:
       *
       * 1. descriptors - the descriptors
       * 2. descriptor_ind - the descriptor index calculating the gradient at
       * 3. kernel_ind - the kernel index 
       * 4. point_target - the index of either the kernel or descriptor that the gradient is being taken 
       * with respect too
       * 5. EquationSetting - determines if things should be added or removed from gradient 
       * 6. GradSetting - whether the gradient is with respect to the descriptors or kernels or both
       **/ 
      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptors,
          const int descriptor_ind,
          const int point_target, 
          const std::vector<settings::EquationSetting> & prim_settings, 
          const settings::GradSetting & grad_setting) const final; 

      static std::unique_ptr<Primitive> create(
          const PassKey<PrimitiveFactory> &, 
          const PrimitiveAttributes & prim_att,
          const int & kernel_index);

  };

  inline std::unique_ptr<Primitive> GaussCorrelated::create(
          const PassKey<PrimitiveFactory> & key, 
          const PrimitiveAttributes & prim_att,
          const int & kernel_index) {
    return std::make_unique<GaussCorrelated>(key, prim_att,kernel_index);
  }
}

#endif // PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
