
#ifndef PANACEA_PRIVATE_SELFENTROPY_H
#define PANACEA_PRIVATE_SELFENTROPY_H
#pragma once

// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "distribution/distributions/distribution.hpp"
#include "passkey.hpp"
#include "private_settings.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class EntropyFactory;

  class SelfEntropy : public EntropyTerm {
      std::unique_ptr<Distribution> distribution_;
    public:
      SelfEntropy(
          const PassKey<EntropyFactory> & key, 
          std::unique_ptr<Distribution> dist) :
        distribution_(std::move(dist)) {};

      virtual settings::EntropyType type() const noexcept final;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) override;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind) override;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

      virtual void set(const settings::EntropyOption & option, std::any val) override;

      virtual const std::vector<int> & getDimensions() const noexcept override;

      virtual void update(const BaseDescriptorWrapper * descriptor_wrapper) override;

      static std::unique_ptr<EntropyTerm> create(
          const PassKey<EntropyFactory> & key,
          const BaseDescriptorWrapper * descriptor_wrapper,
          EntropySettings * settings);

  };

}
#endif // PANACEA_PRIVATE_SELFENTROPY_H
