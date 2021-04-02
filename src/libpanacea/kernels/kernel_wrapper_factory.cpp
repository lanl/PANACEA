
// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "base_kernel_wrapper.hpp"
#include "error.hpp"
#include "kernel_specifications.hpp"
#include "memory.hpp"
#include "primitives/gaussian_uncorrelated.hpp"

// Standard includes
#include <memory>

namespace panacea {

  std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
     BaseDescriptorWrapper * desc_wrapper, 
     const KernelSpecification & kern_specification,
     MemoryManager & memory_manager) const {

    if( kern_specification.is(settings::KernelPrimitive::Gaussian)) {
      if( kern_specification.is(settings::KernelCount::OneToOne)){
        if( kern_specification.is(settings::KernelCorrelation::Uncorrelated)){
          if( kern_specification.is(settings::KernelMemory::Share)){


            auto any_ptr = desc_wrapper->getPointerToRawData();
            if( any_ptr.type() == typeid(std::vector<double> *)){
              memory_manager.managePointer(
                  std::any_cast<std::vector<double>*>(desc_wrapper->getPointerToRawData()),
                  "Kernel Centers");
            } else {
              PANACEA_FAIL("Unsupported types detected, cannot create kernels."); 
            }

            return create_methods_[(*desc_wrapper)::type](
                  PassKey<KernelWrapperFactory>(),
                  desc_wrapper,
                  desc_wrapper->rows(),
                  desc_wrapper->cols());
          }
        }
      } else if( kern_specification.is(settings::KernelCount::Single)) {
        if( kern_specification.is(settings::KernelCorrelation::Correlated) ){
          if( kern_specification.is(settings::KernelMemory::Own )) {
            // Create data for the kernel
            std::vector<double> center;
            if( kern_specification.is(settings::KernelCenter::Mean )) {
              Mean mean;
              center = mean.calculate(desc_wrapper);
            } else if (kern_specification.is(settings::KernelCenter::Mean )) {
              Median median;
              center = median.calculate(desc_wrapper);
            }
            auto kernel_center = std::make_unique<std::vector<double>>(center);

            auto kwrapper = create_methods_[typeid<std::vector<double>*>::value](
                PassKey<KernelWrapperFactory>(),
                kernel_center.get(),
                1,
                kernel_center.size());

            // Unlike managePointer manageMemory means the memory manager now
            // owns the memory data
            memory_manager.manageMemory(
                std::move(kernel_center),
                "Kernel Centers");

            return kwrapper;
          }
        }
      }
    }

    std::string error_msg = "The combination of kernel specifications is not";
    error_msg += " yet supported.";
    PANACEA_FAIL(error_msg);
  }
}
