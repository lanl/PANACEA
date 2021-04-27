#ifndef PANACEA_PRIVATE_PRIMITIVE_FACTORY
#define PANACEA_PRIVATE_PRIMITIVE_FACTORY
#pragma once

// Local private includes
#include "passkey.hpp"
#include "primitive_group.hpp"
#include "settings.hpp"

// Standard includes
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class KernelSpecification;
  class MemoryManager;
  class Primitive;
  class PrimitiveAttributes;
  class PrimitiveFactory;

  class PrimitiveFactory {

    public: 
      using PrimitiveCreateMethod = std::unique_ptr<Primitive>(*)(
        const PassKey<PrimitiveFactory> &,
        PrimitiveAttributes prim_attr,
        const int & kernel_index);

      using PrimitiveCountMethod = void(*)(
        const PassKey<PrimitiveFactory> &,
        PrimitiveGroup & prim_grp,
        const KernelSpecification & specification);

    private:

      static std::unordered_map<settings::KernelPrimitive,
        std::unordered_map<settings::KernelCorrelation,
        PrimitiveCreateMethod>> create_methods_;

      static std::unordered_map<settings::KernelCount,
        PrimitiveCountMethod> count_methods_;

      static void OneToOne(
        const PassKey<PrimitiveFactory> &,
        PrimitiveGroup & prim_grp,
        const KernelSpecification & specification);

      static void Single(
        const PassKey<PrimitiveFactory> &,
        PrimitiveGroup & prim_grp,
        const KernelSpecification & specification);

    public:

      PrimitiveFactory();

      template<class T, 
        settings::KernelPrimitive kern_prim,
        settings::KernelCorrelation kern_corr>
      static bool registerPrimitive(){
        if( create_methods_.count(kern_prim) ) {
          if( create_methods_[kern_prim].count(kern_corr) ) {
            return false;
          } else {
            create_methods_[kern_prim][kern_corr] = T::create;
          }
        } else {
          create_methods_[kern_prim][kern_corr] = T::create;
        }
        return true;
      }

      PrimitiveGroup create(
          const BaseDescriptorWrapper * dwrapper,
          MemoryManager & mem_manager,
          const KernelSpecification & specification,
          std::string name = "") const;
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVE_FACTORY

