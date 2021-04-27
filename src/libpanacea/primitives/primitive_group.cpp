
// Local private includes
#include "primitive_group.hpp"

#include "attribute_manipulators/normalizer.hpp"
#include "primitive_attributes.hpp"

// Standard includes
#include <cassert>

namespace panacea {

  void PrimitiveGroup::update(const BaseDescriptorWrapper * descriptor_wrapper) {
    assert(kernel_wrapper != nullptr); 
    kernel_wrapper->update(descriptor_wrapper);
    // Unnormalize the covariance matrix before updating 
    normalizer.unnormalize(covariance);
    covariance->update(descriptor_wrapper);
    // Now we are free to update the normalization coefficients
    normalizer.update(descriptor_wrapper);

    // Cannot update the reduced covariance matrix and reduced inv covariance matrices
    // these both need to be recalculated
    Reducer reducer;
    reduced_covariance = std::make_unique<ReducedCovariance>(
        reducer.reduce(*covariance, std::vector<int> {}));
    
    Inverter inverter;
    reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*reduced_covariance));

    // Now we need to update all the primitives
    for( auto prim : primitives) {
      prim->update( createPrimitiveAttributes() );
    }
  }

  PrimitiveAttributes PrimitiveGroup::createPrimitiveAttributes() noexcept {
    return PrimitiveAttributes {
    .normalizer = this->normalizer,
    .kernel_wrapper = this->kernel_wrapper,
    .covariance = this->covariance.get(),
    .reduced_covariance = this->reduced_covariance.get(),
    .reduced_inv_covariance = this->reduced_inv_covariance.get()};
  }
}

