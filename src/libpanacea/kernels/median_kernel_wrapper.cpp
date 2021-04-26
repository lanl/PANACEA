
// Local private PANACEA includes
#include "median_kernel_wrapper.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "median.hpp"
#include "data_point_template.hpp"

// Standard incldues
#include <vector>

namespace panacea {

  MedianKernelWrapper::MedianKernelWrapper(
      const PassKey<KernelWrapperFactory> &,
      BaseDescriptorWrapper * desc_wrapper
      ) {

    Median median;
    auto center_ = median.calculate<
      BaseDescriptorWrapper *,Direction::AlongColumns>(desc_wrapper);
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  }

  MedianKernelWrapper::MedianKernelWrapper(
      const PassKey<test::Test> &,
      BaseDescriptorWrapper * desc_wrapper
      ) {

    Median median;
    auto center_ = median.calculate<
      BaseDescriptorWrapper *,Direction::AlongColumns>(desc_wrapper);
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  }

  double& MedianKernelWrapper::operator()(const int point_ind, const int dim_ind) {
    return data_wrapper_(point_ind, dim_ind);
  }

  double MedianKernelWrapper::operator()(const int point_ind, const int dim_ind) const {
    return data_wrapper_(point_ind, dim_ind);
  }

  int MedianKernelWrapper::rows() const {
    return data_wrapper_.rows();
  }

  int MedianKernelWrapper::cols() const {
    return data_wrapper_.cols();
  }

  int MedianKernelWrapper::getNumberDimensions() const {
    return data_wrapper_.getNumberDimensions();
  }

  int MedianKernelWrapper::getNumberPoints() const {
    return data_wrapper_.getNumberPoints();
  }

  void MedianKernelWrapper::set(const Arrangement arrangement) {
    data_wrapper_.set(arrangement);
  }

  const std::any MedianKernelWrapper::getPointerToRawData() const noexcept {
    return data_wrapper_.getPointerToRawData();
  }

  void MedianKernelWrapper::print() const {
    data_wrapper_.print();
  }

  std::type_index MedianKernelWrapper::getTypeIndex() const noexcept {
    return typeid(std::vector<double>);
  }

}
