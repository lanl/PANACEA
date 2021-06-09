
#ifndef PANACEA_PRIVATE_SELFENTROPY_H
#define PANACEA_PRIVATE_SELFENTROPY_H
#pragma once

// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "distribution/distributions/distribution.hpp"
#include "private_settings.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

class EntropyFactory;

class SelfEntropy : public EntropyTerm {
private:
  std::unique_ptr<Distribution> distribution_;

public:
  SelfEntropy(const PassKey<EntropyFactory> &key,
              std::unique_ptr<Distribution> dist)
      : distribution_(std::move(dist)){};

  virtual EntropyTerm::ReadFunction
  getReadFunction(const PassKey<EntropyTerm> &) override;
  virtual EntropyTerm::WriteFunction
  getWriteFunction(const PassKey<EntropyTerm> &) override;

  virtual settings::EntropyType type() const noexcept final;

  virtual double
  compute(const BaseDescriptorWrapper *descriptor_wrapper) override;

  virtual double compute(const BaseDescriptorWrapper *descriptor_wrapper,
                         const int desc_ind) override;

  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper *descriptor_wrapper,
               const int desc_ind,
               const EntropySettings &entropy_settings) override;

  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper *descriptor_wrapper,
               const int desc_ind,
               const PANACEASettings &panacea_settings) override;

  virtual void set(const settings::EntropyOption option, std::any val) override;

  virtual const std::vector<int> &getDimensions() const noexcept override;

  virtual void update(const BaseDescriptorWrapper *descriptor_wrapper) override;

  virtual void
  initialize(const BaseDescriptorWrapper *descriptor_wrapper) override;

  static std::unique_ptr<EntropyTerm>
  create(const PassKey<EntropyFactory> &key,
         const BaseDescriptorWrapper *descriptor_wrapper,
         EntropySettings *settings);

  static std::unique_ptr<EntropyTerm> create(const PassKey<EntropyFactory> &key,
                                             EntropySettings *settings);

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &, EntropyTerm *);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &, EntropyTerm *);
};

} // namespace panacea
#endif // PANACEA_PRIVATE_SELFENTROPY_H
