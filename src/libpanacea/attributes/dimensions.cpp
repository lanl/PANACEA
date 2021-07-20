
// Local public PANACEA includes
#include "dimensions.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <algorithm>
#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>

namespace panacea {

Dimensions::Dimensions(const int num_dimensions) {
  assert(num_dimensions != 0);
  assert(num_dimensions > -2);
  descriptor_dimensions_.resize(num_dimensions);
  std::iota(descriptor_dimensions_.begin(), descriptor_dimensions_.end(), 0);
}

Dimensions::Dimensions(const std::vector<int> &dimensions) {

  // Check the state of the data
  auto copy_dims = dimensions;
  std::sort(copy_dims.begin(), copy_dims.end());
  for (int index = 0; index < copy_dims.size(); ++index) {
    assert(copy_dims.at(index) >= 0 && "Negative dimensions do not make sense");
    if (copy_dims.at(index) != dimensions.at(index)) {
      DimensionsState state_ = DimensionsState::Randomized;
      break;
    }
  }
  descriptor_dimensions_ = dimensions;
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

std::vector<int> &Dimensions::get(const PassKey<Randomizer> &) {
  return descriptor_dimensions_;
}

std::vector<int> &Dimensions::get(const PassKey<DimensionLimiter> &) {
  return descriptor_dimensions_;
}

void Dimensions::print() const noexcept {
  std::cout << "Dimensions" << std::endl;
  for (const auto &dim : descriptor_dimensions_) {
    std::cout << dim << " ";
  }
  std::cout << std::endl;
}
} // namespace panacea
