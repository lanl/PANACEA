
#ifndef PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
#define PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
#pragma once

// Local private PANACEA includes
#include "base_kernel_wrapper.hpp"

#include "data_point_template.hpp"
#include "passkey.hpp"

// Standard includes
#include <any>
#include <cstddef>
#include <typeindex>

namespace panacea {

  class BaseDescriptorWrapper;
  class KernelWrapperFactory;

  namespace test {
    class Test;
  }

  class MedianKernelWrapper : public BaseKernelWrapper {

    private:
      DataPointTemplate<std::vector<double>> data_wrapper_;
    public:

      MedianKernelWrapper(
          const PassKey<KernelWrapperFactory> &,
          BaseDescriptorWrapper * desc_wrapper
          );

      MedianKernelWrapper(
          const PassKey<test::Test> &,
          BaseDescriptorWrapper * desc_wrapper
          );

      virtual double& operator()(const int point_ind, const int dim_ind) final;
      virtual double operator()(const int point_ind, const int dim_ind) const final;
      virtual int rows() const final;
      virtual int cols() const final;
      virtual int getNumberDimensions() const final;
      virtual int getNumberPoints() const final;
      virtual void set(const Arrangement arrangement) final;
      virtual const std::any getPointerToRawData() const noexcept final;
      virtual std::type_index getTypeIndex() const noexcept final;
      virtual void print() const final;
      // Standard any should not be a reference because the underlying type should
      // be a pointer
      static std::unique_ptr<BaseKernelWrapper> create(
          const PassKey<KernelWrapperFactory> &, 
          std::any data, 
          const int rows,
          const int cols);
  };

  inline std::unique_ptr<BaseKernelWrapper> MedianKernelWrapper::create(
      const PassKey<KernelWrapperFactory> & key,
      std::any data,
      const int rows,
      const int cols) {

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return std::make_unique<MedianKernelWrapper>(key, std::any_cast<BaseDescriptorWrapper *>(data)); 
  }

}
#endif // PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
