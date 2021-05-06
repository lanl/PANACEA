#ifndef PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#define PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#pragma once

// Local private includes
#include "kernel_wrapper.hpp"
#include "private_settings.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace panacea {

  class BaseDescriptorWrapper;
  class BaseKernelWrapper;
  class KernelSpecification;

  class KernelWrapperFactory {
    public:

      using KernelCreateMethod = std::unique_ptr<BaseKernelWrapper>(*)(
          const PassKey<KernelWrapperFactory> &,
          std::any data,
          const int rows,
          const int cols);

    private:

      static std::unordered_map<settings::KernelCenterCalculation,
        std::unordered_map<std::type_index, KernelCreateMethod>> create_methods_;

    public:

      KernelWrapperFactory();

      /**
       * Will create a kernel wrapper who's fields are populated based on what is 
       * in the descriptor wrapper. 
       **/
      std::unique_ptr<BaseKernelWrapper> create(
          const BaseDescriptorWrapper * desc_wrapper,
          const KernelSpecification & kern_specification) const;

      /**
       * Will create a shell of a kernel wrapper that is essentially empty. This is an
       * ideal state for reading in information from a restart file to populate the kernel
       * wrapper. Note, that the data (Memory) setting must be set to Own to utilize this
       * method.
       **/
      std::unique_ptr<BaseKernelWrapper> create(
          const KernelSpecification & kern_specification) const;

      template<settings::KernelCenterCalculation kernel_center, class T, class S>
        static bool registerKernel() {
          if( create_methods_.count(kernel_center) == 0 ) {
            create_methods_[kernel_center][std::type_index(typeid(typename std::remove_const<T>::type))] = S::create;
            return true;
          } else {
            if( create_methods_[kernel_center].count(std::type_index(typeid(typename std::remove_const<T>::type))) == 0){
              create_methods_[kernel_center][std::type_index(typeid(typename std::remove_const<T>::type))] = S::create;
              return true;
            }
          }
          return false;
        }
  };
}

#endif // PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
