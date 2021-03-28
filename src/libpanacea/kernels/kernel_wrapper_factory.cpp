
// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "error.hpp"
#include "kernel_specifications.hpp"
#include "primitives/gaussian_uncorrelated.hpp"

// Standard includes
#include <memory>

namespace panacea {

  std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
     BaseDescriptorWrapper * desc_wrapper, 
     const KernelSpecification & kern_specification) const {

    if( kern_specification.is(settings::KernelPrimitive::Gaussian)) {
      if( kern_specification.is(settings::KernelCount::OneToOne)){
        if( kern_specification.is(settings::KernelCorrelation::Uncorrelated)){
          if( kern_specification.is(settings::KernelMemory::Share)){

          }
        }
      }
    }

    std::string error_msg = "The combination of kernel specifications is not";
    error_msg += " yet supported.";
    PANACEA_FAIL(error_msg);
  }
}
