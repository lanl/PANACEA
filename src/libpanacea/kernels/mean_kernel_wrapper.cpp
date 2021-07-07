
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "mean_kernel_wrapper.hpp"

#include "mean.hpp"

// Standard incldues
#include <any>
#include <vector>

namespace panacea {

  /************************************************
   * Public Methods
   ************************************************/

  BaseKernelWrapper::ReadFunction MeanKernelWrapper::getReadFunction_() {
    return MeanKernelWrapper::read;
  }

  BaseKernelWrapper::WriteFunction MeanKernelWrapper::getWriteFunction_() const {
    return MeanKernelWrapper::write;
  }

  MeanKernelWrapper::MeanKernelWrapper(const BaseDescriptorWrapper & dwrapper) {
    Mean mean;
    std::vector<double> center_ = mean.calculate<const BaseDescriptorWrapper &,Direction::AlongColumns>(dwrapper);
    number_pts_mean_ = dwrapper.getNumberPoints();
    data_wrapper_ = DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  }

  MeanKernelWrapper::MeanKernelWrapper(const std::vector<double> & mean_vec) {
    if( mean_vec.size() == 0){
      number_pts_mean_ = 0;
      data_wrapper_ = DataPointTemplate<std::vector<double>>(mean_vec, 0, mean_vec.size());
    } else {
      number_pts_mean_ = 1;
      data_wrapper_ = DataPointTemplate<std::vector<double>>(mean_vec, 1, mean_vec.size());
    }
  }
  /************************************************
   * Public Methods
   ************************************************/
  double& MeanKernelWrapper::at(const int row, const int col) {
    return data_wrapper_.at(row, col);
  }

  double MeanKernelWrapper::at(const int row, const int col) const {
    return data_wrapper_.at(row, col);
  }

  void MeanKernelWrapper::resize(const int rows, const int cols) {
    data_wrapper_.resize(rows,cols);
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

  const Arrangement & MeanKernelWrapper::arrangement() const noexcept {
    return data_wrapper_.arrangement();
  }

  void MeanKernelWrapper::set(const Arrangement arrangement) {
    data_wrapper_.set(arrangement);
  }

  void MeanKernelWrapper::update(const BaseDescriptorWrapper & dwrapper) {
    assert(dwrapper.getNumberDimensions() == data_wrapper_.getNumberDimensions());

    Mean mean;
    std::vector<double> new_center = mean.calculate<const BaseDescriptorWrapper &,
      Direction::AlongColumns>(dwrapper);

        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    const double new_num_pts = static_cast<double>(dwrapper.getNumberPoints());
    const double inv_total_num_pts = 1.0/static_cast<double>(number_pts_mean_ + new_num_pts);
    for( int dim = 0; dim < data_wrapper_.getNumberDimensions(); ++dim){
      data_wrapper_(0,dim) =
        (data_wrapper_(0,dim) * static_cast<double>(number_pts_mean_) +
        new_center.at(dim) * new_num_pts) *
        inv_total_num_pts;
    }
    number_pts_mean_ += dwrapper.getNumberPoints();
  }

  const std::any MeanKernelWrapper::getPointerToRawData() const noexcept {
    return data_wrapper_.getPointerToRawData();
  }

  void MeanKernelWrapper::print() const {
    data_wrapper_.print();
  }

  std::type_index MeanKernelWrapper::getTypeIndex() const noexcept {
    return std::type_index(typeid(std::vector<double>));
  }

  const settings::KernelCenterCalculation MeanKernelWrapper::center() const noexcept {
    return settings::KernelCenterCalculation::Mean;
  }

  const settings::KernelCount MeanKernelWrapper::count() const noexcept {
    return settings::KernelCount::Single;
  }

  std::istream & MeanKernelWrapper::read(BaseKernelWrapper & kwrapper_instance, std::istream & is) {

        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    MeanKernelWrapper & kwrapper_mean = dynamic_cast<MeanKernelWrapper &>(kwrapper_instance);

    std::string line = "";
    while(line.find("[Total Number Points]",0) == std::string::npos) {
      if( is.peek() == EOF ) {
        std::string error_msg = "Did not find [Total Number Points] header while trying ";
        error_msg += "to read in mean kernel wrapper from restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    try {
      is >> kwrapper_mean.number_pts_mean_;// = num_pts;
    } catch (...) {
      std::string error_msg = "Unable to assign total number of points to mean";
      error_msg = " kernel type from file.\n";
      error_msg += "line is: " + line;
      PANACEA_FAIL(error_msg);
    }
    return is;
  }

  std::ostream & MeanKernelWrapper::write(const BaseKernelWrapper & kwrapper_instance, std::ostream & os) {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    const MeanKernelWrapper & kwrapper_mean = dynamic_cast<const MeanKernelWrapper &>(kwrapper_instance);
    os << "[Total Number Points]\n";
    os << kwrapper_mean.number_pts_mean_ << "\n";
    return os;
  }
}
