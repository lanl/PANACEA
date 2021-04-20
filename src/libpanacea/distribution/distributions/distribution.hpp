#ifndef PANACEA_PRIVATE_BASEDISTRIBUTION_H
#define PANACEA_PRIVATE_BASEDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

// Standard includes
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class DistributionSettings;

  class Distribution {

    public:
      virtual settings::DistributionType type() const noexcept = 0; 

      virtual double compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) = 0;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const DistributionSettings & distribution_settings) = 0;

      virtual ~Distribution() = 0;
  };
}

#endif // PANACEA_PRIVATE_BASEDISTRIBUTION_H

