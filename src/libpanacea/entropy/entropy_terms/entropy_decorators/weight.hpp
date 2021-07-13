
#ifndef PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#define PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
#pragma once

// Local private PANACEA includes
#include "entropy_decorator.hpp"

// Public PANACEA includes
#include "passkey.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;
  class EntropyTerm;

  class Weight : public EntropyDecorator {

    private:
      double weight_ = 1.0;
      inline static const PassKey<EntropyTerm> & static_key = key;

      EntropyTerm & getEntropyTerm();
    public:
      Weight(std::unique_ptr<EntropyTerm> entropy_term, const double & weight) : EntropyDecorator(std::move(entropy_term)), weight_(weight) {};

      virtual std::vector<EntropyTerm::ReadElement> getReadElements(const PassKey<EntropyTerm> &) override;
      virtual std::vector<EntropyTerm::WriteElement> getWriteElements(const PassKey<EntropyTerm> &) const override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper) override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind) override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const EntropySettings & entropy_settings
          ) override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const PANACEASettings & panacea_settings
          ) override;

      virtual double compute(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const PANACEASettings & panacea_settings) override;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind) override;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) override;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind,
          const PANACEASettings & panacea_settings) override;

      virtual bool set(const settings::EntropyOption option, std::any val) override;
      virtual std::any get(const settings::EntropyOption option) const override;

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          const EntropyTerm &);

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          EntropyTerm &);

  };

}

#endif // PANACEA_PRIVATE_ENTROPYDECORATOR_WEIGHT_H
