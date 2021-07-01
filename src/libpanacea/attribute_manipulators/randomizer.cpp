
// Local private PANACEA includes
#include "randomizer.hpp"

#include "attributes/dimensions.hpp"
#include "passkey.hpp"

// Standard includes
#include <random>

namespace panacea {

  void Randomizer::randomize(Dimensions & dimensions) const {
    auto & dimension_indices = dimensions.get(PassKey<Randomizer>());
    std::random_shuffle(dimension_indices.begin(), dimension_indices.end());
    dimensions.set(PassKey<Randomizer>(), DimensionsState::Randomized);
  }
}

