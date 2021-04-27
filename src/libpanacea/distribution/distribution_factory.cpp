
// Local private PANACEA includes
#include "distribution_factory.hpp"

#include "distributions/kernel_distribution.hpp"
#include "memory.hpp"
#include "error.hpp"
#include "passkey.hpp"

// Standard includes
#include <any>
#include <memory>
#include <string>
#include <unordered_map>

namespace panacea {
  /***************************************************************
   * Declaring private Member function maps
   **************************************************************/

  std::unordered_map<settings::DistributionType,
    DistributionFactory::DistributionCreateMethod>
        DistributionFactory::create_methods_;

  DistributionFactory::DistributionFactory() {
    DistributionFactory::registerDistribution<KernelDistribution,settings::DistributionType::Kernel>();
  }

  std::unique_ptr<Distribution> DistributionFactory::create(
      const BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      DistributionSettings * settings,
      std::string name) const {

    assert(settings != nullptr);

    if(create_methods_.count(settings->type()) == 0){
      std::string error_msg = "Distribution type is not registered with the factory.";
      PANACEA_FAIL(error_msg);
    }

    return create_methods_.at(settings->type())(
        PassKey<DistributionFactory>(),
        descriptor_wrapper,
        mem_manager,
        settings,
        name);
  }
}
