#ifndef PANACEA_PRIVATE_DISTRIBUTION_FACTORY
#define PANACEA_PRIVATE_DISTRIBUTION_FACTORY
#pragma once

// Local private includes
#include "distributions/distribution.hpp"
#include "distribution_settings/distribution_settings.hpp"
#include "memory.hpp"
#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <any>
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;

  class DistributionFactory {
    public:
      using DistributionCreateMethod = std::unique_ptr<Distribution>(*)(
        const PassKey<DistributionFactory> &,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          DistributionSettings * dist_settings,
          std::string name
          );

    private:
      static std::unordered_map<settings::DistributionType,DistributionCreateMethod>
        create_methods_;

    public:

      template<class T, settings::DistributionType dist_type>
        static bool registerDistribution() {
          if( create_methods_.count(dist_type) ) {
            return false;
          } else {
            create_methods_[dist_type] = T::create;
          }
          return true;
        }

      std::unique_ptr<Distribution> create(
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          DistributionSettings * settings,
          std::string name = "") const;
  };

}
#endif // PANACEA_PRIVATE_DISTRIBUTION_FACTORY
