
#ifndef PANACEA_PRIVATE_KERNELWRAPPER_H
#define PANACEA_PRIVATE_KERNELWRAPPER_H
#pragma once

// Local private includes
#include "base_kernel_wrapper.hpp"
#include "data_point_template.hpp"
#include "private_settings.hpp"
#include "type_map.hpp"

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io_types.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <memory>
#include <typeindex>

namespace panacea {

class KernelWrapperFactory;

namespace test {
class Test;
}

/*
 * The kernel wrapper should be flexible enough to allow
 * the underlying type to vary
 */
template <class T> class KernelWrapper : public BaseKernelWrapper {
private:
  DataPointTemplate<T> data_wrapper_;

  virtual BaseKernelWrapper::ReadFunction getReadFunction_() final;
  virtual BaseKernelWrapper::WriteFunction getWriteFunction_() const final;

public:
  KernelWrapper(const PassKey<test::Test> &){};

  KernelWrapper(const PassKey<KernelWrapperFactory> &, const T &data, int rows,
                int cols)
      : data_wrapper_(data, rows, cols){};

  KernelWrapper(const PassKey<KernelWrapperFactory> &, const int rows,
                const int cols)
      : data_wrapper_(rows, cols){};
  /**
   * Will copy the data instead of storing it as a pointer.
   **/
  KernelWrapper(const PassKey<KernelWrapperFactory> &, T *data, int rows,
                int cols)
      : data_wrapper_(*data, rows, cols){};

  KernelWrapper(const PassKey<KernelWrapperFactory> &, const T *data, int rows,
                int cols)
      : data_wrapper_(*data, rows, cols){};

  KernelWrapper(const PassKey<test::Test> &, const T &data, int rows, int cols)
      : data_wrapper_(data, rows, cols){};

  /**
   * Will copy the data instead of storing it as a pointer.
   **/
  KernelWrapper(const PassKey<test::Test> &, T *data, int rows, int cols)
      : data_wrapper_(*data, rows, cols){};

  KernelWrapper(const PassKey<test::Test> &, const T *data, int rows, int cols)
      : data_wrapper_(*data, rows, cols){};

  KernelWrapper(const PassKey<test::Test> &,
                const BaseDescriptorWrapper &desc_wrapper);

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

  /**************************************
   * Static class funcitons
   **************************************/
  // Standard any should not be a reference because the underlying type should
  // be a pointer
  static std::unique_ptr<BaseKernelWrapper>
  create(const PassKey<KernelWrapperFactory> &, std::any data, const int rows,
         const int cols);

  static std::istream &read(BaseKernelWrapper &, std::istream &);
  static std::ostream &write(const BaseKernelWrapper &, std::ostream &);
};

template <class T>
KernelWrapper<T>::KernelWrapper(const PassKey<test::Test> &,
                                const BaseDescriptorWrapper &dwrapper) {

  if constexpr (std::is_pointer<T>::value) {
    data_wrapper_ =
        DataPointTemplate<T>(std::any_cast<T>(dwrapper.getPointerToRawData()),
                             dwrapper.rows(), dwrapper.cols());
  } else {
    // Because dwrapper is const
    data_wrapper_ = DataPointTemplate<T>(
        *(std::any_cast<const T *>(dwrapper.getPointerToRawData())),
        dwrapper.rows(), dwrapper.cols());
  }
}

template <class T>
const settings::KernelCenterCalculation KernelWrapper<T>::center() const
    noexcept {
  return settings::KernelCenterCalculation::None;
}

template <class T>
const settings::KernelCount KernelWrapper<T>::count() const noexcept {
  return settings::KernelCount::OneToOne;
}

template <class T>
inline double &KernelWrapper<T>::at(const int row, const int col) {
  return data_wrapper_.at(row, col);
}

template <class T>
inline double KernelWrapper<T>::at(const int row, const int col) const {
  return data_wrapper_.at(row, col);
}

template <class T>
inline void KernelWrapper<T>::resize(const int rows, const int cols) {
  data_wrapper_.resize(rows, cols);
}

template <class T> inline int KernelWrapper<T>::rows() const {
  return data_wrapper_.rows();
}

template <class T> inline int KernelWrapper<T>::cols() const {
  return data_wrapper_.cols();
}

template <class T> inline int KernelWrapper<T>::getNumberDimensions() const {
  return data_wrapper_.getNumberDimensions();
}

template <class T> inline int KernelWrapper<T>::getNumberPoints() const {
  return data_wrapper_.getNumberPoints();
}

template <class T>
inline const Arrangement &KernelWrapper<T>::arrangement() const noexcept {
  return data_wrapper_.arrangement();
}

template <class T>
inline void KernelWrapper<T>::set(const Arrangement arrangement) {
  data_wrapper_.set(arrangement);
}

template <class T>
inline void KernelWrapper<T>::update(const BaseDescriptorWrapper &dwrapper) {
  data_wrapper_ =
      DataPointTemplate<T>(std::any_cast<T>(dwrapper.getPointerToRawData()),
                           dwrapper.rows(), dwrapper.cols());
}

template <class T>
inline const std::any KernelWrapper<T>::getPointerToRawData() const noexcept {
  return data_wrapper_.getPointerToRawData();
}

template <class T> inline void KernelWrapper<T>::print() const {
  data_wrapper_.print();
}

template <class T>
inline std::unique_ptr<BaseKernelWrapper>
KernelWrapper<T>::create(const PassKey<KernelWrapperFactory> &key,
                         std::any data_in, const int rows, const int cols) {

  std::any data;
  if (std::type_index(typeid(const BaseDescriptorWrapper *)) ==
      std::type_index(data_in.type())) {

    data = std::any_cast<const BaseDescriptorWrapper *>(data_in)
               ->getPointerToRawData();
  } else {
    data = data_in;
  }

  if (std::type_index(data.type()) == std::type_index(typeid(T *))) {
    if (not std::is_pointer<T>::value) {
      // Allows conversion from a pointer type to a non pointer type
      // That way the kernel will have ownership of the data
      return std::make_unique<KernelWrapper<T>>(key, std::any_cast<T *>(data),
                                                rows, cols);
    } else {
      return std::make_unique<KernelWrapper<T>>(
          key, *(std::any_cast<T *>(data)), rows, cols);
    }
  }

  if (std::type_index(data.type()) == std::type_index(typeid(const T *))) {
    if (not std::is_pointer<T>::value) {
      // Allows conversion from a pointer type to a non pointer type
      // That way the kernel will have ownership of the data
      return std::make_unique<KernelWrapper<T>>(
          key, std::any_cast<const T *>(data), rows, cols);
    } else {
      return std::make_unique<KernelWrapper<T>>(
          key, *(std::any_cast<const T *>(data)), rows, cols);
    }
  }

  if (std::type_index(data.type()) == std::type_index(typeid(T))) {
    return std::make_unique<KernelWrapper<T>>(key, std::any_cast<T>(data), rows,
                                              cols);
  }

  // In the case that the underlying data is not the same. E.g. if we have a
  // pointer e.g. double ***  but we want to store the data as a vector, then
  // what we need to do is make a copy of the data
  if (not std::is_pointer<T>::value) {
    // We can only do this if the data is passed in as a BaseDescriptorWrapper
    // otherwise we do not know the interface for getting the data out
    if (std::type_index(typeid(const BaseDescriptorWrapper *)) ==
        std::type_index(data_in.type())) {
      auto kwrapper = std::make_unique<KernelWrapper<T>>(key, rows, cols);
      auto base_desc_wrapper =
          std::any_cast<const BaseDescriptorWrapper *>(data_in);
      for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
          if (base_desc_wrapper->arrangement() ==
              Arrangement::PointsAlongRowsDimensionsAlongCols) {
            kwrapper->at(row, col) = base_desc_wrapper->operator()(row, col);
          } else {
            kwrapper->at(row, col) = base_desc_wrapper->operator()(col, row);
          }
        }
      }
      return kwrapper;
    } // if data_in is a BaseDescWrapper
  }   // if T is not a pointer

  return nullptr;
}

template <class T>
inline std::istream &KernelWrapper<T>::read(BaseKernelWrapper &,
                                            std::istream &is) {
  return is;
}

template <class T>
inline std::ostream &KernelWrapper<T>::write(const BaseKernelWrapper &,
                                             std::ostream &os) {
  return os;
}

template <class T>
inline std::type_index KernelWrapper<T>::getTypeIndex() const noexcept {
  return std::type_index(typeid(typename std::remove_const<T>::type));
}

template <class T>
inline BaseKernelWrapper::ReadFunction KernelWrapper<T>::getReadFunction_() {
  return KernelWrapper<T>::read;
}

template <class T>
inline BaseKernelWrapper::WriteFunction
KernelWrapper<T>::getWriteFunction_() const {
  return KernelWrapper<T>::write;
}

} // namespace panacea
#endif // PANACEA_PRIVATE_KERNELWRAPPER_H
