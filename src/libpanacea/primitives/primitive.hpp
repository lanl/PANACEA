
#ifndef PANACEA_PRIVATE_PRIMITIVE_H
#define PANACEA_PRIVATE_PRIMITIVE_H
#pragma once

// Local PANACEA includes
#include "private_settings.hpp"

// Standard includes
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class PrimitiveAttributes;

  class Primitive {

    public:

      virtual const settings::KernelPrimitive type() const noexcept = 0;
      virtual const settings::KernelCorrelation correlation() const noexcept = 0;

      virtual int getId() const noexcept = 0;

      /*
       * Updating a primitive inolves updating the kernel pointer
       * reset the inverse covariance matrix
       * reset the prefactor value
       */
      // Do not make const reference
      virtual void update(PrimitiveAttributes &&) = 0;

      /*
       * Computes the density
       */
      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int sample_ind,
          const settings::EquationSetting & prim_settings = settings::EquationSetting::None
          ) const = 0;

      /*
       * Computes the gradient of the density
       *
       * descriptor_ind - the index of the descriptor value we are interested in calculating the gradient at
       * kernel_ind - the index of the kernel
       * point_ind - the point the gradient is being taken with respect too, (not needed because it must
       * either be the descriptor or the kernel otherwise the gradient is 0.0)
       */
      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptors,
          const int descriptor_ind,
          const settings::EquationSetting & prim_settings,
          const settings::GradSetting & grad_setting) const = 0;

      virtual ~Primitive() = 0;
  };

}

#endif // PANACEA_PRIVATE_PRIMITIVE_H

