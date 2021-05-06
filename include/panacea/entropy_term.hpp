#ifndef PANACEA_ENTROPYTERM_H
#define PANACEA_ENTROPYTERM_H
#pragma once

// Local private PANACEA includes
#include "settings.hpp"
#include "file_io_types.hpp"
#include "passkey.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;

  class EntropyTerm {
    public:
      typedef io::ReadInstantiateVector (*ReadFunction)(
          const settings::FileType & file_type,
          std::istream &,
          EntropyTerm *); 

      typedef std::vector<std::any> (*WriteFunction)(
          const settings::FileType & file_type,
          std::ostream &,
          EntropyTerm *); 

    protected:
      const PassKey<EntropyTerm> key;

    public:
      virtual EntropyTerm::ReadFunction getReadFunction(const PassKey<EntropyTerm> &) = 0;
      virtual EntropyTerm::WriteFunction getWriteFunction(const PassKey<EntropyTerm> &) = 0;

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

      static std::vector<std::any> write(
          const settings::FileType & file_type,
          std::ostream &,
          std::any dwrapper_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType & file_type,
          std::istream &,
          std::any dwrapper_instance);



  };
}

#endif // PANACEA_ENTROPYTERM_H
