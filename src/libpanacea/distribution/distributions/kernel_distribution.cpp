
// Local private PANACEA includes
#include "kernel_distribution.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "primitives/primitive.hpp"
#include "primitives/gaussian_uncorrelated.hpp"
#include "primitives/gaussian_correlated.hpp"
#include "primitives/primitive_factory.hpp"
#include "primitives/primitive_group.hpp"
#include "settings.hpp"

// Standard includes
#include <iostream>
#include <string>
#include <vector>

namespace panacea {

  KernelDistribution::KernelDistribution(const PassKey<DistributionFactory> &,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          const KernelSpecification & settings,
          std::string name) {

    PrimitiveFactory::registerPrimitive<GaussUncorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Uncorrelated>();

    PrimitiveFactory::registerPrimitive<GaussCorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Correlated>();

    PrimitiveFactory prim_factory;

    prim_grp_ = prim_factory.create(
        descriptor_wrapper,
        mem_manager,
        settings,
        name);
  
    pre_factor_ = 1.0/static_cast<double>(prim_grp_.primitives.size());

  }

  settings::DistributionType KernelDistribution::type() const noexcept {
    return settings::DistributionType::Kernel;
  }

  double KernelDistribution::compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) {
    // Cycle through the primitives and compute the density
    double density = 0.0;
    for( auto & prim_ptr : prim_grp_.primitives ) {
      density += prim_ptr->compute(descriptor_wrapper, desc_ind);
    }
    return pre_factor_ * density;
  }

  std::vector<double> KernelDistribution::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const DistributionSettings & distribution_settings
          ) {

    assert(descriptor_wrapper->getNumberDimensions() == prim_grp_.kernel_wrapper->getNumberDimensions());

    std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
    for( auto & prim_ptr : prim_grp_.primitives ) {

      std::vector<double> grad_temp = prim_ptr->compute_grad(
          descriptor_wrapper,
          desc_ind,
          distribution_settings.dist_grad_settings.equation_settings,
          distribution_settings.dist_grad_settings.grad_settings);

      std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());
    }

    std::transform(grad.begin(), grad.end(), grad.begin(),
               std::bind(std::multiplies<double>(), std::placeholders::_1, pre_factor_)); 

    return grad;
  }
}
