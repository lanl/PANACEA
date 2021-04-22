
#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
#pragma once

namespace panacea {

  class NumericalGrad : public EntropyDecorator {

    public: 
      NumericalGrad(EntropyTerm entropy_term) : EntropyDecorator(entropy_term) {};

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

  };

}

#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_NUMERICALGRAD_H
