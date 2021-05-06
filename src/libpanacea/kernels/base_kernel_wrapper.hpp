
#ifndef PANACEA_PRIVATE_BASEKERNELWRAPPER_H
#define PANACEA_PRIVATE_BASEKERNELWRAPPER_H
#pragma once

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cstddef>
#include <typeindex>

namespace panacea {

  enum class Arrangement;
  class BaseDescriptorWrapper;

  namespace settings {
    enum class FileType;
    enum class KernelCount;
    enum class KernelCenterCalculation;
  }

  class BaseKernelWrapper;

  /*
   * Base Kernel interface 
   */
  class BaseKernelWrapper  {
    public: 
      typedef std::istream & (*ReadFunction)(BaseKernelWrapper *, std::istream & is); 
      typedef std::ostream & (*WriteFunction)(BaseKernelWrapper *, std::ostream & os); 

    private:

      /**
       * Used to read and write kernel derived class specific meta data
       **/
      virtual BaseKernelWrapper::ReadFunction getReadFunction_() = 0;
      virtual BaseKernelWrapper::WriteFunction getWriteFunction_() = 0;

    public:

      virtual const settings::KernelCenterCalculation center() const noexcept = 0;
      virtual const settings::KernelCount count() const noexcept = 0;

      /**
       * Notice this behavior is different from the descriptor wrapper here
       * we should be passing in the row and column indices not the desc
       * point index and dimension.
       **/
      virtual double& at(const int row, const int col) = 0;
      virtual double at(const int row, const int col) const = 0;

      virtual void resize(const int rows, const int cols) = 0;

      /**
       * Gets the total number of rows of data stored by the kernel
       **/
      virtual int rows() const = 0;

      /**
       * Gets the total number of cols of data stored by the kernel
       **/
      virtual int cols() const = 0;
      virtual int getNumberDimensions() const = 0; 

      /**
       * Returns the total number of points used to create the kernel
       **/
      virtual int getNumberPoints() const = 0; 
      virtual const Arrangement & arrangement() const noexcept = 0; 
      virtual void set(const Arrangement arrangement) = 0;

      /**
       * Update the kernel wrapper
       *
       * The BaseDescriptorWrapper used to update the kernel must have the same number of
       * dimensions as the descriptor wrapper that was used to initialize the kernel.
       **/
      virtual void update(const BaseDescriptorWrapper *) = 0;
      virtual const std::any getPointerToRawData() const noexcept = 0;
      virtual std::type_index getTypeIndex() const noexcept = 0; 
      virtual void print() const = 0;
      virtual ~BaseKernelWrapper() = 0;

      static std::vector<std::any> write(
          const settings::FileType & file_type,
          std::ostream &,
          std::any kern_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType & file_type,
          std::istream &,
          std::any kern_instance);

  };

}
#endif // PANACEA_PRIVATE_BASEKERNELWRAPPER_H
