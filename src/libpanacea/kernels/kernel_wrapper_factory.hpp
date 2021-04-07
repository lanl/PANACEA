#ifndef PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#define PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#pragma once

// Local private includes
//#include "base_kernel_wrapper.hpp"
//#include "descriptors/base_descriptor_wrapper.hpp"
//#include "kernel_specifications.hpp"
#include "kernel_wrapper.hpp"
//#include "memory.hpp"
#include "passkey.hpp"

// Standard includes
#include <any>
#include <memory>
#include <typeindex>
#include <unordered_map>
//#include <vector>

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

      static std::unordered_map<std::type_index, KernelCreateMethod> create_methods_;

    public:

      std::unique_ptr<BaseKernelWrapper> create(
          BaseDescriptorWrapper * desc_wrapper,
          const KernelSpecification & kern_specification , 
          MemoryManager & memory_manager) const;

      template<class T>
        static bool registerType() {
          if( create_methods_.count(typeid(T)) == 0){
            create_methods_[typeid(T)] = KernelWrapper<T>::create;
            return true;
          }
          return false;
        }
  };
}

#endif // PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
