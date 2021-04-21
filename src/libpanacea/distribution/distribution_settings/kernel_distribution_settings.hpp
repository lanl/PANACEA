#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "distribution_settings.hpp"

#include "kernels/kernel_specifications.hpp"
#include "settings.hpp"

namespace panacea {

  struct KernelDistributionSettings : public DistributionSettings {
    settings::GradSetting grad_settings = settings::GradSetting::WRTKernel;
    virtual settings::DistributionType type() const noexcept final;   
    KernelSpecification dist_settings;
  };

}

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTIONSETTINGS_H
