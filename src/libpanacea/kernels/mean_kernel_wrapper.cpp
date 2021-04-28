
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "mean_kernel_wrapper.hpp"

#include "mean.hpp"

// Standard incldues
#include <vector>

namespace panacea {

  MeanKernelWrapper::MeanKernelWrapper(
      const PassKey<KernelWrapperFactory> &,
      const BaseDescriptorWrapper * dwrapper
      ) {

    Mean mean;
    std::vector<double> center_ = mean.calculate<const BaseDescriptorWrapper *,Direction::AlongColumns>(dwrapper);
    number_pts_mean_ = dwrapper->getNumberPoints();
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  }

  MeanKernelWrapper::MeanKernelWrapper(
      const PassKey<test::Test> &,
      const BaseDescriptorWrapper * dwrapper
      ) {

    Mean mean;
    std::vector<double> center_ = mean.calculate<const BaseDescriptorWrapper *,Direction::AlongColumns>(dwrapper);
    number_pts_mean_ = dwrapper->getNumberPoints();
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
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
    return number_pts_mean_;
  }

  void MeanKernelWrapper::set(const Arrangement arrangement) {
    data_wrapper_.set(arrangement);
  }

  void MeanKernelWrapper::update(const BaseDescriptorWrapper * dwrapper) {
    assert(dwrapper!=nullptr);
    assert(dwrapper->getNumberDimensions() == data_wrapper_.getNumberDimensions());

    Mean mean;
    std::vector<double> new_center = mean.calculate<const BaseDescriptorWrapper *,
      Direction::AlongColumns>(dwrapper);

    const double new_num_pts = static_cast<double>(dwrapper->getNumberPoints());
    const double inv_total_num_pts = 1.0/static_cast<double>(number_pts_mean_ + new_num_pts);
    for( int dim = 0; dim < data_wrapper_.getNumberDimensions(); ++dim){
      data_wrapper_(0,dim) = 
        (data_wrapper_(0,dim) * static_cast<double>(number_pts_mean_) + 
        new_center.at(dim) * new_num_pts) * 
        inv_total_num_pts; 
    }
    number_pts_mean_ += dwrapper->getNumberPoints();
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
