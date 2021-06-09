#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTION_GRADIANT_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTION_GRADIANT_H
#pragma once

// Local private PANACEA includes
#include "private_settings.hpp"

// Standard includes
#include <unordered_map>
#include <vector>

namespace panacea {

class BaseDescriptorWrapper;
class KernelDistributionSettings;
class PrimitiveGroup;

class KernelDistributionGradiant {
public:
  using GradiantMethod = std::vector<double> (*)(
      const BaseDescriptorWrapper *descriptor_wrapper,
      const int &descriptor_index, const int &grad_index,
      const PrimitiveGroup &prim_grp,
      const KernelDistributionSettings &distribution_settings,
      const double pre_factor);

  static std::unordered_map<
      settings::GradSetting,
      std::unordered_map<
          settings::EquationSetting,
          std::unordered_map<settings::KernelCount, GradiantMethod>>>
      grad_method;

  KernelDistributionGradiant();
};

} // namespace panacea

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTION_GRADIANT_H
