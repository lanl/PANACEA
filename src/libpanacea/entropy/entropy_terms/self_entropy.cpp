
// Local private PANACEA includes
#include "self_entropy.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "distribution/distributions/distribution.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_factory.hpp"
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"

// Standard includes
#include <cassert>
#include <cmath>

namespace panacea {

  settings::EntropyType SelfEntropy::type() const noexcept {
    return settings::EntropyType::Self;
  } 

  double SelfEntropy::compute(const BaseDescriptorWrapper * descriptor_wrapper) {

    assert(descriptor_wrapper != nullptr);

    double self_entropy = 0.0;
    for( int desc_pt = 0; desc_pt < descriptor_wrapper->getNumberPoints(); ++desc_pt ){
      self_entropy += -1.0 * log(distribution_->compute(descriptor_wrapper, desc_pt));
    }
    return self_entropy;
  }

  double SelfEntropy::compute(const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind) {

    assert(descriptor_wrapper != nullptr);

    return -1.0 * log(distribution_->compute(descriptor_wrapper, desc_ind));
  }

  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const EntropySettings & entropy_settings
          ) {

      assert(entropy_settings.dist_settings != nullptr);

      std::vector<double> inv_distribution;
      inv_distribution.reserve(descriptor_wrapper->getNumberPoints());
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper->getNumberPoints(); ++desc_ind2 ){ 
        inv_distribution.push_back(-1.0/distribution_->compute(descriptor_wrapper, desc_ind2));
      }
      // Compute the gradiant with respect to the Descriptors 
      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);

      const DistributionSettings * dist_settings = entropy_settings.dist_settings.get();
      // Compute the gradiant with respect to each of the Kernels 
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper->getNumberPoints(); ++desc_ind2 ){
        std::vector<double> grad_temp = distribution_->compute_grad(descriptor_wrapper, desc_ind2, desc_ind, *dist_settings);

        std::transform(grad_temp.begin(), grad_temp.end(), grad_temp.begin(),
          std::bind(std::multiplies<double>(), 
            std::placeholders::_1, 
            inv_distribution.at(desc_ind2)));

        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      }
      return grad;
  }

  std::unique_ptr<EntropyTerm> SelfEntropy::create(
      const PassKey<EntropyFactory> & key,
      const BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      EntropySettings * settings,
      std::string name) {

    name = "Self Entropy " + name;
    // Create distribution 
    DistributionFactory dist_factory;
//    dist_factory.registerDistribution<
//      KernelDistribution,settings::DistributionType::Kernel>();

    auto dist = dist_factory.create(descriptor_wrapper, mem_manager, settings->dist_settings.get() ,name);
    return std::make_unique<SelfEntropy>(key, std::move(dist));
  }

  void SelfEntropy::set(const settings::EntropyOption & option, std::any val) {
    std::string error_msg = "SelfEntropy does not contain any options that can be set";
    PANACEA_FAIL(error_msg);
  }

  void SelfEntropy::update(const BaseDescriptorWrapper * descriptor_wrapper) {
    distribution_->update(descriptor_wrapper);
  }
}
