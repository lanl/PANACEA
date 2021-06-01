// Local private PANACEA includes
#include "entropy_decorator.hpp"

#include "entropy/entropy_settings/entropy_settings.hpp"

namespace panacea {

  std::vector<double> EntropyDecorator::compute_grad(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const PANACEASettings & panacea_settings) {
    return entropy_term_->compute_grad(
        descriptor_wrapper,
        desc_ind,
        EntropySettings(panacea_settings));
  }



}


