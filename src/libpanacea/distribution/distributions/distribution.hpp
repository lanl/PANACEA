#ifndef PANACEA_PRIVATE_BASEDISTRIBUTION_H
#define PANACEA_PRIVATE_BASEDISTRIBUTION_H
#pragma once

// Local private PANACEA includes
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <iostream>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class DistributionSettings;
  class Distribution;

  class Distribution {
    public:
      typedef io::ReadInstantiateVector (*ReadFunction)(
          const settings::FileType file_type,
          std::istream &,
          Distribution *); 

      typedef std::vector<std::any> (*WriteFunction)(
          const settings::FileType file_type,
          std::ostream &,
          Distribution *); 

    private:
      virtual Distribution::ReadFunction getReadFunction_() = 0;
      virtual Distribution::WriteFunction getWriteFunction_() = 0;
     
    public:
      virtual settings::DistributionType type() const noexcept = 0; 

      virtual double compute(const BaseDescriptorWrapper * descriptor_wrapper, const int desc_ind) = 0;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const int grad_ind, // The index associated with whatever we are taking the gradiant with respect to
          const DistributionSettings & distribution_settings,
          std::any extra_options = settings::None::None) = 0;

      /**
       * Get the actual dimensions used in the distribution
       **/
      virtual const std::vector<int> & getDimensions() const noexcept = 0;

      virtual void update(const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      virtual ~Distribution() = 0;

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          std::any dist_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          std::any dist_instance);

  };
}

#endif // PANACEA_PRIVATE_BASEDISTRIBUTION_H

