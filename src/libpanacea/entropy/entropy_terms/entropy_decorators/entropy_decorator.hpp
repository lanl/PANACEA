

#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_H
#pragma once

// Public PANACEA include
#include "panacea/entropy_term.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;

  class EntropyDecorator : public EntropyTerm {

    protected:
      std::unique_ptr<EntropyTerm> entropy_term_;

    public:
      explicit EntropyDecorator(std::unique_ptr<EntropyTerm> entropy_term) :
        entropy_term_(std::move(entropy_term)) {};

      virtual EntropyTerm::ReadFunction getReadFunction(const PassKey<EntropyTerm> &) override {
        return entropy_term_->getReadFunction(key);
      }

      virtual EntropyTerm::WriteFunction getWriteFunction(const PassKey<EntropyTerm> &) const override {
        return entropy_term_->getWriteFunction(key);
      }

      virtual EntropyTerm::State state() const noexcept override {
        return entropy_term_->state();
      }

      virtual settings::EntropyType type() const noexcept override {
        return entropy_term_->type();
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper) override {
        return entropy_term_->compute(descriptor_wrapper);
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind) override {
        return entropy_term_->compute(descriptor_wrapper, desc_ind);
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const EntropySettings & entropy_settings
          ) override {
        return entropy_term_->compute(descriptor_wrapper, entropy_settings);
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings
          ) override {
        return entropy_term_->compute(descriptor_wrapper, desc_ind, entropy_settings);
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const PANACEASettings & panacea_settings
          ) override {
        return entropy_term_->compute(descriptor_wrapper, panacea_settings);
      }

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const PANACEASettings & panacea_settings
          ) override {
        return entropy_term_->compute(descriptor_wrapper, desc_ind, panacea_settings);
      }

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind) override {
        return entropy_term_->compute_grad(descriptor_wrapper, desc_ind);
      }

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override {
        return entropy_term_->compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
      }

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const PANACEASettings & panacea_settings) override;

      virtual void set(const settings::EntropyOption option, std::any val) override {
        return entropy_term_->set(option, val);
      }

      virtual const std::vector<int> getDimensions() const noexcept override {
        return entropy_term_->getDimensions();
      }

      virtual void update(const BaseDescriptorWrapper & descriptor_wrapper) override {
        return entropy_term_->update(descriptor_wrapper);
      }

      virtual void initialize(const BaseDescriptorWrapper & descriptor_wrapper) override {
        return entropy_term_->initialize(descriptor_wrapper);
      }
  };

}
#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_H
