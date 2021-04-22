

#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_H
#pragma once

// Local private PANACEA include
#include "entropy/entropy_terms/entropy_term.hpp"

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

      virtual settings::EntropyType type() const noexcept override {
        return entropy_term_->type();
      }

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) override {
        return entropy_term_->compute(descriptor_wrapper);
      } 

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind) override {
        return entropy_term_->compute(descriptor_wrapper, desc_ind);
      }

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override {
        return entropy_term_->compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
      }

      virtual void set(const settings::EntropyOption & option, std::any val) override {
        return entropy_term_->set(option, val);
      }
  };

}
#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_H