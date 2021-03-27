#ifndef PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
#define PANACEA_PRIVATE_KERNELWRAPPER_FACTORY

// Local private includes
#include "base_kernel_wrapper.hpp"
#include "descriptors/base_descriptor_wrapper"

// Standard includes
#include <memory>

namespace panacea {

  class KernelWrapperFactory {

    std::unique_ptr<BaseKernelWrapper> create(
        BaseDescriptorWrapper * desc_wrapper,
        const KernelSpecification & kern_specification ) const;
  };

}

#endif // PANACEA_PRIVATE_KERNELWRAPPER_FACTORY
