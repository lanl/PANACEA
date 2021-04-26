
// Local private PANACEA includes
#include "mean_kernel_wrapper.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "mean.hpp"

// Standard incldues
#include <vector>

namespace panacea {

  MeanKernelWrapper::MeanKernelWrapper(
      const PassKey<KernelWrapperFactory> &,
      BaseDescriptorWrapper * desc_wrapper
      ) {

    Mean mean;
    std::vector<double> center_ = mean.calculate<BaseDescriptorWrapper *,Direction::AlongColumns>(desc_wrapper);
    number_pts_mean_ = desc_wrapper->getNumberPoints();
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  }

  MeanKernelWrapper::MeanKernelWrapper(
      const PassKey<test::Test> &,
      BaseDescriptorWrapper * desc_wrapper
      ) {

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    Mean mean;
    std::vector<double> center_ = mean.calculate<BaseDescriptorWrapper *,Direction::AlongColumns>(desc_wrapper);
    number_pts_mean_ = desc_wrapper->getNumberPoints();
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  }

  double& MeanKernelWrapper::operator()(const int point_ind, const int dim_ind) {
    return data_wrapper_(point_ind, dim_ind);
  }

  double MeanKernelWrapper::operator()(const int point_ind, const int dim_ind) const {
    return data_wrapper_(point_ind, dim_ind);
  }

  int MeanKernelWrapper::rows() const {
    return data_wrapper_.rows();
  }

  int MeanKernelWrapper::cols() const {
    return data_wrapper_.cols();
  }

  int MeanKernelWrapper::getNumberDimensions() const {
    return data_wrapper_.getNumberDimensions();
  }

  int MeanKernelWrapper::getNumberPoints() const {
    return data_wrapper_.getNumberPoints();
  }

  void MeanKernelWrapper::set(const Arrangement arrangement) {
    data_wrapper_.set(arrangement);
  }

  const std::any MeanKernelWrapper::getPointerToRawData() const noexcept {
    return data_wrapper_.getPointerToRawData();
  }

  void MeanKernelWrapper::print() const {
    data_wrapper_.print();
  }

  std::type_index MeanKernelWrapper::getTypeIndex() const noexcept {
    return typeid(std::vector<double>);
  }

}
