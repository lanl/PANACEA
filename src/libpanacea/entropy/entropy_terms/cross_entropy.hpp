
#ifndef PANACEA_PRIVATE_CROSSENTROPY_H
#define PANACEA_PRIVATE_CROSSENTROPY_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

namespace panacea {

  class CrossEntropy : public EntropyTerm {

    public:
      virtual settings::EntropyType type() const noexcept final;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) override;

      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper
          const int desc_ind) override;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

      virtual void set(const EntropyOption & option, std::any val) override;
  };

}
#endif // PANACEA_PRIVATE_CROSSENTROPY_H
