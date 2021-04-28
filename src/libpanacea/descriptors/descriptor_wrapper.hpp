
#ifndef PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
#define PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
#pragma once

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private includes
#include "data_point_template.hpp"
#include "error.hpp"

// Standard includes
#include <any>
#include <vector>
#include <typeindex>

namespace panacea {
  /*
   * The descriptor wrapper should be flexible enough to allow
   * the underlying type to vary
   */
  template<class T>
  class DescriptorWrapper : public BaseDescriptorWrapper {
    private: 
      DataPointTemplate<T> data_wrapper_;
    public:
      DescriptorWrapper(T data, int rows, int cols) : 
        data_wrapper_(data, rows, cols) {};

      typedef const T type;

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
  };

  template<class T>
  inline double& DescriptorWrapper<T>::operator()(const int point_ind, const int dim_ind) {
    return data_wrapper_(point_ind, dim_ind);
  }

  template<class T>
  inline double DescriptorWrapper<T>::operator()(const int point_ind, const int dim_ind) const {
    return data_wrapper_(point_ind, dim_ind);
  }

  template<class T>
  inline int DescriptorWrapper<T>::rows() const { return data_wrapper_.rows(); }

  template<class T>
  inline int DescriptorWrapper<T>::cols() const { return data_wrapper_.cols(); }

  template<class T>
  inline int DescriptorWrapper<T>::getNumberDimensions() const { return data_wrapper_.getNumberDimensions(); }

  template<class T>
  inline int DescriptorWrapper<T>::getNumberPoints() const { return data_wrapper_.getNumberPoints(); }

  template<class T>
  inline void DescriptorWrapper<T>::set(const Arrangement arrangement) {
    data_wrapper_.set(arrangement);
  }

  template<class T>
  inline const std::any DescriptorWrapper<T>::getPointerToRawData() const noexcept {
    return data_wrapper_.getPointerToRawData();
  }

  template<class T>
  inline void DescriptorWrapper<T>::print() const {
    data_wrapper_.print();
  }

  template<class T>
  inline std::type_index DescriptorWrapper<T>::getTypeIndex() const noexcept {
    return typeid(T);
  }

}
#endif // PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
