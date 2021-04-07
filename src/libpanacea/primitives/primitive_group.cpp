
// Local private includes
#include "primitive_group.hpp"

#include "primitive_attributes.hpp"

namespace panacea {

  PrimitiveAttributes PrimitiveGroup::createPrimitiveAttributes() noexcept {
    return PrimitiveAttributes {
    .normalizer = this->normalizer,
    .kernel_wrapper = this->kernel_wrapper,
    .covariance = this->covariance.get(),
    .reduced_covariance = this->reduced_covariance.get(),
    .reduced_inv_covariance = this->reduced_inv_covariance.get()};
  }
}
