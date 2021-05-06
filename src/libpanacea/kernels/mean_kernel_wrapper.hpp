
#ifndef PANACEA_PRIVATE_MEANKERNELWRAPPER_H
#define PANACEA_PRIVATE_MEANKERNELWRAPPER_H
#pragma once

// Local private PANACEA includes
#include "base_kernel_wrapper.hpp"

#include "data_point_template.hpp"
#include "passkey.hpp"

// Standard includes
#include <any>
#include <cstddef>
#include <typeindex>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class KernelWrapperFactory;

  namespace test {
    class Test;
  }

  class MeanKernelWrapper : public BaseKernelWrapper {

    private:
      DataPointTemplate<std::vector<double>> data_wrapper_;
      int number_pts_mean_; // Number of points used to calculate the mean

      virtual BaseKernelWrapper::ReadFunction getReadFunction_() final;
      virtual BaseKernelWrapper::WriteFunction getWriteFunction_() final;
    public:
      explicit MeanKernelWrapper(const PassKey<test::Test> &) {};

      MeanKernelWrapper(
          const PassKey<KernelWrapperFactory> &,
          const BaseDescriptorWrapper * desc_wrapper
          );

      MeanKernelWrapper(
          const PassKey<test::Test> &,
          const BaseDescriptorWrapper * desc_wrapper
          );

      virtual const settings::KernelCenterCalculation center() const noexcept final;
      virtual const settings::KernelCount count() const noexcept final;
      virtual double& at(const int row, const int col) final;
      virtual double at(const int row, const int col) const final;
      virtual void resize(const int rows, const int cols) final;
      virtual int rows() const final;
      virtual int cols() const final;
      virtual int getNumberDimensions() const final;
      virtual int getNumberPoints() const final;
      virtual const Arrangement & arrangement() const noexcept final; 
      virtual void set(const Arrangement arrangement) final;
      virtual void update(const BaseDescriptorWrapper *) final;
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

      static std::istream & read(BaseKernelWrapper *, std::istream &);
      static std::ostream & write(BaseKernelWrapper *, std::ostream &);
  };

  inline std::unique_ptr<BaseKernelWrapper> MeanKernelWrapper::create(
      const PassKey<KernelWrapperFactory> & key,
      std::any data,
      const int rows,
      const int cols) {

    return std::make_unique<MeanKernelWrapper>(key, std::any_cast<const BaseDescriptorWrapper *>(data)); 
  }

}
#endif // PANACEA_PRIVATE_MEANKERNELWRAPPER_H
