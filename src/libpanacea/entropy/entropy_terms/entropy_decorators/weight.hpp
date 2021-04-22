
#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#pragma once

namespace panacea {

  class Weight : public EntropyDecorator {

      double weight_ = 1.0;
    
    public: 
      Weight(EntropyTerm entropy_term, double weight) : EntropyDecorator(entropy_term), weight_(weight) {};

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) override {
        return EntropyDecorator::compute(descriptor_wrapper) * weight_;
      }

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind) override {
        return EntropyDecorator::compute(descriptor_wrapper, desc_ind) * weight_;
      }

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override {

        auto vec = EntropyDecorator::compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
        std::transform(vec.begin(), vec.end(), vec.begin(), std::bind(std::multiplies<double>(), std::placeholder::_1, weight) );
        return vec;
      }

  };

}

#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
