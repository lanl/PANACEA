#ifndef PANACEA_PRIVATE_ENTROPYTERM_H
#define PANACEA_PRIVATE_ENTROPYTERM_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"

// Standard includes
#include <any>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;

  class EntropyTerm {
    public:
      virtual settings::EntropyType type() const noexcept = 0;

      /**
       * Computes entropy if all points are used to sample the entropy
       **/
      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      /**
       * Computes the entropy if a single point is used to sample the entropy
       **/
      virtual double compute(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind) = 0;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) = 0;

      virtual void set(const settings::EntropyOption & option, std::any val) = 0;

      /**
       * Get the actual dimensions used by the entropy term
       **/
      virtual const std::vector<int> & getDimensions() const noexcept = 0;

      /**
       * Update the internals
       **/
      virtual void update(const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      virtual ~EntropyTerm() = 0;
  };
}

#endif // PANACEA_PRIVATE_ENTROPYTERM_H
