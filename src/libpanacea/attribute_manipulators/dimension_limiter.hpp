#ifndef PANACEA_PRIVATE_DIMENSION_LIMITER_H
#define PANACEA_PRIVATE_DIMENSION_LIMITER_H
#pragma once

// Public PANACEA includes
#include "panacea/settings.hpp"

namespace panacea {

class Dimensions;

class DimensionLimiter {
public:
  DimensionLimiter() = default;

  /**
   * Designed to randomize dimensions
   **/
  void limit(Dimensions &dimensions, const int dim_limit) const;
};
} // namespace panacea

#endif // PANACEA_PRIVATE_DIMENSION_LIMITER_H
