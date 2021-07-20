
#ifndef PANACEA_H
#define PANACEA_H
#pragma once

// Public PANACEA includes
#include "file_io.hpp"
#include "settings.hpp"

// Standard includes
#include <any>
#include <memory>

namespace panacea {

class BaseDescriptorWrapper;
class EntropyTerm;

class PANACEA {

public:
  /**
   * This method is used to create an Adapter class
   *
   * External data can be wrapped in order to be used
   * by the library.
   *
   **/
  std::unique_ptr<BaseDescriptorWrapper> wrap(std::any, const int rows,
                                              const int cols) const;

  /**
   * Method will fully initialize an entropy term.
   *
   * Here an entropy term will be fully initialized using the
   * provided descriptor wrapper and its settings.
   **/
  std::unique_ptr<EntropyTerm> create(const BaseDescriptorWrapper &,
                                      const PANACEASettings &settings) const;

  /**
   * Partial creation of an entropy term.
   *
   * Only a shell of an entropy term will be initialized if
   * this method is called. It will create a class instance but
   * will not fully allocate memory for all the members.
   *
   * Before the entropy term can be used to calculate the entropy
   * of a system or its gradiant, either an initialize method of
   * the term must be called or data needs to be loaded from a
   * restart file into the term.
   *
   * There are different reasons why this might be useful, i.e.
   * there are multiple stages to initialization and the descriptors
   * may not be initially known.
   **/
  std::unique_ptr<EntropyTerm> create(const PANACEASettings &settings) const;

  std::unique_ptr<EntropyTerm> create(const std::string &file_name) const;

  std::unique_ptr<io::FileIO> create(const settings::FileType) const;
};

}; // namespace panacea

#endif // PANACEA_H
