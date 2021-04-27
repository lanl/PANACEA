
#ifndef PANACEA_PRIVATE_BASEKERNELWRAPPER_H
#define PANACEA_PRIVATE_BASEKERNELWRAPPER_H
#pragma once

// Standard includes
#include <any>
#include <cstddef>
#include <typeindex>

namespace panacea {

  enum class Arrangement;
  class BaseDescriptorWrapper;
  /*
   * Base Kernel interface 
   */
  class BaseKernelWrapper  {
    public:
      virtual double& operator()(const int point_ind, const int dim_ind) = 0;
      virtual double operator()(const int point_ind, const int dim_ind) const = 0;
      virtual int rows() const = 0;
      virtual int cols() const = 0;
      virtual int getNumberDimensions() const = 0; 
      virtual int getNumberPoints() const = 0; 
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
  };
}
#endif // PANACEA_PRIVATE_BASEKERNELWRAPPER_H
