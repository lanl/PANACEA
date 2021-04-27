
// Local private includes
#include "primitive_factory.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/inverter.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "descriptors/base_descriptor_wrapper.hpp"
#include "error.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "kernels/mean_kernel_wrapper.hpp"
#include "kernels/median_kernel_wrapper.hpp"
#include "memory.hpp"
#include "normalization_methods/normalization_method_factory.hpp"
#include "normalization_methods/normalization_method_none.hpp"
#include "normalization_methods/normalization_method_variance.hpp"
#include "primitives/primitive.hpp"
#include "primitive_attributes.hpp"
#include "primitive_group.hpp"

// Standard includes
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  /*************************************************
   * Declaring Static Private Member Methods
   *************************************************/

  void PrimitiveFactory::OneToOne(const PassKey<PrimitiveFactory> &,
      PrimitiveGroup & prim_grp,
      const KernelSpecification & specification
      ){

    prim_grp.primitives.reserve(prim_grp.kernel_wrapper->getNumberPoints());
    for( int kernel_index = 0;
        kernel_index < prim_grp.kernel_wrapper->getNumberPoints();
        ++kernel_index){

      prim_grp.primitives.push_back(
          create_methods_[specification.get<settings::KernelPrimitive>()]\
          [specification.get<settings::KernelCorrelation>()]\
          (PassKey<PrimitiveFactory>(),
           std::move(prim_grp.createPrimitiveAttributes()),
           kernel_index));
    }
  }

  void PrimitiveFactory::Single(const PassKey<PrimitiveFactory> &,
      PrimitiveGroup & prim_grp,
      const KernelSpecification & specification
      ){

    const int kernel_index = 0;
    prim_grp.primitives.emplace_back(
        create_methods_[specification.get<settings::KernelPrimitive>()]\
        [specification.get<settings::KernelCorrelation>()]\
        (PassKey<PrimitiveFactory>(),
         std::move(prim_grp.createPrimitiveAttributes()),
         kernel_index));
  }

  /***************************************************************
   * Declaring private Member function maps
   **************************************************************/

  std::unordered_map<settings::KernelPrimitive,
    std::unordered_map<settings::KernelCorrelation,
    PrimitiveFactory::PrimitiveCreateMethod>>
      PrimitiveFactory::create_methods_;

  std::unordered_map<settings::KernelCount,
    PrimitiveFactory::PrimitiveCountMethod>
      PrimitiveFactory::count_methods_{
        {settings::KernelCount::OneToOne, PrimitiveFactory::OneToOne},
        {settings::KernelCount::Single, PrimitiveFactory::Single}
      };

  /***************************************************
   * File scope static functions
   ***************************************************/

  static Normalizer createNormalizer(
      const BaseDescriptorWrapper * dwrapper,
      const KernelSpecification & specification) {

    if( specification.is(settings::KernelAlgorithm::Strict) ) {
      return Normalizer(dwrapper, NormalizerOption::Strict);
    }
    // Flexible option will avoid errors if coefficients are 0.0, 
    // e.g. if variance is 0.0, will set such coefficients to 1.0
    return Normalizer(dwrapper, NormalizerOption::Flexible);
  }

  static std::unique_ptr<Covariance> createCovariance(
      const BaseDescriptorWrapper * dwrapper,
      const KernelSpecification & specification) {

    if( specification.is(settings::KernelAlgorithm::Strict) ) {
      return std::make_unique<Covariance>(dwrapper, CovarianceOption::Strict);
    }
    return std::make_unique<Covariance>(dwrapper, CovarianceOption::Flexible);
  }

  /***********************************************************
   * Declaring public methods
   ***********************************************************/

  PrimitiveFactory::PrimitiveFactory() {
    
    registerPrimitive<GaussUncorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Uncorrelated>();

    registerPrimitive<GaussCorrelated,
      settings::KernelPrimitive::Gaussian,
      settings::KernelCorrelation::Correlated>();

  }

  PrimitiveGroup PrimitiveFactory::create(
      const BaseDescriptorWrapper * dwrapper,
      MemoryManager & mem_manager,
      const KernelSpecification & specification,
      std::string name) const {

    KernelWrapperFactory kfactory;

/*    kfactory.registerKernel<settings::KernelCenterCalculation::None,
      std::vector<std::vector<double>>*,
      KernelWrapper<std::vector<std::vector<double>>*>>();

    kfactory.registerKernel<settings::KernelCenterCalculation::Mean,
      std::vector<double>,
      MeanKernelWrapper>();

    kfactory.registerKernel<settings::KernelCenterCalculation::Median,
      std::vector<double>,
      MedianKernelWrapper>();*/

    auto kwrapper = kfactory.create(dwrapper, specification, mem_manager, name);

    Reducer reducer;
    Inverter inverter;
  
    PrimitiveGroup prim_grp;
    prim_grp.kernel_wrapper = kwrapper.get();

    prim_grp.covariance = createCovariance(dwrapper, specification);

    prim_grp.normalizer = createNormalizer(dwrapper, specification);
    prim_grp.normalizer.normalize(*prim_grp.covariance); 

    prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(
        reducer.reduce(*prim_grp.covariance, std::vector<int> {}));
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(
        inverter.invert(*prim_grp.reduced_covariance));

    // Update the dwrapper so that it has an uptodate account of the reduced dimensions
    // I don't like having this in here!
//    dwrapper->setReducedNumberDimensions(prim_grp.reduced_covariance->getChosenDimensionIndices());

    if(create_methods_.count(specification.get<settings::KernelPrimitive>()) == 0){
      std::string error_msg = "Kernel Primitive is not supported: ";
      error_msg += settings::toString(specification.get<settings::KernelPrimitive>());
      PANACEA_FAIL(error_msg);
    }

    if( count_methods_.count(specification.get<settings::KernelCount>()) == 0){
      std::string error_msg = "Kernel count method is not supported: ";
      error_msg += settings::toString(specification.get<settings::KernelCount>());
      PANACEA_FAIL(error_msg);
    }

    count_methods_[specification.get<settings::KernelCount>()](PassKey<PrimitiveFactory>(),prim_grp,specification);

    // Assign a default name to the primitive
    if( name.empty() ) {
      name = specification.get<std::string>();
    }

    name = "KernelWrapper: " + name;
    mem_manager.manageMemory(std::move(kwrapper),name);
    prim_grp.name = name;

    return prim_grp;
  }
}
