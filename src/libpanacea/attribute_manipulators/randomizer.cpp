
// Local private PANACEA includes
#include "randomizer.hpp"

#include "attributes/dimensions.hpp"
#include "passkey.hpp"

// Standard includes
#include <algorithm>
#include <random>

namespace panacea {

  void Randomizer::randomize(Dimensions & dimensions) const {
    auto & dimension_indices = dimensions.get(PassKey<Randomizer>());

    auto rng = std::default_random_engine {};
    std::shuffle(dimension_indices.begin(), dimension_indices.end(), rng);
    dimensions.set(PassKey<Randomizer>(), DimensionsState::Randomized);
  }
}

