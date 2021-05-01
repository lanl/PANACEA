
#ifndef PANACEA_BASEDESCRIPTORWRAPPER_H
#define PANACEA_BASEDESCRIPTORWRAPPER_H
#pragma once

// Public PANACEA includes
#include "settings.hpp"

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
      virtual double& operator()(const int point_ind, const int dim_ind) = 0;
      virtual double operator()(const int point_ind, const int dim_ind) const = 0;
      virtual int rows() const = 0;
      virtual int cols() const = 0;
      virtual int getNumberDimensions() const = 0; 
      virtual int getNumberPoints() const = 0; 
      virtual const Arrangement & arrangement() const noexcept = 0; 
      virtual void set(const Arrangement arrangement) = 0;
      
      /**
       * Resizing the underlying data container will only be possible if the descriptor
       * wrapper actaully owns the data. 
       **/
      virtual void resize(const int rows, const int cols) = 0;
      virtual const std::any getPointerToRawData() const noexcept = 0;
      virtual std::type_index getTypeIndex() const noexcept = 0; 
      virtual void print() const = 0;

      virtual ~BaseDescriptorWrapper() = 0;

      static std::vector<std::any> write(
          settings::FileType file_type,
          std::ostream &,
          std::any vector_instance);

      static std::vector<std::any> read(
          settings::FileType file_type,
          std::istream &,
          std::any vector_instance);

  };
}
#endif // PANACEA_BASEDESCRIPTORWRAPPER_H
