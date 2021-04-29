
// Local private includes
#include "primitive_group.hpp"

#include "attribute_manipulators/inverter.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "primitive_attributes.hpp"
#include "primitive_factory.hpp"

// Standard includes
#include <cassert>

namespace panacea {

  PrimitiveAttributes PrimitiveGroup::createPrimitiveAttributes() noexcept {
    return PrimitiveAttributes {
    .normalizer = this->normalizer,
    .kernel_wrapper = this->kernel_wrapper.get(),
    .covariance = this->covariance.get(),
    .reduced_covariance = this->reduced_covariance.get(),
    .reduced_inv_covariance = this->reduced_inv_covariance.get()};
  }
}

