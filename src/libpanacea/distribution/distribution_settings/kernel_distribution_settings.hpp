#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "distribution_settings.hpp"

#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"

// Standard includes
#include <any>

namespace panacea {

struct KernelDistributionSettings : public DistributionSettings {
  // Why not put this member in Distribution Settings, because for something
  // like a histogram it may not make sense to have equation settings, that is
  // why we have the set method
  settings::EquationSetting eq_settings = settings::EquationSetting::None;
  virtual void set(std::any val) final;
  virtual settings::DistributionType type() const noexcept final;
  KernelSpecification dist_settings;
};
} // namespace panacea

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
