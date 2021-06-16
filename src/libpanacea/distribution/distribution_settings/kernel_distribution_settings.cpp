
// Local private PANACEA includes
#include "kernel_distribution_settings.hpp"

#include "private_settings.hpp"

// Standard includes
#include <typeindex>

namespace panacea {

  settings::DistributionType KernelDistributionSettings::type() const noexcept {
    return settings::DistributionType::Kernel;
  }

  void KernelDistributionSettings::set(std::any val) {
    if( std::type_index(val.type()) == std::type_index(typeid(settings::EquationSetting)) ) {
      eq_settings = std::any_cast<settings::EquationSetting>(val);
    }
  }
}
