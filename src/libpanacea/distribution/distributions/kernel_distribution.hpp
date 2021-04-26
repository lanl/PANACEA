#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "distribution.hpp"

#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "kernel_distribution/kernel_distribution_gradiant.hpp"
#include "memory.hpp"
#include "passkey.hpp"
#include "primitives/primitive_group.hpp"

// Standard includes
#include <any>
#include <memory>
#include <string>
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
      KernelDistributionGradiant kern_dist_grad;
      // For KDE 1/N value, where N is the number Kernels/primitives
      double pre_factor_;
    public:
      KernelDistribution(const PassKey<DistributionFactory> &,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          const KernelSpecification & settings,
          std::string name);

      virtual settings::DistributionType type() const noexcept final;

      virtual double compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) final;

      /**
       * Keep in mind the default grad_setting is inherited from distribution base class.
       **/
      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const int grad_ind,
          const DistributionSettings & distribution_settings,
          std::any grad_setting) final;

      static std::unique_ptr<Distribution> create(
          const PassKey<DistributionFactory> &,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          DistributionSettings * settings,
          std::string name = "");
  };

  inline std::unique_ptr<Distribution> KernelDistribution::create(
      const PassKey<DistributionFactory> & key, 
      BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      DistributionSettings * settings,
      std::string name) {

    assert(settings->type() == settings::DistributionType::Kernel);

    KernelDistributionSettings * kern_dist_settings = static_cast<KernelDistributionSettings *>(settings);
    // The any must be the KernelSpecifications object
    return std::make_unique<KernelDistribution>(
        key,
        descriptor_wrapper,
        mem_manager,
        kern_dist_settings->dist_settings,
        name);
  }
}

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTION_H
