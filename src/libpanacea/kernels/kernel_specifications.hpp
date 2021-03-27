#ifndef PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
#define PANACEA_PRIVATE_KERNEL_SPECIFICATIONS

namespace panacea {

  class KernelSpecification {
      const KernelCorrelation kern_correlation_;
      const KernelCount kern_count_;
      const KernelPrimitive kern_prim_;
      const KernelNormalization kern_normalization_;
      const KernelMemory kern_memory_;
    public:
      KernelSpecification(
          const KernelCorrelation & kern_correlation,
          const KernelCount & kern_count,
          const KernelPrimitive & kern_prim,
          const KernelNormalization & kern_normalization,
          const KernelMemory & kern_memory) :
        kern_correlation_(kern_correlation),
        kern_count_(kern_count),
        kern_prim_(kern_prim),
        kern_normalization_(kern_normalization),
        kern_memory_(kern_memory)
          {};

      bool is(const KernelCorrelation & correlation) const {
        if( correlation == kern_correlation_ ) return true;
        return false;
      }

      bool is(const KernelCount & kern_count) const {
        if( kern_count == kern_count_ ) return true;
        return false;
      }

      bool is(const KernelPrimitive & kern_prim) const;
        if( kern_prim == kern_prim_ ) return true;
        return false;
      }

      bool is(const KernelNormalization & kern_normalization) const;
        if( kern_normalization == kern_normalization_ ) return true;
        return false;
      }
      
      bool is(const KernelMemory & kern_memory) const;
        if( kern_memory == kern_memory_ ) return true;
        return false;
      }
  };
}
#endif // PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
