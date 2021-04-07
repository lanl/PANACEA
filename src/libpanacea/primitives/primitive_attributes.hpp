
#ifndef PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#define PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"

// Standard includes
#include <optional>

namespace panacea {
  
  class BaseKernelWrapper;
  class Normalizer;

  /*
   * Class provides a generic means of passing in common attributes needed by the primitives
   */
  class PrimitiveAttributes {
    public:
      PrimitiveAttributes() = default;
      PrimitiveAttributes(PrimitiveAttributes & prim);
      PrimitiveAttributes(PrimitiveAttributes && prim);
      PrimitiveAttributes & operator=(PrimitiveAttributes & prim);
      PrimitiveAttributes & operator=(PrimitiveAttributes && prim);
      Normalizer * normalizer = nullptr;
      BaseKernelWrapper * kernel_wrapper = nullptr;
      std::unique_ptr<Covariance> covariance = nullptr;
      std::unique_ptr<ReducedCovariance> reduced_covariance = nullptr;
      std::unique_ptr<ReducedInvCovariance> reduced_inv_covariance = nullptr;       
      /*std::optional<Covariance> covariance;
      std::optional<ReducedCovariance> reduced_covariance;
      std::optional<ReducedInvCovariance> reduced_inv_covariance;       */
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
