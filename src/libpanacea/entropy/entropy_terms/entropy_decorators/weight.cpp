
// Standard includes
#include <vector>

namespace panacea {

  double Weight::compute(
      const BaseDescriptorWrapper * descriptor_wrapper) {
    return EntropyDecorator::compute(descriptor_wrapper) * weight_;
  }

  double Weight::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind) {
    return EntropyDecorator::compute(descriptor_wrapper, desc_ind) * weight_;
  }

  std::vector<double> Weight::compute_grad(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings) {

    auto vec = EntropyDecorator::compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
    std::transform(vec.begin(), vec.end(), vec.begin(), std::bind(std::multiplies<double>(), std::placeholder::_1, weight) );
    return vec;
  }

}


