
#ifndef PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
#define PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
#pragma once

// Private local includes
#include "primitive.hpp"

// Standard includes
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class PrimitiveAttributes;

  class GaussUncorrelated : public Primitive {
    public:

      virtual void reset(const & PrimitiveAttributes) const final;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int sample_ind,
          const int kernel_ind) const final;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptors,
          const int descriptor_ind,
          const int kernel_ind,
          const int point_target) const final; 

    private:
      PrimitiveAttributes attributes_;
      double pre_factor_;
  };
}

#endif // PANACEA_PRIVATE_GAUSSIAN_UNCORRELATED_H
