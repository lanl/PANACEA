
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
#include "memory.hpp"
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

  /***********************************************************
   * Declaring public methods
   ***********************************************************/

  PrimitiveGroup PrimitiveFactory::create(
      BaseDescriptorWrapper * dwrapper,
      MemoryManager & mem_manager,
      const KernelSpecification & specification) const {

    KernelWrapperFactory kfactory;
    auto kwrapper = kfactory.create(dwrapper, specification, mem_manager);

    Reducer reducer;
    Inverter inverter;

    PrimitiveGroup prim_grp;
    prim_grp.kernel_wrapper = kwrapper.get();
    prim_grp.covariance = std::make_unique<Covariance>(dwrapper);
    prim_grp.reduced_covariance = std::make_unique<ReducedCovariance>(reducer.reduce(*prim_grp.covariance, std::vector<int> {}));
    prim_grp.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(inverter.invert(*prim_grp.reduced_covariance));

    // Update the dwrapper so that it has an uptodate account of the reduced dimensions
    dwrapper->setReducedNumberDimensions(prim_grp.reduced_covariance->getChosenDimensionIndices());

    if(create_methods_.count(pecification.get<settings::KernelPrimitive>()) == 0){
      std::string error_msg = "Kernel Primitive is not supported";
      PANACEA_FAIL(error_msg);
    }

    if( count_methods_.count(specification.get<settings::KernelCount>()) == 0){
      std::string error_msg = "Kernel count method is not supported.";
      PANACEA_FAIL(error_msg);
    }

    count_methods_[specification.get<settings::KernelCount>()](PassKey<PrimitiveFactory>(),prim_grp,specification);

    mem_manager.manageMemory(std::move(kwrapper),specification.get<std::string>());
    return prim_grp;
  }
}
