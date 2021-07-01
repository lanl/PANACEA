#ifndef PANACEA_PRIVATE_DIMENSIONS_H
#define PANACEA_PRIVATE_DIMENSIONS_H
#pragma once

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <vector>

namespace panacea {

  enum class DimensionsState {
    Randomized,
    Ordered
  };

  class Dimensions {

      DimensionState state_ = DimensionState::Ordered;
      std::vector<int> descriptor_dimensions_;
    public:
      /**
       * Specificy the maximum number of dimensions that can be used.
       **/
      explicit Dimensions(const int max_num_dimensions) :
        descriptor_dimensions_(max_num_dimensions);

      std::vector<int>::const_iterator begin() const noexcept;
      std::vector<int>::const_iterator end() const noexcept;

      /**
       * Changes to the dimensions class are limited to
       * the:
       * Randomizer
       **/
      void set(PassKey<Randomizer>, DimensionsState state);

      std::size_t size() const noexcept { return descriptor_dimensions_.size(); }

      std::vector<int> & get(PassKey<Randomizer>);
  };
} // panacea


#endif // PANACEA_PRIVATE_DIMENSIONS_H
