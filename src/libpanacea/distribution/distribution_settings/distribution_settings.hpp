#ifndef PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  namespace settings {
    enum class DistributionType;
    enum class EquationSetting;
    enum class GradSetting;
  }

  struct DistributionGradSettings {
    std::vector<settings::EquationSetting> equation_settings;
    settings::GradSetting grad_settings;
  };

  struct DistributionSettings {
    DistributionGradSettings dist_grad_settings;
    virtual settings::DistributionType type() const noexcept = 0;   
  };

}

#endif // PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
