
#ifndef PANACEA_PRIVATE_MEANKERNELWRAPPER_H
#define PANACEA_PRIVATE_MEANKERNELWRAPPER_H
#pragma once

// Local private PANACEA includes
#include "base_kernel_wrapper.hpp"

#include "data_point_template.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <cstddef>
#include <memory>
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
  virtual BaseKernelWrapper::WriteFunction getWriteFunction_() const final;

  explicit MeanKernelWrapper(const BaseDescriptorWrapper &desc_wrapper);
  explicit MeanKernelWrapper(const std::vector<double> &);

public:
  explicit MeanKernelWrapper(const PassKey<test::Test> &){};

  MeanKernelWrapper(const PassKey<KernelWrapperFactory> &,
                    const BaseDescriptorWrapper &desc_wrapper)
      : MeanKernelWrapper(desc_wrapper){};

  MeanKernelWrapper(const PassKey<test::Test> &,
                    const BaseDescriptorWrapper &desc_wrapper)
      : MeanKernelWrapper(desc_wrapper){};

  /**
   * If a vector is supplied it is assumed to be the mean
   **/
  MeanKernelWrapper(const PassKey<KernelWrapperFactory> &,
                    const std::vector<double> &data)
      : MeanKernelWrapper(data){};

  MeanKernelWrapper(const PassKey<test::Test> &,
                    const std::vector<double> &data)
      : MeanKernelWrapper(data){};

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
MeanKernelWrapper::create(const PassKey<KernelWrapperFactory> &key,
                          std::any data, const int rows, const int cols) {

  if (std::type_index(data.type()) ==
      std::type_index(typeid(const BaseDescriptorWrapper *))) {
    return std::make_unique<MeanKernelWrapper>(
        key, *std::any_cast<const BaseDescriptorWrapper *>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(BaseDescriptorWrapper *))) {
    return std::make_unique<MeanKernelWrapper>(
        key, const_cast<const BaseDescriptorWrapper &>(
                 *std::any_cast<BaseDescriptorWrapper *>(data)));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(BaseDescriptorWrapper &))) {
    return std::make_unique<MeanKernelWrapper>(
        key, const_cast<const BaseDescriptorWrapper &>(
                 std::any_cast<BaseDescriptorWrapper &>(data)));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(const BaseDescriptorWrapper &))) {
    return std::make_unique<MeanKernelWrapper>(
        key, std::any_cast<const BaseDescriptorWrapper &>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(std::vector<double>))) {
    return std::make_unique<MeanKernelWrapper>(
        key, std::any_cast<std::vector<double>>(data));

  } else if (std::type_index(data.type()) ==
             std::type_index(typeid(const std::vector<double>))) {
    return std::make_unique<MeanKernelWrapper>(
        key, std::any_cast<const std::vector<double>>(data));

  } else {
    std::string error_msg = "Unsupported data type encountered while ";
    error_msg += "attempting to create Kernel center mean";
    throw std::runtime_error(error_msg);
  }
  return nullptr;
}
} // namespace panacea
#endif // PANACEA_PRIVATE_MEANKERNELWRAPPER_H
