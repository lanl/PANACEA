
#ifndef PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
#define PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
#pragma once

// Local private PANACEA includes
#include "base_kernel_wrapper.hpp"

#include "data_point_template.hpp"
#include "error.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <cstddef>
#include <deque>
#include <memory>
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
  std::vector<std::deque<double>> points_near_median_;

  /**
   * Will alternate if an odd number of points need to be remove
   * alternate between removing an extra element from the front and back
   **/
  bool remove_from_front_ = true;
  int number_pts_median_; // Number of points used to calculate the median

  virtual BaseKernelWrapper::ReadFunction getReadFunction_() final;
  virtual BaseKernelWrapper::WriteFunction getWriteFunction_() const final;

  /**
   * Private constructors
   **/
  explicit MedianKernelWrapper(const BaseDescriptorWrapper &desc_wrapper);
  explicit MedianKernelWrapper(const std::vector<double> &);

public:
  explicit MedianKernelWrapper(const PassKey<test::Test> &){};

  MedianKernelWrapper(const PassKey<KernelWrapperFactory> &,
                      const BaseDescriptorWrapper &desc_wrapper)
      : MedianKernelWrapper(desc_wrapper){};

  MedianKernelWrapper(const PassKey<test::Test> &,
                      const BaseDescriptorWrapper &desc_wrapper)
      : MedianKernelWrapper(desc_wrapper){};

  /**
   * If initialized with a single vector the median is assumed to be the
   * vector passed in.
   **/
  MedianKernelWrapper(const PassKey<KernelWrapperFactory> &,
                      const std::vector<double> &data)
      : MedianKernelWrapper(data){};

  MedianKernelWrapper(const PassKey<test::Test> &,
                      const std::vector<double> &data)
      : MedianKernelWrapper(data){};

  virtual const settings::KernelCenterCalculation center() const noexcept final;
  virtual const settings::KernelCount count() const noexcept final;
  virtual double &at(const int row, const int col) final;
  virtual double at(const int row, const int col) const final;
  virtual void resize(const int rows, const int cols) final;
  virtual int rows() const final;
  virtual int cols() const final;
  virtual int getNumberDimensions() const final;
  virtual int getNumberPoints() const final;
  virtual const Arrangement &arrangement() const noexcept final;
  virtual void set(const Arrangement arrangement) final;
  virtual void update(const BaseDescriptorWrapper &) final;
  virtual const std::any getPointerToRawData() const noexcept final;
  virtual std::type_index getTypeIndex() const noexcept final;
  virtual void print() const final;
  // Standard any should not be a reference because the underlying type should
  // be a pointer
  static std::unique_ptr<BaseKernelWrapper>
  create(const PassKey<KernelWrapperFactory> &, std::any data, const int rows,
         const int cols);

  static std::istream &read(BaseKernelWrapper &, std::istream &);
  static std::ostream &write(const BaseKernelWrapper &, std::ostream &);
};

inline std::unique_ptr<BaseKernelWrapper>
MedianKernelWrapper::create(const PassKey<KernelWrapperFactory> &key,
                            std::any data, const int rows, const int cols) {

  if (std::type_index(data.type()) ==
      std::type_index(typeid(const BaseDescriptorWrapper *))) {
    return std::make_unique<MedianKernelWrapper>(
        key, *std::any_cast<const BaseDescriptorWrapper *>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(BaseDescriptorWrapper *))) {
    return std::make_unique<MedianKernelWrapper>(
        key, *const_cast<const BaseDescriptorWrapper *>(
                 std::any_cast<BaseDescriptorWrapper *>(data)));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(const BaseDescriptorWrapper &))) {
    return std::make_unique<MedianKernelWrapper>(
        key, std::any_cast<const BaseDescriptorWrapper &>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(BaseDescriptorWrapper &))) {
    return std::make_unique<MedianKernelWrapper>(
        key, const_cast<const BaseDescriptorWrapper &>(
                 std::any_cast<BaseDescriptorWrapper &>(data)));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(const std::vector<double>))) {
    return std::make_unique<MedianKernelWrapper>(
        key, std::any_cast<const std::vector<double>>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(std::vector<double>))) {
    return std::make_unique<MedianKernelWrapper>(
        key, std::any_cast<std::vector<double>>(data));

  } else {
    std::string error_msg = "Unsupported data type encountered while ";
    error_msg += "attempting to create Kernel center median";
    PANACEA_FAIL(error_msg);
  }
  return nullptr;
}

} // namespace panacea
#endif // PANACEA_PRIVATE_MEDIANKERNELWRAPPER_H
