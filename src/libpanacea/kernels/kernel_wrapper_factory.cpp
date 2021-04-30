
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "base_kernel_wrapper.hpp"
#include "error.hpp"
#include "kernel_specifications.hpp"
#include "mean.hpp"
#include "mean_kernel_wrapper.hpp"
#include "median.hpp"
#include "median_kernel_wrapper.hpp"
#include "passkey.hpp"
#include "primitives/gaussian_uncorrelated.hpp"
#include "private_settings.hpp"

// Standard includes
#include <memory>
#include <string>
#include <typeindex>

namespace panacea {

  std::unordered_map<settings::KernelCenterCalculation, 
    std::unordered_map<std::type_index, KernelWrapperFactory::KernelCreateMethod>>
    KernelWrapperFactory::create_methods_;

  KernelWrapperFactory::KernelWrapperFactory() {

    registerKernel<settings::KernelCenterCalculation::None,
      std::vector<std::vector<double>>*,
      KernelWrapper<std::vector<std::vector<double>>*>>();

    registerKernel<settings::KernelCenterCalculation::None,
      double ***,
      KernelWrapper<double ***>>();

    registerKernel<settings::KernelCenterCalculation::Mean,
      std::vector<double>,
      MeanKernelWrapper>();

    registerKernel<settings::KernelCenterCalculation::Median,
      std::vector<double>,
      MedianKernelWrapper>();
  }

  std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
      const BaseDescriptorWrapper * desc_wrapper, 
      const KernelSpecification & kern_specification) const {

    // Ensure valid method exists
    if( create_methods_.count(kern_specification.get<settings::KernelCenterCalculation>()) == 0){
      std::string error_msg = "Kernel creation method is missing for the specified type: ";
      error_msg += settings::toString(kern_specification.get<settings::KernelCenterCalculation>());
      PANACEA_FAIL(error_msg);
    }

    if( kern_specification.is(settings::KernelCount::OneToOne)){

      if( desc_wrapper->getTypeIndex() != std::type_index(desc_wrapper->getPointerToRawData().type()) ) {
        std::string error_msg = "Descriptor data index and pointerToRawData are not consistent";
        PANACEA_FAIL(error_msg);
      }

      if( create_methods_[kern_specification.get<settings::KernelCenterCalculation>()].count(desc_wrapper->getTypeIndex()) == 0){
        std::string error_msg = "Kernel creation method is missing for the specified internal type.";
        PANACEA_FAIL(error_msg);
      }

      if( kern_specification.is(settings::KernelMemory::Share)){
      
        auto desc_data_type_index = desc_wrapper->getTypeIndex();
        if( desc_data_type_index != 
            std::type_index(typeid(std::vector<std::vector<double>> *)) &&
            desc_data_type_index != std::type_index(typeid(double ***))){
          PANACEA_FAIL("Unsupported types detected, cannot create kernels."); 
        }

        if( not kern_specification.is(settings::KernelCenterCalculation::None )) {
          PANACEA_FAIL("Kernel Center Calculation must be None when Count is OneToOne."); 
        }
        return create_methods_[kern_specification.get<settings::KernelCenterCalculation>()][desc_data_type_index](
            PassKey<KernelWrapperFactory>(),
            desc_wrapper->getPointerToRawData(),
            desc_wrapper->rows(),
            desc_wrapper->cols());
      } else {
        PANACEA_FAIL("OneToOne kernels that are not shared, are not yet supported.");
      }
    } else if( kern_specification.is(settings::KernelCount::Single)) {
      if( kern_specification.is(settings::KernelMemory::Own )) {

        if( not kern_specification.is(settings::KernelCenterCalculation::Mean) &&
            not kern_specification.is(settings::KernelCenterCalculation::Median)) {
          std::string error_msg = "Kernel Center Calculation must be Mean or Median when Count is Single: ";
          error_msg += settings::toString(kern_specification.get<settings::KernelCenterCalculation>());
          PANACEA_FAIL(error_msg); 
        }

        return create_methods_[kern_specification.get<settings::KernelCenterCalculation>()]
        [std::type_index(typeid(std::vector<double>))](
          PassKey<KernelWrapperFactory>(),
          desc_wrapper,
          1,
          desc_wrapper->getNumberDimensions());

      }
    }
    std::string error_msg = "The combination of kernel specifications is not";
    error_msg += " yet supported.\n";

    error_msg += kern_specification.get<std::string>();
    PANACEA_FAIL(error_msg);
    return std::unique_ptr<BaseKernelWrapper>();
  }
}
