
#ifndef PANACEA_PRIVATE_CROSSENTROPY_H
#define PANACEA_PRIVATE_CROSSENTROPY_H
#pragma once

// Local private PANACEA includes
#include "entropy_term.hpp"

#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <any>
#include <memory>

namespace panacea {

  class BaseDescriptorWrapper;
  class Distribution;
  class EntropyFactory;

  class CrossEntropy : public EntropyTerm {
      std::unique_ptr<Distribution> distribution_;
    public:
      CrossEntropy(
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
#endif // PANACEA_PRIVATE_CROSSENTROPY_H
