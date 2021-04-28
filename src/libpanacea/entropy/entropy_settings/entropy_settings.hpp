#ifndef PANACEA_PRIVATE_ENTROPYSETTINGS_H
#define PANACEA_PRIVATE_ENTROPYSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "private_settings.hpp"

// Standard includes
#include <memory>

namespace panacea {

  struct EntropySettings {
    EntropySettings() = default;
    explicit EntropySettings(const PANACEASettings &);
    settings::EntropyType type = settings::EntropyType::Self;
    std::optional<double> weight;
    std::optional<bool> numerical_grad_switch;
    std::optional<double> numerical_grad_inc;
    std::unique_ptr<DistributionSettings> dist_settings = nullptr;
  };

}

#endif // PANACEA_PRIVATE_ENTROPYSETTINGS_H
