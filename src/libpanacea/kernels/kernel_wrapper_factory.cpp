// Private local PANACEA includes
#include "kernel_wrapper_factory.hpp"

#include "base_kernel_wrapper.hpp"
#include "error.hpp"
#include "kernel_specifications.hpp"
#include "mean.hpp"
#include "mean_kernel_wrapper.hpp"
#include "median.hpp"
#include "median_kernel_wrapper.hpp"
#include "primitives/gaussian_uncorrelated.hpp"
#include "private_settings.hpp"
#include "type_map.hpp"

// Local public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <memory>
#include <string>
#include <typeindex>

namespace panacea {

std::unordered_map<
    settings::KernelCenterCalculation,
    std::unordered_map<
        std::type_index,
        std::unordered_map<std::type_index,
                           KernelWrapperFactory::KernelCreateMethod>>>
    KernelWrapperFactory::create_methods_;

KernelWrapperFactory::KernelWrapperFactory() {

  registerKernel<settings::KernelCenterCalculation::None,
                 std::vector<std::vector<double>> *,
                 std::vector<std::vector<double>>,
                 KernelWrapper<std::vector<std::vector<double>>>>();

  registerKernel<settings::KernelCenterCalculation::None,
                 std::vector<std::vector<double>> *,
                 std::vector<std::vector<double>> *,
                 KernelWrapper<std::vector<std::vector<double>> *>>();

  registerKernel<settings::KernelCenterCalculation::None,
                 std::vector<std::vector<double>>,
                 std::vector<std::vector<double>>,
                 KernelWrapper<std::vector<std::vector<double>>>>();

  registerKernel<settings::KernelCenterCalculation::None, double ***,
                 std::vector<std::vector<double>>,
                 KernelWrapper<std::vector<std::vector<double>>>>();

  registerKernel<settings::KernelCenterCalculation::None, double ***,
                 double ***, KernelWrapper<double ***>>();

  registerKernel<settings::KernelCenterCalculation::Mean, std::vector<double>,
                 std::vector<double>, MeanKernelWrapper>();

  registerKernel<settings::KernelCenterCalculation::Median, std::vector<double>,
                 std::vector<double>, MedianKernelWrapper>();
}

std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
    const BaseDescriptorWrapper &desc_wrapper,
    const KernelSpecification &kern_specification) const {

  // Ensure valid method exists
  if (create_methods_.count(
          kern_specification.get<settings::KernelCenterCalculation>()) == 0) {
    std::string error_msg =
        "Kernel creation method is missing for the specified type: ";
    error_msg += settings::toString(
        kern_specification.get<settings::KernelCenterCalculation>());
    PANACEA_FAIL(error_msg);
  }

  if (kern_specification.is(settings::KernelCount::OneToOne)) {

    if (create_methods_[kern_specification
                            .get<settings::KernelCenterCalculation>()]
            .count(desc_wrapper.getTypeIndex()) == 0) {
      std::string error_msg =
          "Kernel creation method is missing for the specified internal type.";
      PANACEA_FAIL(error_msg);
    }

    if (not kern_specification.is(settings::KernelCenterCalculation::None)) {
      PANACEA_FAIL(
          "Kernel Center Calculation must be None when Count is OneToOne.");
    }
    if (kern_specification.is(settings::KernelMemory::Share) or
        kern_specification.is(settings::KernelMemory::OwnIfRestart)) {

      // This check is only appropriate if memory is shared, the desc_wrapper
      // type and pointer will likley differ in the case of non shared memory
      // because it's likely you will not have a pointer as the underlying type
      if (desc_wrapper.getTypeIndex() !=
          std::type_index(desc_wrapper.getPointerToRawData().type())) {
        std::string error_msg =
            "Descriptor data index and pointerToRawData are not consistent";
        if (type_map.count(desc_wrapper.getTypeIndex())) {
          error_msg += "\ndesc_wrapper type is: " +
                       type_map.at(desc_wrapper.getTypeIndex());
          error_msg += "\n";
        }
        PANACEA_FAIL(error_msg);
      }
      auto desc_data_type_index = desc_wrapper.getTypeIndex();
      auto kern_data_type_index = desc_data_type_index;
      if (desc_data_type_index !=
              std::type_index(typeid(std::vector<std::vector<double>> *)) &&
          desc_data_type_index != std::type_index(typeid(double ***))) {
        PANACEA_FAIL("Unsupported types detected, cannot create kernels.");
      }

      return create_methods_[kern_specification
                                 .get<settings::KernelCenterCalculation>()]
                            [desc_data_type_index][kern_data_type_index](
                                PassKey<KernelWrapperFactory>(), &desc_wrapper,
                                desc_wrapper.rows(), desc_wrapper.cols());
    } else { // If owned

      auto kern_data_type_index =
          std::type_index(typeid(std::vector<std::vector<double>>));
      auto desc_data_type_index = desc_wrapper.getTypeIndex();
      if (desc_data_type_index !=
              std::type_index(typeid(std::vector<std::vector<double>> *)) &&
          desc_data_type_index !=
              std::type_index(typeid(std::vector<std::vector<double>>)) &&
          desc_data_type_index != std::type_index(typeid(double ***))) {
        std::string error_msg =
            "Unsupported types detected, cannot create kernels:\n";
        error_msg += "OneToOne\nOwn\n\n";
        error_msg += "The only supported coversions are from:\n";
        error_msg += "vector<vector<double>>* to vector<vector<double>>\n";
        error_msg += "vector<vector<double>>  to vector<vector<double>>\n";
        error_msg += "double ***              to vector<vector<double>>\n";
        if (type_map.count(desc_data_type_index)) {
          error_msg += "\n";
          error_msg += "The type passed in is identified as " +
                       type_map.at(desc_data_type_index) + "\n";
        }
        PANACEA_FAIL(error_msg);
      }

      return create_methods_[kern_specification
                                 .get<settings::KernelCenterCalculation>()]
                            [desc_data_type_index][kern_data_type_index](
                                PassKey<KernelWrapperFactory>(), &desc_wrapper,
                                desc_wrapper.rows(), desc_wrapper.cols());
    }
  } else if (kern_specification.is(settings::KernelCount::Single)) {
    if (kern_specification.is(settings::KernelMemory::Own)) {

      if (not kern_specification.is(settings::KernelCenterCalculation::Mean) &&
          not kern_specification.is(
              settings::KernelCenterCalculation::Median)) {
        std::string error_msg = "Kernel Center Calculation must be Mean or "
                                "Median when Count is Single: ";
        error_msg += settings::toString(
            kern_specification.get<settings::KernelCenterCalculation>());
        PANACEA_FAIL(error_msg);
      }

      return create_methods_[kern_specification
                                 .get<settings::KernelCenterCalculation>()]
                            [std::type_index(typeid(std::vector<double>))]
                            [std::type_index(typeid(std::vector<double>))](
                                PassKey<KernelWrapperFactory>(), &desc_wrapper,
                                1, desc_wrapper.getNumberDimensions());
    }
  }
  std::string error_msg = "The combination of kernel specifications is not";
  error_msg += " yet supported.\n";

  error_msg += kern_specification.get<std::string>();
  PANACEA_FAIL(error_msg);
  return std::unique_ptr<BaseKernelWrapper>();
}

std::unique_ptr<BaseKernelWrapper> KernelWrapperFactory::create(
    const KernelSpecification &kern_specification) const {

  // Ensure valid method exists
  if (create_methods_.count(
          kern_specification.get<settings::KernelCenterCalculation>()) == 0) {
    std::string error_msg =
        "Kernel creation method is missing for the specified type: ";
    error_msg += settings::toString(
        kern_specification.get<settings::KernelCenterCalculation>());
    PANACEA_FAIL(error_msg);
  }

  if (kern_specification.is(settings::KernelCount::OneToOne)) {

    if (kern_specification.is(settings::KernelMemory::Share)) {
      std::string error_msg =
          "OneToOne kernels that are not owned, are not yet supported. ";
      error_msg += "When constructed without a descriptor wrapper.\n";
      PANACEA_FAIL(error_msg);
    } else {
      auto data_type_index =
          std::type_index(typeid(std::vector<std::vector<double>>));
      // Initialize with an empty vector of vectors
      std::vector<std::vector<double>> data;
      return create_methods_[kern_specification
                                 .get<settings::KernelCenterCalculation>()]
                            [data_type_index][data_type_index](
                                PassKey<KernelWrapperFactory>(), data, 0, 0);
    }
  } else if (kern_specification.is(settings::KernelCount::Single)) {
    if (kern_specification.is(settings::KernelMemory::Own) or
        kern_specification.is(settings::KernelMemory::OwnIfRestart)) {

      if (not kern_specification.is(settings::KernelCenterCalculation::Mean) &&
          not kern_specification.is(
              settings::KernelCenterCalculation::Median)) {
        std::string error_msg = "Kernel Center Calculation must be Mean or "
                                "Median when Count is Single: ";
        error_msg += settings::toString(
            kern_specification.get<settings::KernelCenterCalculation>());
        PANACEA_FAIL(error_msg);
      }
      std::vector<double> data;
      return create_methods_[kern_specification
                                 .get<settings::KernelCenterCalculation>()]
                            [std::type_index(typeid(std::vector<double>))]
                            [std::type_index(typeid(std::vector<double>))](
                                PassKey<KernelWrapperFactory>(), data, 0, 0);
    }
  }
  std::string error_msg = "The combination of kernel specifications is not";
  error_msg += " yet supported.\n";

  error_msg += kern_specification.get<std::string>();
  PANACEA_FAIL(error_msg);
  return std::unique_ptr<BaseKernelWrapper>();
}
} // namespace panacea
