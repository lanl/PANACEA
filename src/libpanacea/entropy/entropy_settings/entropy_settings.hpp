#ifndef PANACEA_PRIVATE_ENTROPYSETTINGS_H
#define PANACEA_PRIVATE_ENTROPYSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "private_settings.hpp"

// Standard includes
#include <memory>

namespace panacea {

  enum class Method {
    Compute,
    ComputeGradiant,
    Create
  };

  class EntropySettings {
    private:
      std::unique_ptr<DistributionSettings> dist_settings = nullptr;

    public:
    EntropySettings() = default;
    EntropySettings(
        const PANACEASettings &,
        const settings::Memory = settings::Memory::SelfOwnIfRestartCrossOwn );
    settings::EntropyType type = settings::EntropyType::Self;
    settings::Memory memory_policy = settings::Memory::SelfOwnIfRestartCrossOwn;
    std::optional<double> weight;
    std::optional<bool> numerical_grad_switch;
    std::optional<double> numerical_grad_inc;

    settings::EquationSetting compute_equation_settings = settings::EquationSetting::None;
    settings::EquationSetting grad_equation_settings = settings::EquationSetting::None;

    /**
     * Return settings specefic to the method to be used
     * E.g. if the distribution settings are to be passed into a compute method
     * vs a compute_grad method there me be some adjustments that should be made
     **/
    void setDistributionSettings(std::unique_ptr<DistributionSettings> dist_settings);
    const DistributionSettings & getDistributionSettings(const Method) const;
  };

}

#endif // PANACEA_PRIVATE_ENTROPYSETTINGS_H
