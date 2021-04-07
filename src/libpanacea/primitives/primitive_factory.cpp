
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

// Standard includes
#include <memory>
#include <unordered_map>
#include <vector>

namespace panacea {

  std::unordered_map<settings::KernelPrimitive,
        std::unordered_map<settings::KernelCorrelation,
        PrimitiveFactory::PrimitiveCreateMethod>>
          PrimitiveFactory::create_methods_;

  std::vector<std::unique_ptr<Primitive>> PrimitiveFactory::create(
      BaseDescriptorWrapper * dwrapper,
      MemoryManager & mem_manager,
      const KernelSpecification & specification) const {

    KernelWrapperFactory kfactory;
    auto kwrapper = kfactory.create(dwrapper, specification, mem_manager);

    Reducer reducer;
    Inverter inverter;

    PrimitiveAttributes attr;
    attr.kernel_wrapper = kwrapper.get();
    attr.covariance = std::make_unique<Covariance>(dwrapper);
    attr.reduced_covariance = std::make_unique<ReducedCovariance>(reducer.reduce(*attr.covariance, std::vector<int> {}));
    attr.reduced_inv_covariance = std::make_unique<ReducedInvCovariance>(inverter.invert(*attr.reduced_covariance));

    if(specification.get<settings::KernelPrimitive>() != settings::KernelPrimitive::Gaussian){
      std::string error_msg = "Currently, only gaussian kernels are supported";
      PANACEA_FAIL(error_msg);
    }

    std::vector<std::unique_ptr<Primitive>> primitives;
    if( specification.is(settings::KernelCount::OneToOne)){
      primitives.reserve(kwrapper->getNumberPoints());
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      for( int kernel_index = 0; kernel_index < kwrapper->getNumberPoints(); ++kernel_index){
        std::cout << "Creating kernel with index " << kernel_index << std::endl;
        primitives.push_back(
          create_methods_[specification.get<settings::KernelPrimitive>()]\
        [specification.get<settings::KernelCorrelation>()]\
        (PassKey<PrimitiveFactory>(), std::move(attr), kernel_index));
        std::cout << "Id is " << primitives.at(kernel_index)->getId() << std::endl;
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      
    } else {
      const int kernel_index = 0;
      primitives.emplace_back(
          create_methods_[specification.get<settings::KernelPrimitive>()]\
        [specification.get<settings::KernelCorrelation>()]\
        (PassKey<PrimitiveFactory>(), std::move(attr), kernel_index));
    }
  
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    mem_manager.manageMemory(std::move(kwrapper),specification.get<std::string>());
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return primitives;
  }
}
