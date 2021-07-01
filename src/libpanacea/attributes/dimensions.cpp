
// Local public PANACEA includes
#include "dimensions.hpp"

// Standard includes
#include <cassert>
#include <numeric>
#include <vector>

namespace panacea {

  Dimensions::Dimensions(const int max_num_dimensions) {
    assert(max_num_dimensions != 0);
    assert(max_num_dimensions > -2);
    descriptor_dimensions_.resize(max_num_dimensions);
    std::iota(descriptor_dimensions_.begin(), descriptor_dimensions_.end(), 0);
  }

  std::vector<int>::const_iterator Dimensions::begin() const noexcept {
    return descriptor_dimensions_.begin();
  }

  std::vector<int>::const_iterator Dimensions::end() const noexcept {
    return descriptor_dimensions_.end();
  }

  void Dimensions::set(PassKey<Randomizer>, DimensionsState state) {
    state_ = state;
  }

  std::vector<int> & Dimensions::get(PassKey<Randomizer>) {
    return descriptor_dimensions_;
  }
} // panacea
