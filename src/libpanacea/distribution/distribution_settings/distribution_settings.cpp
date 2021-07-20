
// Local private PANACEA includes
#include "distribution_settings.hpp"

#include "error.hpp"
#include "kernel_distribution_settings.hpp"

// Standard includes
#include <memory>
#include <string>

namespace panacea {
DistributionSettings::~DistributionSettings(){};

std::unique_ptr<DistributionSettings>
DistributionSettings::create(const DistributionSettings &settings) {

  if (settings.type() == settings::DistributionType::Kernel) {
    const KernelDistributionSettings &kernel_settings =
        dynamic_cast<const KernelDistributionSettings &>(settings);
    return std::make_unique<KernelDistributionSettings>(kernel_settings);
  }

  std::string error_msg = "Unsupported distribution type encountered.";
  PANACEA_FAIL(error_msg);

  return nullptr;
}

} // namespace panacea
