
#ifndef PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
#define PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
#pragma once

// Private local includes
#include "primitive.hpp"

#include "primitive_attributes.hpp"

// Standard includes
#include <vector>

namespace panacea {

  enum class EquationSetting;
  enum class GradSetting;
  class BaseDescriptorWrapper;
  class PrimitiveAttributes;

  class GaussUncorrelated : public Primitive {
    private:
      const int kernel_index_;
      PrimitiveAttributes attributes_; 
      double pre_factor_;
    public:
      explicit GaussCorrelated(const int & kernel_index) : kernel_index_(kernel_index) {};
      GaussCorrelated(const PrimitiveAttributes & prim_att, const int & kernel_index) : kernel_index_(kernel_index) {};

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
          std::vector<EquationSetting> prim_settings, 
          GradSetting grad_setting) const final; 

  };
}

#endif // PANACEA_PRIVATE_GAUSSIAN_CORRELATED_H
