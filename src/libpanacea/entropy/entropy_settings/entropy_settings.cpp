
// Local private PANACEA includes
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "entropy_settings.hpp"
#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"

using namespace panacea::settings;

namespace panacea {

  EntropySettings::EntropySettings(const PANACEASettings & in, const Memory memory_policy) {
    if(auto val = in.get<EntropyType>() ) {
      this->type = *val;
    }
    if(auto val = in.getWeight() ) {
      this->weight = *val;
    }
    if(auto val = in.numericalGrad() ) {
      this->numerical_grad_switch = *val;
      // Only matters if calculating numerical gradiants
      if(auto val2 = in.getIncrement() ) {
        this->numerical_grad_inc = *val2;
      }
    }
    if(auto dist_type = in.get<DistributionType>() ) {
      if( dist_type == DistributionType::Kernel ){
        auto kern_dist_settings = std::make_unique<KernelDistributionSettings>();

        if( auto val = in.get<KernelCorrelation>() ) {
          kern_dist_settings->dist_settings.set(*val);
        }
        if( auto val = in.get<KernelCount>() ) {
          kern_dist_settings->dist_settings.set(*val);
        }
        if(  auto val = in.get<KernelPrimitive>() ) {
          kern_dist_settings->dist_settings.set(*val);
        }
        if( auto val = in.get<KernelNormalization>() ) {
          kern_dist_settings->dist_settings.set(*val);
        }
        if( auto val = in.get<KernelCenterCalculation>() ) {
          kern_dist_settings->dist_settings.set(*val);
        }
        if( auto val = in.get<PANACEAAlgorithm>() ){
          if( *val == PANACEAAlgorithm::Flexible ) {
            kern_dist_settings->dist_settings.set(KernelAlgorithm::Flexible);
          } else {
            kern_dist_settings->dist_settings.set(KernelAlgorithm::Strict);
          }
        }
        if( this->type == EntropyType::Self ) {
          // Make the kernels share the data from the descriptors if it is a
          // Self entropy term, the only time this switches to ownership is if
          // data is loaded from a restart file.
          kern_dist_settings->dist_settings.set(KernelMemory::OwnIfRestart);
        } else {
          kern_dist_settings->dist_settings.set(KernelMemory::Own);
        }
        this->dist_settings = std::move(kern_dist_settings);
      }
    }
  }
}
