
// Local public PANACEA includes
#include "dimensions.hpp"

// Standard includes
#include <algorithm>
#include <vector>

namespace panacea {

  Dimensions::Dimensions(const int max_num_dimensions) {
    std::iota(descriptor_dimensions_.begin(), descriptor_dimensions_.end(), 1);
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
