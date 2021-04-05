
#ifndef PANACEA_PRIVATE_PRIMITIVE_H
#define PANACEA_PRIVATE_PRIMITIVE_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class PrimitiveAttributes;
  enum class EquationSetting;
  enum class GradSetting;

  class Primitive {
   
    public:
//      virtual ~Primitive() = default;

      /*
       * Reset a primitive inolves updating the kernel pointer
       * reset the inverse covariance matrix 
       * reset the prefactor value
       */
      // Do not make const reference
      virtual void reset(PrimitiveAttributes) = 0;

      /*
       * Computes the density
       */
      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int sample_ind) const = 0;

      /*
       * Computes the gradient of the density
       *
       * descriptor_ind - the index of the descriptor value we are interested in calculating the gradient at
       * kernel_ind - the index of the kernel 
       * point_ind - the point the gradient is being taken with respect too
       */
      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptors,
          const int descriptor_ind,
          const int point_target, 
          std::vector<EquationSetting> prim_settings, 
          GradSetting grad_setting) const = 0; 
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVE_H

