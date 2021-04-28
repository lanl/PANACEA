
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "median_kernel_wrapper.hpp"

#include "median.hpp"
#include "data_point_template.hpp"

// Standard incldues
#include <vector>

namespace panacea {

  /*******************************************
   * File scope functions
   *******************************************/

  namespace {

    void update_median_point_values(const BaseDescriptorWrapper * dwrapper, std::vector<std::deque<double>> & pts_near_median ){
      const int num_pts = dwrapper->getNumberPoints();
      const int ndim = dwrapper->getNumberDimensions();

      if( pts_near_median.size() != ndim ) {
        pts_near_median.resize(ndim);
      }

      for( int dim = 0; dim < ndim; ++dim) {
        for( int pt = 0; pt < num_pts; ++pt) {
          pts_near_median.at(dim).push_back(dwrapper->operator()(pt, dim));
        }
      }

      // Sort each of the deques
      for( int dim = 0; dim < ndim; ++dim) {
        std::sort(pts_near_median.at(dim).begin(), pts_near_median.at(dim).end());
      } 
    
    }

    void trim(
        std::vector<std::deque<double>> & pts_near_median,
        const int max_number_pts,
        bool & remove_more_from_front){

      const int num_pts = pts_near_median.at(0).size();
      const int diff = max_number_pts - num_pts;
      if( num_pts > max_number_pts ) {
        int remove_from_front = diff/2;
        int remove_from_back = diff/2;
        if( diff % 2 == 1 ) {
          if( remove_more_from_front ) {
            ++remove_from_front;
            remove_from_front = false;
          } else {
            ++remove_from_back;
            remove_from_front = true;
          }
        }  
      
        for( int dim = 0; dim < pts_near_median.size(); ++dim){
          // Erase from end first
          auto it_end = pts_near_median.at(dim).end();
          pts_near_median.at(dim).erase((it_end - remove_from_back), it_end); 
  
          auto it_begin = pts_near_median.at(dim).begin();
          pts_near_median.at(dim).erase(it_begin, it_begin + remove_from_front); 
        }
      }
    }
  }

  /*******************************************
   * Public methods
   *******************************************/

  MedianKernelWrapper::MedianKernelWrapper(
      const PassKey<KernelWrapperFactory> &,
      const BaseDescriptorWrapper * dwrapper
      ) {

    Median median;
    auto center_ = median.calculate<
      const BaseDescriptorWrapper *,
            Direction::AlongColumns>(dwrapper);

    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
    update_median_point_values(dwrapper, points_near_median_);
    trim(points_near_median_, number_pts_store_, remove_from_front_);
    
    number_pts_median_ = dwrapper->getNumberPoints();
  }

  MedianKernelWrapper::MedianKernelWrapper(
      const PassKey<test::Test> &,
      const BaseDescriptorWrapper * dwrapper
      ) {

    Median median;
    auto center_ = median.calculate<
      const BaseDescriptorWrapper *,Direction::AlongColumns>(dwrapper);
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());

    update_median_point_values(dwrapper, points_near_median_);
    trim(points_near_median_, number_pts_store_, remove_from_front_);
    number_pts_median_ = dwrapper->getNumberPoints();
  }

  void MedianKernelWrapper::update(const BaseDescriptorWrapper * dwrapper) {

    update_median_point_values(dwrapper, points_near_median_);
    trim(points_near_median_, number_pts_store_, remove_from_front_);

    Median median;
    // Because of how the points are stored, for ease of sorting we need to
    // calculate the median here Along the Rows. 
    auto center_ = median.calculate<
      std::vector<std::deque<double>>,Direction::AlongRows>(points_near_median_);

    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
    number_pts_median_ += dwrapper->getNumberPoints();
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
    return number_pts_median_;
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
