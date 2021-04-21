#ifndef PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

namespace panacea {

  namespace settings {
    enum class DistributionType;
    enum class EquationSetting;
 //   enum class GradSetting;
  }

  //struct DistributionGradSettings {
   // settings::GradSetting grad_settings = settings::GradSetting::WRTKernel;
  //};

  struct DistributionSettings {
    //DistributionGradSettings dist_grad_settings;
    settings::EquationSetting equation_settings;
    virtual settings::DistributionType type() const noexcept = 0;   
    virtual ~DistributionSettings() = 0;
  };

}

#endif // PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
