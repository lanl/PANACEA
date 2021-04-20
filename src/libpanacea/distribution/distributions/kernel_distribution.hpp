#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "distribution.hpp"

#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "memory.hpp"
#include "passkey.hpp"
#include "primitives/primitive_group.hpp"

// Standard includes
#include <any>
#include <memory>
#include <vector>

namespace panacea {

  namespace settings {
    enum class DistributionType;
    enum class EquationSetting;
    enum class GradSetting;
  }

  class BaseDescriptorWrapper;
  class DistributionFactory;
  class DistributionSettings;

  class KernelDistribution : public Distribution {

    private:
      PrimitiveGroup prim_grp_;
    public:
      KernelDistribution(const PassKey<DistributionFactory> &,
          const BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          const KernelSpecification & settings) {};

      virtual settings::DistributionType type() const noexcept final;

      virtual double compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) final;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const DistributionSettings & distribution_settings) final;

      static std::unique_ptr<Distribution> create(
          const PassKey<DistributionFactory> &,
          const BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          DistributionSettings * settings);
  };

  inline std::unique_ptr<Distribution> KernelDistribution::create(
      const PassKey<DistributionFactory> & key, 
      const BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      DistributionSettings * settings) {

    assert(settings->type() == settings::DistributionType::Kernel);

    KernelDistributionSettings * kern_dist_settings = static_cast<KernelDistributionSettings *>(settings);
    // The any must be the KernelSpecifications object
    return std::make_unique<KernelDistribution>(
        key,
        descriptor_wrapper,
        mem_manager,
        kern_dist_settings->dist_settings);
  }
}

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTION_H
