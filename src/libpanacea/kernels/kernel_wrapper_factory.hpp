#ifndef PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#define PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#pragma once

// Local private includes
#include "kernel_wrapper.hpp"
#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace panacea {

  class BaseDescriptorWrapper;
  class BaseKernelWrapper;
  class KernelSpecification;
  class MemoryManager;

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

      std::unique_ptr<BaseKernelWrapper> create(
          BaseDescriptorWrapper * desc_wrapper,
          const KernelSpecification & kern_specification , 
          MemoryManager & memory_manager,
          std::string name) const;

      template<settings::KernelCenterCalculation kernel_center, class T, class S>
        static bool registerKernel() {
          if( create_methods_.count(kernel_center) == 0 ) {
            create_methods_[kernel_center][typeid(T)] = S::create;
            return true;
          } else {
            if( create_methods_[kernel_center].count(typeid(T)) == 0){
              create_methods_[kernel_center][typeid(T)] = S::create;
              return true;
            }
          }
          return false;
        }
  };
}

#endif // PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
