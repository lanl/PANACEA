
// Local private PANACEA includes
#include "distribution_factory.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "distributions/kernel_distribution.hpp"
#include "error.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <iostream>
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

  std::unordered_map<settings::DistributionType,
    DistributionFactory::DistributionCreateShellMethod>
        DistributionFactory::create_shell_methods_;

  DistributionFactory::DistributionFactory() {
    DistributionFactory::registerDistribution<KernelDistribution,settings::DistributionType::Kernel>();
  }

  std::unique_ptr<Distribution> DistributionFactory::create(
      const BaseDescriptorWrapper * descriptor_wrapper,
      DistributionSettings * settings) const {

    assert(descriptor_wrapper != nullptr);
    assert(settings != nullptr);

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    if(create_methods_.count(settings->type()) == 0){
      std::string error_msg = "Distribution type is not registered with the factory.";
      PANACEA_FAIL(error_msg);
    }

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return create_methods_.at(settings->type())(
        PassKey<DistributionFactory>(),
        descriptor_wrapper,
        settings);
  }

  std::unique_ptr<Distribution> DistributionFactory::create(
      DistributionSettings * settings) const {
    
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    std::cout << "Dist type " << settings->type() << std::endl;
    return create_shell_methods_.at(settings->type())(
        PassKey<DistributionFactory>(),
        settings);
  }

}
