
#ifndef PANACEA_PRIVATE_CROSSENTROPY_H
#define PANACEA_PRIVATE_CROSSENTROPY_H
#pragma once

// Local private PANACEA includes
#include "distribution/distributions/distribution.hpp"
#include "settings.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class SelfEntropy : public EntropyTerm {
      std::unique_ptr<Distribution> distribution_;
    public:

      explicit SelfEntropy(std::unique_ptr<distribution> dist) : 
        distribution_(std::move(distribution)) {};

      virtual settings::EntropyType type() const noexcept final;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) final;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) final;

  };

}
#endif // PANACEA_PRIVATE_SELFENTROPY_H
