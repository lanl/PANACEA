#ifndef PANACEA_ENTROPYTERM_H
#define PANACEA_ENTROPYTERM_H
#pragma once

// Local private PANACEA includes
#include "file_io_types.hpp"
#include "passkey.hpp"
#include "settings.hpp"

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
      const settings::FileType file_type, std::istream &, EntropyTerm &);

  typedef std::vector<std::any> (*WriteFunction)(
      const settings::FileType file_type, std::ostream &, const EntropyTerm &);

  struct ReadElement {
    ReadElement(ReadFunction read_in, EntropyTerm &term_in)
        : read(read_in), term(term_in){};
    ReadFunction read;
    EntropyTerm &term;
  };

  struct WriteElement {
    WriteElement(WriteFunction write_in, const EntropyTerm &term_in)
        : write(write_in), term(term_in){};
    WriteFunction write;
    const EntropyTerm &term;
  };

  /**
   * The state of the entropy term, it is possible to create an entropy
   * term that has not yet been fully initialized, e.g. if you need a
   * placeholder in memory where the data from a restart file needs to
   * be injected.
   **/
  enum class State { Shell, Initialized };

protected:
  inline static const PassKey<EntropyTerm> key;

public:
  /**
   * These are auxillary methods are used internally and are not meant to be
   *accessible
   **/
  virtual std::vector<EntropyTerm::ReadElement>
  getReadElements(const PassKey<EntropyTerm> &) = 0;
  virtual std::vector<EntropyTerm::WriteElement>
  getWriteElements(const PassKey<EntropyTerm> &) const = 0;

  /**
   * Will return the state of the entropy term
   *
   * It can be used to determine if it is appropraite to calculate the entropy
   * or gradiant yet or if further initialization is needed.
   **/
  virtual State state() const noexcept = 0;

  /**
   * Entropy type:
   * - Self
   * - Cross
   **/
  virtual settings::EntropyType type() const noexcept = 0;

  virtual bool set(const settings::EntropyOption opt, std::any value) = 0;

  virtual std::any get(const settings::EntropyOption opt) const = 0;

  /**
   * Computes entropy if all points are used to sample the entropy
   *
   * Uses internally stored settings.
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper) = 0;

  /**
   * Computes the entropy if a single point is used to sample the entropy
   *
   * Uses internally stored settings
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const int desc_ind) = 0;

  /**
   * Computes entropy if all points are used to sample the entropy
   *
   * Will overwrite settings with call to compute
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const EntropySettings &entropy_settings) = 0;

  /**
   * Computes the entropy if a single point is used to sample the entropy
   *
   * Will overwrite settings with call to compute
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const int desc_ind,
                         const EntropySettings &entropy_settings) = 0;

  /**
   * Computes entropy if all points are used to sample the entropy
   *
   * Will overwrite settings with call to compute
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const PANACEASettings &panacea_settings) = 0;

  /**
   * Computes the entropy if a single point is used to sample the entropy
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual double compute(const BaseDescriptorWrapper &descriptor_wrapper,
                         const int desc_ind,
                         const PANACEASettings &panacea_settings) = 0;

  /**
   * Computes the gradiant of the entropy term at the location of the
   * descriptor given by 'desc_ind'.
   *
   * The vector returned contains the gradiant in each dimension.
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper &descriptor_wrapper,
               const int desc_ind) = 0;

  /**
   * Computes the gradiant of the entropy term at the location of the
   * descriptor given by 'desc_ind'.
   *
   * The vector returned contains the gradiant in each dimension. In cases
   * where the dimensions being utilized have been reduced 0.0s will be
   * assigned to dimensions that are not used.
   *
   * Throws error if the entropy term has not been fully initialized before
   * calling compute.
   **/
  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper &descriptor_wrapper,
               const int desc_ind, const EntropySettings &entropy_settings) = 0;

  virtual std::vector<double>
  compute_grad(const BaseDescriptorWrapper &descriptor_wrapper,
               const int desc_ind, const PANACEASettings &panacea_settings) = 0;

  /**
   * Get the actual dimensions used by the entropy term, there is a filtering
   * process used to actally pick which dimensions are used. For instance
   * if linear dependence is detected between dimensions then some of the
   * dimensions will be removed.
   **/
  virtual const std::vector<int> getDimensions() const noexcept = 0;

  /**
   * Get the maximum number of dimension, this will be based on the dimensions
   * provided by the descriptor wrapper.
   **/
  virtual const int getMaximumNumberOfDimensions() const noexcept = 0;

  /**
   * Update the internal members.
   *
   * This method is used if it is desired to change how the entropy terms
   * are evalulating a configuration, where a configuration is determined by
   * the values of the descriptors.
   *
   * Updating the entropy term with the provided descriptors will allow the
   *entropy term to better adjust and more effectively calculate gradiants that
   *can lead to uniqueness or similarity in configurations depending on whether
   *the entropy term is to be maximized of minimized.
   **/
  virtual void update(const BaseDescriptorWrapper &descriptor_wrapper) = 0;

  /**
   * Initialize internal members.
   *
   * Similar to updating but assumes you are initializing data structures as
   *opposed to changing values already stored in them. With update the internal
   *datastructures should already exist in memory, initialize will allocate
   *memory on the heap if needed.
   **/
  virtual void initialize(const BaseDescriptorWrapper &descriptor_wrapper) = 0;

  /**
   * Some method to combine entropy terms, such that the covariance matrices are
   *updated.
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

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &, std::any entropy_instance);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &,
                                        std::any entropy_instance);
};
} // namespace panacea

#endif // PANACEA_ENTROPYTERM_H
