
#ifndef PANACEA_PRIVATE_CROSSENTROPY_H
#define PANACEA_PRIVATE_CROSSENTROPY_H
#pragma once

// Local private PANACEA includes
#include "entropy_term.hpp"

#include "distribution/distributions/distribution.hpp"
#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class EntropyFactory;
  class MemoryManager;

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

      virtual void set(const EntropyOption & option, std::any val) override;

      static std::unique_ptr<EntropyTerm> create(
          const PassKey<EntropyFactory> & key,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          EntropySettings * settings,
          std::string name);

  };

}
#endif // PANACEA_PRIVATE_SELFENTROPY_H
