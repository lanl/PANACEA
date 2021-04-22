#ifndef PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

namespace panacea {

  struct DistributionSettings {
    settings::EquationSetting equation_settings = settings::EquationSetting::None;
    virtual settings::DistributionType type() const noexcept = 0;   
    virtual ~DistributionSettings() = 0;
  };

}

#endif // PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
