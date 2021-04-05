
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
#include "primitives/primitive.hpp"
#include "primitive_attributes.hpp"

namespace panacea {

  std::unordered_map<settings::KernelPrimitive,
        std::unordered_map<settings::KernelCorrelation,
        PrimitiveFactory::PrimitiveCreateMethod>>
          PrimitiveFactory::create_methods_;

  std::unique_ptr<Primitive> PrimitiveFactory::create(
      BaseDescriptorWrapper * dwrapper,
      MemoryManager & mem_manager,
      const KernelSpecification & specification) const {

    Covariance cov(dwrapper);

    Reducer reducer;
    ReducedCovariance reduced_cov = reducer.reduce(cov, std::vector<int> {});

    Inverter inverter;
    ReducedInvCovariance reduced_inv_cov = inverter.invert(reduced_cov);

    KernelWrapperFactory kfactory;
    auto kwrapper = kfactory.create(dwrapper, specification, mem_manager);

    PrimitiveAttributes attr;
    attr.kernel_wrapper = kwrapper.get();
    attr.reduced_covariance = &reduced_cov;
    attr.reduced_inv_covariance = &reduced_inv_cov;

    if(specification.get<settings::KernelPrimitive>() != settings::KernelPrimitive::Gaussian){
      std::string error_msg = "Currently, only gaussian kernels are supported";
      PANACEA_FAIL(error_msg);
    }

    int index = 0;
    return create_methods_[specification.get<settings::KernelPrimitive>()]\
      [specification.get<settings::KernelCorrelation>()]\
      (PassKey<PrimitiveFactory>(), attr, index);
  }
}
