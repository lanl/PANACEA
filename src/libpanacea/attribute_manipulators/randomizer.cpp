
// Local private PANACEA includes
#include "randomizer.hpp"

#include "attributes/dimensions.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <algorithm>
#include <random>

namespace panacea {

  void Randomizer::randomize(
      Dimensions & dimensions,
      const settings::RandomizeDimensions randomize_dims,
      const settings::RandomizeNumberDimensions randomize_num_dims) const {

    auto & dimension_indices = dimensions.get(PassKey<Randomizer>());

    if( randomize_dims == settings::RandomizeDimensions::Yes) {
      auto rng = std::default_random_engine {};
      std::shuffle(dimension_indices.begin(), dimension_indices.end(), rng);
      dimensions.set(PassKey<Randomizer>(), DimensionsState::Randomized);
    }

    if( randomize_num_dims == settings::RandomizeNumberDimensions::Yes ) {
      if ( dimension_indices.size() > 1 ) {
        dimension_indices.resize(1 + std::rand() % dimension_indices.size() );
      }

      if( dimensions.state() == DimensionsState::Randomized ) {
        dimensions.set(PassKey<Randomizer>(), DimensionsState::FullyRandomized);
      } else {
        dimensions.set(PassKey<Randomizer>(), DimensionsState::RandomizedCount);
      }
    }

  }
}

