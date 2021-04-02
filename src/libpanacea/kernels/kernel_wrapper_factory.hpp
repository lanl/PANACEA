#ifndef PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#define PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#pragma once

// Local private includes
#include "base_kernel_wrapper.hpp"
#include "descriptors/base_descriptor_wrapper.hpp"
#include "kernel_specifications.hpp"
#include "memory.hpp"

// Standard includes
#include <memory>

namespace panacea {

  class KernelWrapperFactory {

    private:

      using KernelCreateMethod = unique_ptr<BaseKernelWrapper>(*)(PassKey<KernelWrapperFactory>, T data, int rows, int cols);

      std::unordered_map<std::typeinfo, KernelCreateMethod> create_methods_;

    public:

      std::unique_ptr<BaseKernelWrapper> create(
          BaseDescriptorWrapper * desc_wrapper,
          const KernelSpecification & kern_specification , 
          MemoryManager & memory_manager) const;

      template<class T>
      static bool registerType() {
        if( create_methods_.count(typeid<T>::value) == 0){
          create_methods_[typeid<T>::value] = KernelWrapper<T>::create();
          return true;
        }

        // Standard types 
        if( create_methods_.count(typeid<std::vector<double>*>::value) == 0){
          create_methods_[typeid<std::vector<double>*>::value] = 
            KernelWrapper<std::vector<double>*>::create();
          return true;
        }

        return false;
      }
  };

}

#endif // PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
