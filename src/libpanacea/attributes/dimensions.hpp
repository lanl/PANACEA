#ifndef PANACEA_PRIVATE_DIMENSIONS_H
#define PANACEA_PRIVATE_DIMENSIONS_H
#pragma once

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <vector>

namespace panacea {

  enum class DimensionsState {
    Randomized,      // The total number is fixed but the order is random
    RandomizedCount, // Only the number of descriptors is random
    FullyRandomized, // Count and order of descriptors is random
    Ordered          // Descriptor dimensions appear in sequential order
  };

  class DimensionLimiter;
  class Randomizer;

  class Dimensions {

      DimensionsState state_ = DimensionsState::Ordered;
      std::vector<int> descriptor_dimensions_;
    public:
      /**
       * Specificy the number of dimensions that can be used.
       **/
      explicit Dimensions(const int num_dimensions);
      explicit Dimensions(const std::vector<int> & dimensions);

      std::vector<int>::const_iterator begin() const noexcept;
      std::vector<int>::const_iterator end() const noexcept;

      int at(int index) const { return descriptor_dimensions_.at(index); }

      /**
       * Changes to the dimensions class are limited to
       * the:
       * Randomizer
       **/
      void set(PassKey<Randomizer>, DimensionsState state);

      const DimensionsState state() const noexcept { return state_; }

      std::size_t size() const noexcept { return descriptor_dimensions_.size(); }

      std::vector<int> & get(PassKey<Randomizer>);
      std::vector<int> & get(PassKey<DimensionLimiter>);

      /**
       * Returns a copy of the dimensions as a vector of ints
       **/
      std::vector<int> convert() const noexcept { return descriptor_dimensions_; }
  };
} // panacea


#endif // PANACEA_PRIVATE_DIMENSIONS_H
