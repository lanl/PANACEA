
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
#include <deque>
#include <typeindex>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class KernelWrapperFactory;

  namespace test {
    class Test;
  }

  class MedianKernelWrapper : public BaseKernelWrapper {

    private:
      DataPointTemplate<std::vector<double>> data_wrapper_;

      /**
       * Number of points to track to watch to keep an up to date median
       **/
      int number_pts_store_ = 100;
      std::vector<std::deque<double>>  points_near_median_;

      /**
       * Will alternate if an odd number of points need to be remove
       * alternate between removing an extra element from the front and back
       **/
      bool remove_from_front_ = true;
      int number_pts_median_; // Number of points used to calculate the median
    public:

      MedianKernelWrapper(
          const PassKey<KernelWrapperFactory> &,
          const BaseDescriptorWrapper * desc_wrapper
          );

      MedianKernelWrapper(
          const PassKey<test::Test> &,
          const BaseDescriptorWrapper * desc_wrapper
          );

      virtual double& at(const int row, const int col) final;
      virtual double at(const int row, const int col) const final;
      virtual int rows() const final;
      virtual int cols() const final;
      virtual int getNumberDimensions() const final;
      virtual int getNumberPoints() const final;
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
  };

  inline std::unique_ptr<BaseKernelWrapper> MedianKernelWrapper::create(
      const PassKey<KernelWrapperFactory> & key,
      std::any data,
      const int rows,
      const int cols) {

    return std::make_unique<MedianKernelWrapper>(key, std::any_cast<const BaseDescriptorWrapper *>(data)); 
  }

}
#endif // PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
