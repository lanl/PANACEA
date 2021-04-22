

#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_H
#pragma once

namespace panacea {

  class EntropyDecorator : public EntropyTerm {

    protected:
      EntropyTerm* entropy_term_;

    public:
      EntropyDecorator(EntropyTerm* entropy_term) : entropy_term_(entropy_term) {};

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

  };

}
#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_H
