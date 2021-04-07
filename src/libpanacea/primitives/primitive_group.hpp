
#ifndef PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#define PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "primitive.hpp"
#include "primitive_attributes.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class BaseKernelWrapper;
  class Normalizer;

  class PrimitiveGroup {
    public:
      Normalizer * normalizer = nullptr;
      BaseKernelWrapper * kernel_wrapper = nullptr;
      std::unique_ptr<Covariance> covariance = nullptr;
      std::unique_ptr<ReducedCovariance> reduced_covariance = nullptr;
      std::unique_ptr<ReducedInvCovariance> reduced_inv_covariance = nullptr;
      std::vector<std::unique_ptr<Primitive>> primitives;

      PrimitiveAttributes createPrimitiveAttributes() noexcept;
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVE_GROUP_H
