
#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
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

  class NumericalGrad : public EntropyDecorator {

    double inc_ratio_ = 0.0001;
    bool numerical_grad_ = true;

    public: 
      explicit NumericalGrad(std::unique_ptr<EntropyTerm> entropy_term) : 
        EntropyDecorator(std::move(entropy_term)) {};

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

      virtual void set(const settings::EntropyOption & option, std::any val) override;
  };

}

#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
