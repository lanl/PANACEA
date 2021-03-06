#ifndef PANACEA_PRIVATE_RANDOMIZER_H
#define PANACEA_PRIVATE_RANDOMIZER_H
#pragma once

// Public PANACEA includes
#include "panacea/settings.hpp"

namespace panacea {

class Dimensions;

class Randomizer {
public:
  Randomizer() = default;

  /**
   * Designed to randomize dimensions
   **/
  void randomize(Dimensions &dimensions, const settings::RandomizeDimensions,
                 const settings::RandomizeNumberDimensions) const;
};
} // namespace panacea

#endif // PANACEA_PRIVATE_RANDOMIZER_H
