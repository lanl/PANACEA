
#ifndef PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
#define PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
#pragma once

// Local private includes
#include "base_descriptor_wrapper.hpp"
#include "data_point_template.hpp"
#include "error.hpp"

// Standard includes
#include <any>
#include <vector>

namespace panacea {
  /*
   * The descriptor wrapper should be flexible enough to allow
   * the underlying type to vary
   */
  template<class T>
  class DescriptorWrapper : public BaseDescriptorWrapper {
    private: 
      DataPointTemplate<T> data_wrapper_;
      std::vector<int> reduced_chosen_dimensions_;
    public:
      DescriptorWrapper(T data, int rows, int cols);

      typedef const T type;

      virtual double& operator()(const int point_ind, const int dim_ind) final;
      virtual double operator()(const int point_ind, const int dim_ind) const final;

      virtual int rows() const final;
      virtual int cols() const final;
      virtual int getNumberDimensions() const final;
      virtual int getNumberPoints() const final;
      virtual void set(const Arrangement arrangement) final;
      virtual void setReducedNumberDimensions(std::vector<int> reduced_chosen_dimensions) final;
      virtual const std::vector<int> getReducedDimensions() const final;
      virtual const size_t getNumberReducedDimensions() const final;
      virtual std::any getPointerToRawData() noexcept final;
  };

  template<class T>
  inline DescriptorWrapper<T>::DescriptorWrapper(T data, int rows, int cols) : 
        data_wrapper_(data, rows, cols) {
      // Create range of dimensions
      reduced_chosen_dimensions_.reserve(data_wrapper_.getNumberDimensions());
      for( int dim = 0; dim < data_wrapper_.getNumberDimensions(); ++dim) {
        reduced_chosen_dimensions_.push_back(dim);
      }
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
    if( data_wrapper_.getNumberDimensions() < reduced_chosen_dimensions_.size() ) {
      reduced_chosen_dimensions_.resize(data_wrapper_.getNumberDimensions());
    } else if (data_wrapper_.getNumberDimensions() > reduced_chosen_dimensions_.size() ) {
      int starting_dim = reduced_chosen_dimensions_.size() - 1;
      reduced_chosen_dimensions_.resize(data_wrapper_.getNumberDimensions());
      for( int dim = starting_dim; dim < data_wrapper_.getNumberDimensions(); ++dim) {
        reduced_chosen_dimensions_.at(dim) = dim;
      }
    }
  }

  template<class T>
  inline void DescriptorWrapper<T>::setReducedNumberDimensions(std::vector<int> reduced_chosen_dimensions) {
    /// Ensure that the maximum chosen dimension is less than
    /// the total number of dimensions
    /// Ensure the minimum dimension index is not less than 0
    if(reduced_chosen_dimensions.size() > 0) {
      if(*std::max_element(reduced_chosen_dimensions.begin(),
            reduced_chosen_dimensions.end()) > data_wrapper_.getNumberDimensions()) {
        PANACEA_FAIL("Cannot set reduced number of dimensions, indices are too large");
      }
      if(*std::min_element(reduced_chosen_dimensions.begin(),
            reduced_chosen_dimensions.end()) < 0) {
        PANACEA_FAIL("Cannot set reduced number of dimensions, indices are too large");
      }
    }
    reduced_chosen_dimensions_ = reduced_chosen_dimensions;
  }

  template<class T>
  inline const std::vector<int> DescriptorWrapper<T>::getReducedDimensions() const {
    return reduced_chosen_dimensions_; 
  }

  template<class T>
  inline const size_t DescriptorWrapper<T>::getNumberReducedDimensions() const {
    return reduced_chosen_dimensions_.size();
  }

  template<class T>
  inline std::any DescriptorWrapper<T>::getPointerToRawData() noexcept {
    return data_wrapper_.getPointerToRawData();
  }


}
#endif // PANACEA_PRIVATE_DESCRIPTORWRAPPER_H
