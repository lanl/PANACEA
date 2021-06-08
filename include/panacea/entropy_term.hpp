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
          const settings::FileType file_type,
          std::istream &,
          EntropyTerm *);

      typedef std::vector<std::any> (*WriteFunction)(
          const settings::FileType file_type,
          std::ostream &,
          EntropyTerm *);

    protected:
      const PassKey<EntropyTerm> key;

    public:
      /**
       * These are auxillary methods used internally and are not meant to be accessible
       **/
      virtual EntropyTerm::ReadFunction getReadFunction(const PassKey<EntropyTerm> &) = 0;
      virtual EntropyTerm::WriteFunction getWriteFunction(const PassKey<EntropyTerm> &) = 0;

      /**
       * Entropy type:
       * - Self
       * - Cross
       **/
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

      /**
       * Computes the gradiant of the entropy term at the location of the
       * descriptor given by 'desc_ind'.
       *
       * The vector returned contains the gradiant in each dimension.
       **/
      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const EntropySettings & entropy_settings) = 0;

      virtual std::vector<double> compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const PANACEASettings & panacea_settings) = 0;

      virtual void set(const settings::EntropyOption option, std::any val) = 0;

      /**
       * Get the actual dimensions used by the entropy term
       **/
      virtual const std::vector<int> & getDimensions() const noexcept = 0;

      /**
       * Update the internal members.
       *
       * This method is used if it is desired to change how the entropy terms
       * are evalulating a configuration, where a configuration is determined by
       * the values of the descriptors.
       *
       * Updating the entropy term with the provided descriptors will allow the entropy
       * term to better adjust and more effectively calculate gradiants that can
       * lead to uniqueness or similarity in configurations depending on whether the
       * entropy term is to be maximized of minimized.
       **/
      virtual void update(const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      /**
       * Initialize internal members.
       *
       * Similar to updating but assumes you are initializing data structures as opposed
       * to changing values already stored in them. With update the internal datastructures
       * should already exist in memory, initialize will allocate memory on the heap if needed.
       **/
      virtual void initialize(const BaseDescriptorWrapper * descriptor_wrapper) = 0;

      /**
       * Some method to combine entropy terms, such that the covariance matrices are updated.
       *
       * By only storing a diff we can achieve a small memory footprint.
       *
       * Covariance and Mean and one delta object
       *
       * If you give me a delta I can
       *
       * Make test case, great
       **/

      virtual ~EntropyTerm() = 0;

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          std::any dwrapper_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          std::any dwrapper_instance);

  };
}

#endif // PANACEA_ENTROPYTERM_H
