
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

    return self_entropy;
  }

  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const EntropySettings & entropy_settings
          ) {

      assert(entropy_settings.dist_settings != nullptr);
//      std::vector<double> inv_distribution();
//      inv_distribution.reserve(descriptor_wrapper->getNumberPoints());
      
//      for( int desc_ind1 = 0; desc_ind1 < descriptor_wrapper->getNumberPoints(); ++desc_ind1 ){
//        inv_distribution.push_back( 1.0/distribution_.compute(descriptor_wrapper, desc_ind1));
//      }
      double inv_distribution = 1.0/distribution_->compute(descriptor_wrapper, desc_ind);

      // Compute the gradiant with respect to the Descriptors 
      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);

      const DistributionSettings * dist_settings = entropy_settings.dist_settings.get();
      // Compute the gradiant with respect to each of the Kernels 
      for( int grad_ind = 0; grad_ind < descriptor_wrapper->getNumberPoints(); ++grad_ind ){
        std::vector<double> grad_temp = distribution_->compute_grad(descriptor_wrapper, desc_ind, grad_ind, *dist_settings);
        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());
      }
      std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(), 
            std::placeholders::_1, 
            inv_distribution));

      return grad;
  }

  std::unique_ptr<EntropyTerm> SelfEntropy::create(
      const PassKey<EntropyFactory> & key,
      BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      EntropySettings * settings,
      std::string name) {

    name = "Self Entropy " + name;

    // Create distribution 
    DistributionFactory dist_factory;
    dist_factory.registerDistribution<
      KernelDistribution,settings::DistributionType::Kernel>();

    auto dist = dist_factory.create(descriptor_wrapper, mem_manager, settings->dist_settings.get() ,name);
    return std::make_unique<SelfEntropy>(key, std::move(dist));
  }

  void SelfEntropy::set(const EntropyOption & option, std::any val) {
    std::string error_msg = "SelfEntropy does not contain any options that can be set";
    PANACEA_FAIL(error_msg);
  }
}
