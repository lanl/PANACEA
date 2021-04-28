
// Local private PANACEA includes
#include "kernel_distribution_settings.hpp"

#include "private_settings.hpp"

namespace panacea {

  settings::DistributionType KernelDistributionSettings::type() const noexcept {
    return settings::DistributionType::Kernel;
  }
}
