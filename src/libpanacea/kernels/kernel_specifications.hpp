#ifndef PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
#define PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
#pragma once
 
// Local private PANACEA includes
#include "settings.hpp"

namespace panacea {

  namespace defaults {
    const ::panacea::settings::KernelCorrelation kern_correlation_default 
      = ::panacea::settings::KernelCorrelation::Correlated;
    const ::panacea::settings::KernelCount kern_count_default 
      = ::panacea::settings::KernelCount::Single;
    const ::panacea::settings::KernelPrimitive kern_primitive_default 
      = ::panacea::settings::KernelPrimitive::Gaussian;
    const ::panacea::settings::KernelNormalization kern_normalization_default 
      = ::panacea::settings::KernelNormalization::Variance;
    const ::panacea::settings::KernelMemory kern_memory_default 
      = ::panacea::settings::KernelMemory::Own;
    const ::panacea::settings::KernelCenter kern_center_default 
      = ::panacea::settings::KernelCenter::Mean;
  }

  class KernelSpecification {
      const settings::KernelCorrelation kern_correlation_ = defaults::kern_correlation_default;
      const settings::KernelCount kern_count_ = defaults::kern_count_default;
      const settings::KernelPrimitive kern_prim_ = defaults::kern_primitive_default;
      const settings::KernelNormalization kern_normalization_ = defaults::kern_normalization_default;
      const settings::KernelMemory kern_memory_ = defaults::kern_memory_default;
      const settings::KernelCenter kern_center_ = defaults::kern_center_default;
    public:
      KernelSpecification(
          const settings::KernelCorrelation & kern_correlation,
          const settings::KernelCount & kern_count,
          const settings::KernelPrimitive & kern_prim,
          const settings::KernelNormalization & kern_normalization,
          const settings::KernelMemory & kern_memory,
          const settings::KernelCenter & kern_center) :
        kern_correlation_(kern_correlation),
        kern_count_(kern_count),
        kern_prim_(kern_prim),
        kern_normalization_(kern_normalization),
        kern_memory_(kern_memory),
        kern_center_(kern_center)
          {};

      inline bool is(const settings::KernelCorrelation & correlation) const noexcept {
        if( correlation == kern_correlation_ ) return true;
        return false;
      }

      inline bool is(const settings::KernelCount & kern_count) const noexcept {
        if( kern_count == kern_count_ ) return true;
        return false;
      }

      inline bool is(const settings::KernelPrimitive & kern_prim) const noexcept {
        if( kern_prim == kern_prim_ ) return true;
        return false;
      }

      inline bool is(const settings::KernelNormalization & kern_normalization) const noexcept {
        if( kern_normalization == kern_normalization_ ) return true;
        return false;
      }
      
      inline bool is(const settings::KernelMemory & kern_memory) const noexcept {
        if( kern_memory == kern_memory_ ) return true;
        return false;
      }
      
      inline bool is(const settings::KernelCenter & kern_center) const noexcept {
        if( kern_center == kern_center_ ) return true;
        return false;
      }
  };
}
#endif // PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
