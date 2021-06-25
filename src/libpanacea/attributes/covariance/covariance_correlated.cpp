
// Local private PANACEA includes
#include "covariance_correlated.hpp"

#include "attributes/covariance.hpp"
#include "covariance_functions.hpp"
#include "error.hpp"
#include "matrix/matrix.hpp"
#include "type_map.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>
#include <typeindex>

namespace panacea {

  namespace correlated {
    void updateCovariance(
        Matrix & covariance,
        const Vector & current_mean,
        const Vector & new_mean,
        const int current_num_pts,
        const BaseDescriptorWrapper & desc_wrap) {

      const int num_dims = desc_wrap.getNumberDimensions();
      const int num_pts = desc_wrap.getNumberPoints();

      assert(covariance.rows() == num_dims);
      assert(covariance.cols() == num_dims);
      assert(current_mean.rows() == num_dims);
      assert(new_mean.rows() == num_dims);
      const int total_num_pts = current_num_pts + num_pts;
      for( int dim=0; dim<num_dims; ++dim){
        { // account for diagonal
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += desc_wrap(pt,dim) * desc_wrap(pt,dim);
          }
          double A_ij = covariance(dim,dim) * (static_cast<double>(current_num_pts) - 1.0) +
            static_cast<double>(current_num_pts) * current_mean(dim)*current_mean(dim) + sum;
          double B_ij = static_cast<double>(total_num_pts) * new_mean(dim)*new_mean(dim);

          covariance(dim,dim)  = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          covariance(dim,dim) *= (A_ij - B_ij);
        } // end account for diagonal
        for( int dim2=dim+1; dim2<num_dims; ++dim2) { // account for off diagonal elements
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += desc_wrap(pt,dim) * desc_wrap(pt,dim2);
          }
          double A_ij = covariance(dim,dim2) * (static_cast<double>(current_num_pts) - 1.0) +
            static_cast<double>(current_num_pts) * current_mean(dim)*current_mean(dim2) + sum;
          double B_ij = static_cast<double>(total_num_pts) * -1.0* new_mean(dim) * new_mean(dim2);
          covariance(dim,dim2) = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          covariance(dim,dim2) *= (A_ij + B_ij);
          covariance(dim2,dim) = covariance(dim,dim2);
        } // account for off diagonal elements
      }
    }
  } // namespace correlated

  CovarianceCorrelated::CovarianceCorrelated(const CovarianceBuild memory) {
    if( memory == CovarianceBuild::Allocate) {
      matrix_ = createMatrix(0, 0);
      mean_ = createVector(0);
    }
  }

  CovarianceCorrelated::CovarianceCorrelated(
      const BaseDescriptorWrapper & desc_wrap,
      const CovarianceOption opt) {
    // Resize the covariance matrix based on the number of descriptor dimensions
    const int num_dims = desc_wrap.getNumberDimensions();
    matrix_ = createMatrix(num_dims, num_dims);
    mean_ = createVector(num_dims);

    auto new_mean = createVector(num_dims);
    mean_->setZero();
    new_mean->setZero();

    covariance::calculateMean(*new_mean.get(), desc_wrap);

    matrix_->resize(num_dims, num_dims);
    matrix_->setZero();

    correlated::updateCovariance(
        *matrix_.get(),
        *mean_.get(),
        *new_mean.get(),
        total_number_data_pts_,
        desc_wrap);

    if( isZero() ) {
      covariance::accountForZeroMatrix(opt, *matrix_.get());
    }else if(desc_wrap.getNumberPoints() == 1) {
      if( opt == CovarianceOption::Flexible ) {
        matrix_->makeIdentity();
      } else {
        std::string error_msg = "You are trying to create covariance matrix with ";
        error_msg += "a single point Either get a better starting data set or allow";
        error_msg += " flexibility in the algorithm.";
        PANACEA_FAIL(error_msg);
      }
    } else {
      covariance::accountForZeroOnDiagonal(opt, *matrix_.get());
      // Set any diagonal elements that are equal to 0.0 equal to 1.0
    }

    mean_ = std::move(new_mean);
    // Record the total number of data points used to create the covariance matrix
    total_number_data_pts_ = desc_wrap.getNumberPoints();

  }

  CovarianceCorrelated::CovarianceCorrelated(
      std::unique_ptr<Matrix> matrix,
      std::unique_ptr<Vector> mean,
      int total_num_pts,
      const CovarianceOption opt) :
        matrix_(std::move(matrix)),
        mean_(std::move(mean)),
        total_number_data_pts_(total_num_pts) {

    assert(matrix_->rows() == matrix_->cols() && "Covariance matrix must be square");
    assert(mean_->rows() == matrix_->rows() && "Mean and covariance matrix must have the same dimensions.");
    assert(total_num_pts > 0 && "Total number of points must be greater than 0");

    // Check for symmetry
    const double threshold = 1E-5;
    covariance::checkForSymmetry(*matrix_.get(), threshold);

    // All elements in the covariance matrix are found to be 0.0
    if( isZero() ) {
      covariance::accountForZeroMatrix(opt, *matrix_.get());
    }  else {
      covariance::accountForZeroOnDiagonal(opt, *matrix_.get());
    }
  }

  bool CovarianceCorrelated::isZero(const double threshold ) const noexcept {
    return matrix_->isZero(threshold);
  }

  void CovarianceCorrelated::update(const BaseDescriptorWrapper & desc_wrap) {

    auto new_mean = createVector(mean_->rows());
    for( int row=0; row< mean_->rows(); ++row){
      new_mean->operator()(row) = mean_->operator()(row);
    }

    covariance::updateMean(
        *new_mean.get(),
        desc_wrap,
        total_number_data_pts_);

    correlated::updateCovariance(
        *matrix_.get(),
        *mean_.get(),
        *new_mean.get(),
        total_number_data_pts_,
        desc_wrap);

    mean_ = std::move(new_mean);
    total_number_data_pts_ += desc_wrap.getNumberPoints();

  }

  double CovarianceCorrelated::operator()(const int row, const int col) const {
    return matrix_->operator()(row,col);
  }

  int CovarianceCorrelated::rows() const {
    return matrix_->rows();
  }

  int CovarianceCorrelated::cols() const {
    return matrix_->cols();
  }

  bool CovarianceCorrelated::defined() const noexcept {
    if( matrix_.get() == nullptr) {
      return false;
    }
    if( mean_.get() == nullptr) {
      return false;
    }
    return true;
  }

  double CovarianceCorrelated::getDeterminant() const {
    return matrix_->getDeterminant();
  }

  double CovarianceCorrelated::getMean(const int index) const {
    return mean_->operator()(index);
  }

  int CovarianceCorrelated::getCummulativeDescPoints() const {
    return total_number_data_pts_;
  }

  bool CovarianceCorrelated::is(const NormalizationState & state) const noexcept {
    return normalized_ == state;
  }

  const NormalizationState & CovarianceCorrelated::getNormalizationState() const noexcept {
    return normalized_;
  }

  void CovarianceCorrelated::set(PassKey<Normalizer>, NormalizationState state) {
    normalized_ = state;
  }

  double & CovarianceCorrelated::operator()(
      PassKey<Normalizer>, const int row, const int col) {
    return matrix_->operator()(row,col);
  }

  void CovarianceCorrelated::print() const {
    covariance::printCovariance(*matrix_.get(), *mean_.get(), total_number_data_pts_);
  }

  std::vector<std::any> CovarianceCorrelated::write(
      const settings::FileType file_type,
      std::ostream & os,
      std::any cov_instance) {

    const CovarianceCorrelated & cov_mat = [&]() -> const CovarianceCorrelated & {
      if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(Covariance *))){

        return const_cast<const CovarianceCorrelated &>(
            dynamic_cast<CovarianceCorrelated &>(
              *std::any_cast<Covariance *>(cov_instance)));

      }else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(Covariance &))){

        return const_cast<const CovarianceCorrelated &>(
            dynamic_cast<CovarianceCorrelated &>(
              std::any_cast<Covariance &>(cov_instance)));

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(const Covariance *))){

        return dynamic_cast<const CovarianceCorrelated &>(
            *std::any_cast<const Covariance *>(cov_instance));

      }else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(const Covariance &))){

        return dynamic_cast<const CovarianceCorrelated &>(
            std::any_cast<const Covariance &>(cov_instance));

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(CovarianceCorrelated *))){

        return const_cast<const CovarianceCorrelated &>(
            *std::any_cast<CovarianceCorrelated *>(cov_instance));

      }else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(CovarianceCorrelated &))){

        return const_cast<const CovarianceCorrelated &>(
            std::any_cast<CovarianceCorrelated &>(cov_instance));

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(const CovarianceCorrelated *))){

        return *std::any_cast<const CovarianceCorrelated *>(cov_instance);

      }else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(const CovarianceCorrelated &))){

        return std::any_cast<const CovarianceCorrelated &>(cov_instance);

      } else {
        std::string error_msg = "Unsupported Covariance type encountered.";
        PANACEA_FAIL(error_msg);
      }
      return std::any_cast<const CovarianceCorrelated &>(cov_instance);
    }();

    std::vector<std::any> nested_objs;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {

      os << cov_mat.total_number_data_pts_ << "\n\n";
      os << "[Normalization State]\n";
      os << cov_mat.normalized_ << "\n\n";
      // Note order of writing out the covariance matrix must be the
      // same as order of reading in
      nested_objs.push_back(cov_mat.matrix_.get());
      nested_objs.push_back(cov_mat.mean_.get());
    }

    return nested_objs;
  }

  io::ReadInstantiateVector CovarianceCorrelated::read(
      const settings::FileType file_type,
      std::istream & is,
      std::any cov_instance) {

    CovarianceCorrelated & cov_mat = [&]() -> CovarianceCorrelated & {
      if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(Covariance *))){

        return dynamic_cast<CovarianceCorrelated &>(
              *std::any_cast<Covariance *>(cov_instance));

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(std::unique_ptr<Covariance> *))){

        return dynamic_cast<CovarianceCorrelated &>(
              *std::any_cast<std::unique_ptr<Covariance> *>(cov_instance)->get());

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(Covariance &))){

        return dynamic_cast<CovarianceCorrelated &>(
              std::any_cast<Covariance &>(cov_instance));

      } else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(CovarianceCorrelated *))){

        return *std::any_cast<CovarianceCorrelated *>(cov_instance);

      }else if( std::type_index(cov_instance.type()) ==
          std::type_index(typeid(CovarianceCorrelated &))){

        return std::any_cast<CovarianceCorrelated &>(cov_instance);

      } else {
        std::string error_msg = "Unsupported Covariance type encountered.";
        if( type_map.count(std::type_index(cov_instance.type()))){
          error_msg += "Type identified as: ";
          error_msg += type_map.at(std::type_index(cov_instance.type()));
          error_msg += "\n";
        }
        PANACEA_FAIL(error_msg);
      }
      return std::any_cast<CovarianceCorrelated &>(cov_instance);
    }();

    io::ReadInstantiateVector nested_objs;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {

      std::string line = "";

      std::getline(is, line);
      {
        std::istringstream ss(line);
        ss >> cov_mat.total_number_data_pts_;
      }

      while(line.find("[Normalization State]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Normalization State] header while trying ";
          error_msg += "to read restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      std::getline(is, line);
      if( line.find("Normalized",0) != std::string::npos){
        cov_mat.normalized_ = NormalizationState::Normalized;
      }else if(line.find("Unnormalized",0) != std::string::npos ) {
        cov_mat.normalized_ = NormalizationState::Unnormalized;
      } else {
        std::cout << "Warning normalization state value is missing from restart file.";
        std::cout << " Assuming covariance data is unnormalized.\n";
        std::cout << "line is: " << line << std::endl;
        cov_mat.normalized_ = NormalizationState::Unnormalized;
      }

      // Check to see if memory has been allocated to the internal vector and matrix
      if( cov_mat.matrix_.get() == nullptr ) {
        cov_mat.matrix_ = createMatrix(0,0);
      }
      if( cov_mat.mean_.get() == nullptr ) {
        cov_mat.mean_ = createVector(0);
      }
      nested_objs.emplace_back(cov_mat.matrix_.get(), std::nullopt);
      nested_objs.emplace_back(cov_mat.mean_.get(),std::nullopt);
    }

    return nested_objs;
  }
}
