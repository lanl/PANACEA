
// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "primites/gaussian_uncorrelated.hpp"

// Standard includes
#include <memory>

namespace panacea {

  std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
     BaseDescriptorWrapper * desc_wrapper, 
     const KernelSpecification & kern_specification) const {

    if( kern_specification.is(KernelPrimitive::Gaussian)) {
      if( kern_specification.is(KernelCount::OneToOne)){
        if( kern_specification.is(KernelCorrelation::Uncorrelated)){
          if( kern_specification.is(KernelMemory::Shared)){

          }
        }
      }
    }

    std::string error_msg = "The combination of kernel specifications is not";
    error_msg += " yet supported.";
    PANACEA_FAIL(error_msg);
  }
}
