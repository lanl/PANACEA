#ifndef PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#define PANACEA_PRIVATE_KERNELDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "distribution.hpp"

#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "kernel_distribution/kernel_distribution_gradiant.hpp"
#include "primitives/primitive_group.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace panacea {

namespace settings {
enum class DistributionType;
enum class EquationSetting;
enum class GradSetting;
} // namespace settings

class BaseDescriptorWrapper;
class DistributionFactory;
class DistributionSettings;

class KernelDistribution : public Distribution {

private:
  PrimitiveGroup prim_grp_;
  KernelDistributionGradiant kern_dist_grad;
  // For KDE 1/N value, where N is the number Kernels/primitives
  double pre_factor_;

  virtual Distribution::ReadFunction getReadFunction_() final;
  virtual Distribution::WriteFunction getWriteFunction_() final;

public:
  KernelDistribution(const PassKey<DistributionFactory> &,
                     const BaseDescriptorWrapper *descriptor_wrapper,
                     const KernelSpecification &settings);

  /**
   * Creates a shell of the distribution that is appropriate for loading in
   * values from a restart file.
   **/
  KernelDistribution(const PassKey<DistributionFactory> &,
                     const KernelSpecification &settings);

  virtual settings::DistributionType type() const noexcept final;

  virtual double compute(const BaseDescriptorWrapper *descriptor_wrapper,
                         const int desc_ind) final;

  /**
   * Keep in mind the default grad_setting is inherited from distribution base
   *class.
   **/
  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper *descriptor_wrapper,
               const int desc_ind, const int grad_ind,
               const DistributionSettings &distribution_settings,
               std::any grad_setting) final;

  virtual const std::vector<int> &getDimensions() const noexcept final;

  /**
   * Will update the underlying data groups and ensure the prefactor is up to
   *date.
   **/
  virtual void update(const BaseDescriptorWrapper *descriptor_wrapper) final;

  virtual void
  initialize(const BaseDescriptorWrapper *descriptor_wrapper) final;

  static std::unique_ptr<Distribution>
  create(const PassKey<DistributionFactory> &,
         const BaseDescriptorWrapper *descriptor_wrapper,
         DistributionSettings *settings);

  static std::unique_ptr<Distribution>
  create(const PassKey<DistributionFactory> &, DistributionSettings *settings);

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &, Distribution *);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &, Distribution *);
};

inline std::unique_ptr<Distribution>
KernelDistribution::create(const PassKey<DistributionFactory> &key,
                           const BaseDescriptorWrapper *descriptor_wrapper,
                           DistributionSettings *settings) {

  assert(settings->type() == settings::DistributionType::Kernel);

  KernelDistributionSettings *kern_dist_settings =
      dynamic_cast<KernelDistributionSettings *>(settings);

  // The any must be the KernelSpecifications object
  return std::make_unique<KernelDistribution>(
      key, descriptor_wrapper, kern_dist_settings->dist_settings);
}

inline std::unique_ptr<Distribution>
KernelDistribution::create(const PassKey<DistributionFactory> &key,
                           DistributionSettings *settings) {

  assert(settings->type() == settings::DistributionType::Kernel);

  KernelDistributionSettings *kern_dist_settings =
      dynamic_cast<KernelDistributionSettings *>(settings);

  // Switch default Memory to OwnIfRestart, not possible to create a shell
  // distribution that does not own it's kernels if loading from a restart file.
  // If the distribution is later initialized then we can use the shared
  // approach instead
  // kern_dist_settings->dist_settings.set(settings::KernelMemory::OwnIfRestart);
  // The any must be the KernelSpecifications object
  return std::make_unique<KernelDistribution>(
      key, kern_dist_settings->dist_settings);
}
} // namespace panacea

#endif // PANACEA_PRIVATE_KERNELDISTRIBUTION_H
