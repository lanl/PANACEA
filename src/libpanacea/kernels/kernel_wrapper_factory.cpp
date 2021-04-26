
// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "base_kernel_wrapper.hpp"
#include "descriptors/base_descriptor_wrapper.hpp"
#include "error.hpp"
#include "kernel_specifications.hpp"
#include "mean.hpp"
#include "median.hpp"
#include "memory.hpp"
#include "passkey.hpp"
#include "settings.hpp"
#include "primitives/gaussian_uncorrelated.hpp"

// Standard includes
#include <memory>
#include <string>
#include <typeindex>

namespace panacea {

  std::unordered_map<settings::KernelCenterCalculation, 
    std::unordered_map<std::type_index, KernelWrapperFactory::KernelCreateMethod>>
    KernelWrapperFactory::create_methods_;

  std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
      BaseDescriptorWrapper * desc_wrapper, 
      const KernelSpecification & kern_specification,
      MemoryManager & memory_manager,
      std::string name = "") const {

    // Ensure valid method exists
    if( create_methods_.count(kern_specification.get<settings::KernelCenterCalculation>()) == 0){
      std::string error_msg = "Kernel creation method is missing for the specified type.";
      PANACEA_FAIL(error_msg);
    }
    if( create_methods_[kern_specification.get<settings::KernelCenterCalculation>()].count(desc_wrapper->getTypeIndex()) == 0){
      std::string error_msg = "Kernel creation method is missing for the specified type.";
      PANACEA_FAIL(error_msg);
    }
    
    name = "Kernel Centers: " + name;

    if( kern_specification.is(settings::KernelCount::OneToOne)){
      if( kern_specification.is(settings::KernelMemory::Share)){
        auto any_ptr = desc_wrapper->getPointerToRawData();
        if( any_ptr.type() == typeid(std::vector<std::vector<double>> *)){
          memory_manager.managePointer(
              std::any_cast<std::vector<std::vector<double>>*>(
                desc_wrapper->getPointerToRawData()),
              name);
        } else {
          PANACEA_FAIL("Unsupported types detected, cannot create kernels."); 
        }

        if( not kern_specification.is(settings::KernelCenterCalculation::None )) {
          PANACEA_FAIL("Kernel Center Calculation must be None when Count is OneToOne."); 
        }
        return create_methods_[kern_specification.get<settings::KernelCenterCalculation>()][desc_wrapper->getTypeIndex()](
            PassKey<KernelWrapperFactory>(),
            desc_wrapper->getPointerToRawData(),
            desc_wrapper->rows(),
            desc_wrapper->cols());
      }
    } else if( kern_specification.is(settings::KernelCount::Single)) {
      if( kern_specification.is(settings::KernelMemory::Own )) {

        if( not kern_specification.is(settings::KernelCenterCalculation::Mean) ||
            not kern_specification.is(settings::KernelCenterCalculation::Median)) {
          PANACEA_FAIL("Kernel Center Calculation must be Mean or Median when Count is Single."); 
        }


        return create_methods_[kern_specification.get<settings::KernelCenterCalculation>()]
        [typeid(std::vector<double>)](
          PassKey<KernelWrapperFactory>(),
          desc_wrapper,
          1,
          desc_wrapper->getNumberDimensions());


        // Create data for the kernel
/*        std::vector<std::vector<double>> centers;
        if( kern_specification.is(settings::KernelCenterCalculation::Mean )) {
          Mean mean;
          centers.push_back(mean.calculate(*desc_wrapper));
        } else if (kern_specification.is(settings::KernelCenterCalculation::Median )) {
          Median median;
          centers.push_back(median.calculate(*desc_wrapper));
        }
        auto kernel_center = std::make_unique<std::vector<std::vector<double>>>(centers);

        auto kwrapper = create_methods_[
          typeid(std::vector<std::vector<double>>*)](
              PassKey<KernelWrapperFactory>(),
              kernel_center.get(),
              1,
              kernel_center->size());
*/
        // Unlike managePointer manageMemory means the memory manager now
        // owns the memory data
//        memory_manager.manageMemory(
//            std::move(kernel_center),
//            name);

//        return kwrapper;
      }
    }
    std::string error_msg = "The combination of kernel specifications is not";
    error_msg += " yet supported.";
    PANACEA_FAIL(error_msg);
  }
}
