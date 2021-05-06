#ifndef PANACEA_PRIVATE_PRIMITIVE_FACTORY
#define PANACEA_PRIVATE_PRIMITIVE_FACTORY
#pragma once

// Local private includes
#include "passkey.hpp"
#include "primitive_group.hpp"
#include "private_settings.hpp"

// Standard includes
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class KernelSpecification;
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
        PrimitiveGroup & prim_grp);

    private:

      static std::unordered_map<settings::KernelPrimitive,
        std::unordered_map<settings::KernelCorrelation,
        PrimitiveCreateMethod>> create_methods_;

      static std::unordered_map<settings::KernelCount,
        PrimitiveCountMethod> count_methods_;

      static void OneToOne(
        const PassKey<PrimitiveFactory> &,
        PrimitiveGroup & prim_grp);

      static void Single(
        const PassKey<PrimitiveFactory> &,
        PrimitiveGroup & prim_grp);

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

      /**
       * Create a primitive group
       **/
      PrimitiveGroup createGroup(
          const BaseDescriptorWrapper * dwrapper,
          const KernelSpecification & specification,
          const std::string & name = "") const;

      /**
       * Create a primitive group
       *
       * Creates a shell of the primitive group that is appropriate for
       * loading in values from a restart file
       **/
      PrimitiveGroup createGroup(
          const KernelSpecification & specification,
          const std::string & name = "") const;

      /**
       * Updates the primitive group with the values from the new descriptors
       * provided by dwrapper.
       **/ 
      void update(
          const PassKey<PrimitiveGroup> &,
          const BaseDescriptorWrapper * dwrapper,
          PrimitiveGroup & primitive_grp) const;

      
      enum class ResetOption {
        All,
        ReducedCovariance,
        ReducedInvCovariance,
        Primitives
      };

      /**
       * Provides a means for resetting values in the primitive group 
       **/
      void reset(
          const PassKey<PrimitiveGroup> &,
          PrimitiveGroup & primitive_grp,
          const ResetOption = ResetOption::All) const;
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVE_FACTORY

