#ifndef PANACEA_PRIVATE_BASEDISTRIBUTION_H
#define PANACEA_PRIVATE_BASEDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

// Standard includes
#include <any>
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
          const int grad_ind, // The index associated with whatever we are taking the gradiant with respect to
          const DistributionSettings & distribution_settings, std::any extra_options = settings::None::None) = 0;

      virtual void update(const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      virtual ~Distribution() = 0;
  };
}

#endif // PANACEA_PRIVATE_BASEDISTRIBUTION_H

