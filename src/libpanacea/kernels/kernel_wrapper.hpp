
#ifndef PANACEA_PRIVATE_KERNELWRAPPER_H
#define PANACEA_PRIVATE_KERNELWRAPPER_H
#pragma once

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private includes
#include "base_kernel_wrapper.hpp"
#include "data_point_template.hpp"
#include "passkey.hpp"

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
  template<class T>
    class KernelWrapper : public BaseKernelWrapper {
      private:
        DataPointTemplate<T> data_wrapper_;
      public:
        KernelWrapper(const PassKey<KernelWrapperFactory> &, T data, int rows, int cols) : 
          data_wrapper_(data, rows, cols) {};

        KernelWrapper(const PassKey<test::Test> &, T data, int rows, int cols) : 
          data_wrapper_(data, rows, cols) {};

        virtual double& operator()(const int point_ind, const int dim_ind) final;
        virtual double operator()(const int point_ind, const int dim_ind) const final;
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

  template<class T>
    inline double& KernelWrapper<T>::operator()(const int point_ind, const int dim_ind) {
      return data_wrapper_(point_ind, dim_ind);
    }

  template<class T>
    inline double KernelWrapper<T>::operator()(const int point_ind, const int dim_ind) const {
      return data_wrapper_(point_ind, dim_ind);
    }

  template<class T>
    inline int KernelWrapper<T>::rows() const {
      return data_wrapper_.rows();
    }

  template<class T>
    inline int KernelWrapper<T>::cols() const {
      return data_wrapper_.cols();
    }

  template<class T>
    inline int KernelWrapper<T>::getNumberDimensions() const {
      return data_wrapper_.getNumberDimensions();
    }

  template<class T>
    inline int KernelWrapper<T>::getNumberPoints() const {
      return data_wrapper_.getNumberPoints();
    }

  template<class T>
    inline void KernelWrapper<T>::set(const Arrangement arrangement) {
      data_wrapper_.set(arrangement);
    }

  template<class T>
    inline void KernelWrapper<T>::update(const BaseDescriptorWrapper * dwrapper) {
      std::cout << "Calling update Kernel" << std::endl;
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      data_wrapper_ = DataPointTemplate<T>(
          std::any_cast<T>(dwrapper->getPointerToRawData()),
          dwrapper->rows(),
          dwrapper->cols());
    }

  template<class T>
    inline const std::any KernelWrapper<T>::getPointerToRawData() const noexcept {
      return data_wrapper_.getPointerToRawData();
    }

  template<class T>
    inline void KernelWrapper<T>::print() const {
      data_wrapper_.print();
    }

  template<class T>
    inline std::unique_ptr<BaseKernelWrapper> KernelWrapper<T>::create(
        const PassKey<KernelWrapperFactory> & key,
        std::any data,
        const int rows,
        const int cols) {

      std::cout << "Calling create KernelWrapper" << std::endl;
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return std::make_unique<KernelWrapper<T>>(key, std::any_cast<T>(data), rows, cols); 
    }

  template<class T>
    inline std::type_index KernelWrapper<T>::getTypeIndex() const noexcept {
      return typeid(T);
    }
}
#endif // PANACEA_PRIVATE_KERNELWRAPPER_H
