
#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#pragma once

// Local private PANACEA includes
#include "entropy_decorator.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;
  class EntropyTerm;

  class Weight : public EntropyDecorator {

      double weight_ = 1.0;
    
    public: 
      Weight(std::unique_ptr<EntropyTerm> entropy_term, const double & weight) : EntropyDecorator(std::move(entropy_term)), weight_(weight) {};

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
  };

}

#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
