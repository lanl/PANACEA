
#ifndef PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#define PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#pragma once

// Local private includes
#include "attribute_manipulators/normalizer.hpp"

namespace panacea {

  class BaseKernelWrapper;
  class Covariance;
  class ReducedCovariance;
  class ReducedInvCovariance;

  /*
   * Class provides a generic means of passing in common attributes needed by the primitives
   */
  class PrimitiveAttributes {
    public:
      Normalizer * normalizer = nullptr; // Cannot be a copy
      BaseKernelWrapper * kernel_wrapper = nullptr;
      Covariance * covariance = nullptr;
      ReducedCovariance * reduced_covariance = nullptr;
      ReducedInvCovariance * reduced_inv_covariance = nullptr;
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
