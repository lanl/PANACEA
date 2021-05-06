#ifndef PANACEA_PRIVATE_DISTRIBUTION_FACTORY
#define PANACEA_PRIVATE_DISTRIBUTION_FACTORY
#pragma once

// Local private includes
#include "distributions/distribution.hpp"
#include "distribution_settings/distribution_settings.hpp"
#include "private_settings.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

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
          const BaseDescriptorWrapper * descriptor_wrapper,
          DistributionSettings * dist_settings);

      using DistributionCreateShellMethod = std::unique_ptr<Distribution>(*)(
        const PassKey<DistributionFactory> &,
          DistributionSettings * dist_settings);

    private:
      static std::unordered_map<settings::DistributionType,DistributionCreateMethod>
        create_methods_;

      static std::unordered_map<settings::DistributionType,DistributionCreateShellMethod>
        create_shell_methods_;

    public:
      DistributionFactory();

      template<class T, settings::DistributionType dist_type>
        static bool registerDistribution() {
          if( create_methods_.count(dist_type) ) {
            return false;
          } else {
            create_methods_[dist_type] = T::create;
          }

          if( create_shell_methods_.count(dist_type) ) {
            return false;
          } else {
            create_shell_methods_[dist_type] = T::create;
          }
          return true;
        }

      std::unique_ptr<Distribution> create(
          const BaseDescriptorWrapper * descriptor_wrapper,
          DistributionSettings * settings) const;

      /**
       * Will create a shell of a distribution as no descriptor wrapper was provided.
       **/
      std::unique_ptr<Distribution> create(
          DistributionSettings * settings) const;

  };

}
#endif // PANACEA_PRIVATE_DISTRIBUTION_FACTORY
