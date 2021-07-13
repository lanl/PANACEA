
#ifndef PANACEA_BASEDESCRIPTORWRAPPER_H
#define PANACEA_BASEDESCRIPTORWRAPPER_H
#pragma once

// Public PANACEA includes
#include "settings.hpp"
#include "file_io_types.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <vector>
#include <typeindex>

namespace panacea {

  enum class Arrangement;
  /*
   * Base Descriptor interface
   */
  class BaseDescriptorWrapper {
    public:

      /**
       * Return the dimensions available to the wrapper
       **/
      virtual double& operator()(const int point_ind, const int dim_ind) = 0;
      virtual double operator()(const int point_ind, const int dim_ind) const = 0;
      virtual int rows() const = 0;
      virtual int cols() const = 0;

      /**
       * Get the number of dimensions associated with each descriptor point
       *
       * Take for instance an atomic configuration, each atom may have
       * a number of descriptors associated with it, in that case the atom
       * would be equivalent to a descriptor point located in a multi dimensinal
       * descriptor space. Each of the descriptors associated with the atom
       * would represent a descriptor dimension.
       *
       * E.g. A descriptor dimension could be element, or distance
       * between an atom and it's closest neigbor.
       **/
      virtual int getNumberDimensions() const = 0;

      /**
       * The number of descriptor points in the wrapper
       **/
      virtual int getNumberPoints() const = 0;

      /**
       * Returns an enum indicating if dimensions are along rows or columns
       *
       * The default when you construct a descriptor wrapper is to assume
       * that your data is arranged such that the rows represent different
       * points and the columns represent differen dimensions.
       **/
      virtual const Arrangement & arrangement() const noexcept = 0;

      /**
       * Here the arrangement can be changed
       *
       * We can alter whether the rows are associated with the points or
       * whether the columns are associated with the points.
       **/
      virtual void set(const Arrangement arrangement) = 0;

      /**
       * Resizing the underlying data container will only be possible if the descriptor
       * wrapper actaully owns the data.
       **/
      virtual void resize(const int rows, const int cols) = 0;
      virtual const std::any getPointerToRawData() const noexcept = 0;

      /**
       * Method will return the underlying type of the wrapper
       *
       * The descriptor wrapper is an adapter class, it thus abstracts
       * away the underlying storage type, in circumstances where it is
       * necessary to know what the undelying type is this method can be used.
       **/
      virtual std::type_index getTypeIndex() const noexcept = 0;

      /**
       * Will output to the terminal the contents of the wrapper.
       **/
      virtual void print() const = 0;

      virtual ~BaseDescriptorWrapper() = 0;

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
#endif // PANACEA_BASEDESCRIPTORWRAPPER_H
