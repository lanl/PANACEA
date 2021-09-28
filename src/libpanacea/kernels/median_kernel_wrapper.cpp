
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "median_kernel_wrapper.hpp"

#include "data_point_template.hpp"
#include "median.hpp"

// Standard incldues
#include <algorithm>
#include <cstdlib>
#include <iomanip>
#include <vector>

namespace panacea {

/*******************************************
 * File scope functions
 *******************************************/

namespace {

void update_median_point_values(
    const BaseDescriptorWrapper &dwrapper,
    std::vector<std::deque<double>> &pts_near_median) {
  const int num_pts = dwrapper.getNumberPoints();
  const int ndim = dwrapper.getNumberDimensions();

  if (pts_near_median.size() != ndim) {
    pts_near_median.resize(ndim);
  }

  for (int dim = 0; dim < ndim; ++dim) {
    for (int pt = 0; pt < num_pts; ++pt) {
      pts_near_median.at(dim).push_back(dwrapper(pt, dim));
    }
  }

  // Sort each of the deques
  for (int dim = 0; dim < ndim; ++dim) {
    std::sort(pts_near_median.at(dim).begin(), pts_near_median.at(dim).end());
  }
}

void trim(std::vector<std::deque<double>> &pts_near_median,
          const int max_number_pts, bool &remove_more_from_front) {

  const int num_pts = [&]() -> const int {
    if (pts_near_median.size() == 0) {
      return 0;
    }
    return pts_near_median.at(0).size();
  }();

  const int diff = std::abs(max_number_pts - num_pts);
  if (num_pts > max_number_pts) {
    int remove_from_front = diff / 2;
    int remove_from_back = diff / 2;
    if (diff % 2 == 1) {
      if (remove_more_from_front) {
        ++remove_from_front;
        remove_from_front = false;
      } else {
        ++remove_from_back;
        remove_from_front = true;
      }
    }

    for (int dim = 0; dim < pts_near_median.size(); ++dim) {
      // Erase from end first
      auto it_end = pts_near_median.at(dim).end();
      pts_near_median.at(dim).erase((it_end - remove_from_back), it_end);

      auto it_begin = pts_near_median.at(dim).begin();
      pts_near_median.at(dim).erase(it_begin, it_begin + remove_from_front);
    }
  }
}
} // namespace

/*******************************************
 * Private methods
 *******************************************/

BaseKernelWrapper::ReadFunction MedianKernelWrapper::getReadFunction_() {
  return MedianKernelWrapper::read;
}

BaseKernelWrapper::WriteFunction
MedianKernelWrapper::getWriteFunction_() const {
  return MedianKernelWrapper::write;
}

MedianKernelWrapper::MedianKernelWrapper(
    const BaseDescriptorWrapper &dwrapper) {

  Median median;
  auto center_ =
      median.calculate<const BaseDescriptorWrapper &, Direction::AlongColumns>(
          dwrapper);

  data_wrapper_ =
      DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  update_median_point_values(dwrapper, points_near_median_);
  trim(points_near_median_, number_pts_store_, remove_from_front_);

  number_pts_median_ = dwrapper.getNumberPoints();
}

MedianKernelWrapper::MedianKernelWrapper(
    const std::vector<double> &median_vec) {

  int num_pts = 0;
  if (median_vec.size() == 0) {
    data_wrapper_ = DataPointTemplate<std::vector<double>>(median_vec, num_pts,
                                                           median_vec.size());
  } else {
    num_pts = 1;
    data_wrapper_ = DataPointTemplate<std::vector<double>>(median_vec, num_pts,
                                                           median_vec.size());
  }
  for (int dim = 0; dim < median_vec.size(); ++dim) {
    for (int pt = 0; pt < num_pts; ++pt) {
      points_near_median_.at(dim).push_back(median_vec.at(dim));
    }
  }
  trim(points_near_median_, number_pts_store_, remove_from_front_);

  number_pts_median_ = num_pts;
}

/*******************************************
 * Public methods
 *******************************************/
void MedianKernelWrapper::update(const BaseDescriptorWrapper &dwrapper) {

  update_median_point_values(dwrapper, points_near_median_);
  trim(points_near_median_, number_pts_store_, remove_from_front_);

  Median median;
  // Because of how the points are stored, for ease of sorting we need to
  // calculate the median here Along the Rows.
  auto center_ =
      median.calculate<std::vector<std::deque<double>>, Direction::AlongRows>(
          points_near_median_);

  data_wrapper_ =
      DataPointTemplate<std::vector<double>>(center_, 1, center_.size());
  number_pts_median_ += dwrapper.getNumberPoints();
}

double &MedianKernelWrapper::at(const int row, const int col) {
  return data_wrapper_.at(row, col);
}

double MedianKernelWrapper::at(const int row, const int col) const {
  return data_wrapper_.at(row, col);
}

void MedianKernelWrapper::resize(const int rows, const int cols) {
  data_wrapper_.resize(rows, cols);
}

int MedianKernelWrapper::rows() const { return data_wrapper_.rows(); }

int MedianKernelWrapper::cols() const { return data_wrapper_.cols(); }

int MedianKernelWrapper::getNumberDimensions() const {
  return data_wrapper_.getNumberDimensions();
}

int MedianKernelWrapper::getNumberPoints() const { return number_pts_median_; }

const Arrangement &MedianKernelWrapper::arrangement() const noexcept {
  return data_wrapper_.arrangement();
}

void MedianKernelWrapper::set(const Arrangement arrangement) {
  data_wrapper_.set(arrangement);
}

const std::any MedianKernelWrapper::getPointerToRawData() const noexcept {
  return data_wrapper_.getPointerToRawData();
}

void MedianKernelWrapper::print() const { data_wrapper_.print(); }

std::type_index MedianKernelWrapper::getTypeIndex() const noexcept {
  return std::type_index(typeid(std::vector<double>));
}

const settings::KernelCenterCalculation MedianKernelWrapper::center() const
    noexcept {
  return settings::KernelCenterCalculation::Median;
}

const settings::KernelCount MedianKernelWrapper::count() const noexcept {
  return settings::KernelCount::Single;
}

/********************************************
 * Static methods
 ********************************************/

std::istream &MedianKernelWrapper::read(BaseKernelWrapper &kwrapper_instance,
                                        std::istream &is) {

  MedianKernelWrapper &kwrapper_median =
      dynamic_cast<MedianKernelWrapper &>(kwrapper_instance);
  std::string line = "";
  while (line.find("[Total Number Points]", 0) == std::string::npos) {
    if (is.peek() == EOF) {
      std::string error_msg =
          "Did not find [Total Number Points] header while trying ";
      error_msg += "to read in median kernel wrapper from restart file.";
      PANACEA_FAIL(error_msg);
    }
    std::getline(is, line);
  }

  try {
    is >> kwrapper_median.number_pts_median_;
  } catch (...) {
    std::string error_msg = "Unable to assign total number of points to median";
    error_msg = " kernel type from file.\n";
    error_msg += "line is: " + line;
    PANACEA_FAIL(error_msg);
  }

  std::getline(is, line);
  while (line.find("[Number Points Tracked Near Median]", 0) ==
         std::string::npos) {
    if (is.peek() == EOF) {
      std::string error_msg =
          "Did not find [Number Points Tracked Near Median]";
      error_msg +=
          " header while trying to read in median kernel wrapper from ";
      error_msg += "restart file.";
      PANACEA_FAIL(error_msg);
    }
    std::getline(is, line);
  }

  try {
    is >> kwrapper_median.number_pts_store_;
  } catch (...) {
    std::string error_msg =
        "Did not find [Number Points Tracked Near Median] value";
    error_msg += " while trying to read restart file.\n";
    error_msg += "line is: " + line;
    PANACEA_FAIL(error_msg);
  }

  std::getline(is, line);
  while (line.find("[Points Near Median]", 0) == std::string::npos) {
    if (is.peek() == EOF) {
      std::string error_msg = "Did not find [Points Near Median]";
      error_msg +=
          " header while trying to read in median kernel wrapper from ";
      error_msg += "restart file.";
      PANACEA_FAIL(error_msg);
    }
    std::getline(is, line);
  }

  std::istringstream ss(line);

  int rows;
  try {
    ss >> rows;
  } catch (...) {
    std::string error_msg =
        "Unable to read in rows from Median Kernel section of ";
    error_msg += "restart file.\n";
    error_msg += "line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }
  int cols;
  try {
    ss >> cols;
  } catch (...) {
    std::string error_msg =
        "Unable to read in columns from Median Kernel section of ";
    error_msg += "restart file.\n";
    error_msg += "line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  try {
    kwrapper_median.points_near_median_.clear();
    for (int row = 0; row < rows; ++row) {
      std::getline(is, line);
      std::istringstream ss_data(line);
      std::deque<double> data_row;
      for (int col = 0; col < cols; ++col) {
        double value;
        ss_data >> value;
        data_row.push_back(value);
      }
      kwrapper_median.points_near_median_.push_back(data_row);
    }
  } catch (...) {
    std::string error_msg =
        "Detected an error while attempting to read in points near ";
    error_msg += "median section from restart file.\n";
    error_msg += "line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }
  return is;
}

std::ostream &
MedianKernelWrapper::write(const BaseKernelWrapper &kwrapper_instance,
                           std::ostream &os) {
  const MedianKernelWrapper &kwrapper_median =
      dynamic_cast<const MedianKernelWrapper &>(kwrapper_instance);
  os << "[Total Number Points]\n";
  os << kwrapper_median.number_pts_median_ << "\n";
  os << "[Number Points Tracked Near Median]\n";
  os << kwrapper_median.number_pts_store_ << "\n";
  os << "[Points Near Median]\n";
  int rows = kwrapper_median.points_near_median_.size();
  int cols = 0;
  if (rows > 0) {
    cols = kwrapper_median.points_near_median_.at(0).size();
  }
  os << rows << " " << cols << "\n";
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      os << std::setfill(' ') << std::setw(14) << std::setprecision(8)
         << std::right << kwrapper_median.points_near_median_.at(row).at(col)
         << " ";
    }
    os << "\n";
  }
  return os;
}

} // namespace panacea
