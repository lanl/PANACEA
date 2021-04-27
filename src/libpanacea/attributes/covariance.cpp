/*
 * Class for storing the covariance matrix
 */

// Local private PANACEA includes
#include "covariance.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"
#include "error.hpp"

// Local public PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Standard includes
#include <cassert>
#include <iostream>
namespace panacea {

  namespace correlated {
    void calculateMean(Vector * mean, const BaseDescriptorWrapper * desc_wrap) {
      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      mean->resize(num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double sum = 0.0;  
        for( int pt = 0; pt<num_pts; ++pt) {
          sum += (*desc_wrap)(pt, dim);
        }
        (*mean)(dim) = sum/static_cast<double>(num_pts);
      } 
    }

    void updateCovariance(
        Matrix * covariance,
        const Vector * current_mean,
        const Vector * new_mean,
        const int current_num_pts,
        const BaseDescriptorWrapper * desc_wrap) {

      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();

      assert(covariance->rows() == num_dims);
      assert(covariance->cols() == num_dims);
      assert(current_mean->rows() == num_dims);
      assert(new_mean->rows() == num_dims);
      const int total_num_pts = current_num_pts + num_pts;
      for( int dim=0; dim<num_dims; ++dim){
        { // account for diagonal
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim); 
          }
          const double A_ij = (*covariance)(dim,dim) * (static_cast<double>(current_num_pts) - 1.0) + 
            static_cast<double>(current_num_pts) * (*current_mean)(dim)*(*current_mean)(dim) + sum;
          (*covariance)(dim,dim)  = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          (*covariance)(dim,dim) *= (A_ij - static_cast<double>(total_num_pts) * (*new_mean)(dim)*(*new_mean)(dim)); 
        } // end account for diagonal
        for( int dim2=dim+1; dim2<num_dims; ++dim2) { // account for off diagonal elements
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim2); 
          }
          const double A_ij = (*covariance)(dim,dim2) * (static_cast<double>(current_num_pts) - 1.0) + 
            static_cast<double>(current_num_pts) * (*current_mean)(dim)*(*current_mean)(dim2) + sum;
          (*covariance)(dim,dim2) = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          (*covariance)(dim,dim2) *= (A_ij + 
              static_cast<double>(total_num_pts) * -1.0* (*new_mean)(dim) * (*new_mean)(dim2));
          (*covariance)(dim2,dim) = (*covariance)(dim,dim2);
        } // account for off diagonal elements
      }
    }

    void updateMean(
        Vector * mean,
        const BaseDescriptorWrapper * desc_wrap,
        const int current_num_desc_pts) {

      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      assert(mean->rows() == num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double new_sum = 0.0;  
        for( int pt = 0; pt<num_pts; ++pt) {
          new_sum += (*desc_wrap)(pt, dim);
        }
        double old_sum = (*mean)(dim) * static_cast<double>(current_num_desc_pts);
        (*mean)(dim) = (old_sum + new_sum)/static_cast<double>(num_pts + current_num_desc_pts);
      }
    }
  } // namespace correlated

  Covariance::Covariance(const BaseDescriptorWrapper * desc_wrap, const CovarianceOption opt) {
    // Resize the covariance matrix based on the number of descriptor dimensions
    assert(desc_wrap != nullptr);
    const int num_dims = desc_wrap->getNumberDimensions();
    matrix_ = createMatrix(num_dims, num_dims);
    mean_ = createVector(num_dims);

    auto new_mean = createVector(num_dims);
    for( int row=0; row< mean_->rows(); ++row){
      mean_->operator()(row) = 0.0;
      new_mean->operator()(row) = mean_->operator()(row);
    }

    correlated::calculateMean(new_mean.get(), desc_wrap);

    matrix_->resize(num_dims, num_dims);
    matrix_->setZero();
    
    correlated::updateCovariance(
        matrix_.get(),
        mean_.get(),
        new_mean.get(),
        total_number_data_pts_,
        desc_wrap);

    if( isZero() ) {
      // Making matrix equal to the identity matirx
      if( opt == CovarianceOption::Flexible ) {
        matrix_->makeIdentity();
      } else {
        std::string error_msg = "Covariance matrix is 0 everywhere, likely because your data is stacked.";
        error_msg += " Either get a better starting data set or allow flexibility in the algorithm.";
        PANACEA_FAIL(error_msg);
      }
    }else if(desc_wrap->getNumberPoints() == 1) {
      if( opt == CovarianceOption::Flexible ) {
        matrix_->makeIdentity();
      } else {
        std::string error_msg = "You are trying to create covariance matrix with a single point";
        error_msg += " Either get a better starting data set or allow flexibility in the algorithm.";
        PANACEA_FAIL(error_msg);
      }
    } else { 
      for( int dim1 = 0; dim1 < desc_wrap->getNumberDimensions(); ++dim1){

        if( std::abs(matrix_->operator()(dim1,dim1)) < 1E-9 ){
          if ( opt == CovarianceOption::Flexible ) {
            matrix_->operator()(dim1,dim1) = 1.0;
          } else {
            std::string error_msg = "Zero's detected in the diagonal of covariance matrix. ";
            error_msg += " Either get a better starting data set or allow flexibility in the";
            error_msg += " covariance matrix algorithm.";
            PANACEA_FAIL(error_msg);
          }
        }
      }
      // Set any diagonal elements that are equal to 0.0 equal to 1.0
    }

    mean_ = std::move(new_mean);
    // Record the total number of data points used to create the covariance matrix
    total_number_data_pts_ = desc_wrap->getNumberPoints();
    
  }

  Covariance::Covariance(
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
    for( int i = 0; i < matrix_->rows(); ++i) {
      for( int j = i+1; j < matrix_->rows(); ++j) {
        // Don't need to check the diagonal
        if( std::abs(matrix_->operator()(i,j)) > threshold) {
          // Only check values that are not too close to 0.0
          const double diff = std::abs(matrix_->operator()(i,j) - matrix_->operator()(j,i));
          const double avg =  std::abs(matrix_->operator()(i,j) + matrix_->operator()(j,i))/2.0;
          assert(diff/avg < threshold && "Covariance symmetry criteria not satisfied"); 
        }
      }
    }

    // All elements in the covariance matrix are found to be 0.0
    if( isZero() ) {
      // Making matrix equal to the identity matirx
      if( opt == CovarianceOption::Flexible ) {
        matrix_->makeIdentity();
      } else {
        std::string error_msg = "Covariance matrix is 0 everywhere, likely because your data is stacked.";
        error_msg += " Either get a better starting data set or allow flexibility in the algorithm.";
        PANACEA_FAIL(error_msg);
      }
    }  else {
      
      for( int dim1 = 0; dim1 < matrix_->rows(); ++dim1){

        if( std::abs(matrix_->operator()(dim1,dim1)) < 1E-9 ){
          if ( opt == CovarianceOption::Flexible ) {
            matrix_->operator()(dim1,dim1) = 1.0;
          } else {
            std::string error_msg = "Zero's detected in the diagonal of covariance matrix. ";
            error_msg += " Either get a better starting data set or allow flexibility in the";
            error_msg += " covariance matrix algorithm.";
            PANACEA_FAIL(error_msg);
          }
        }
      }
    }
  }

  bool Covariance::isZero(const double threshold ) const noexcept {
    assert(threshold > 0.0);
    // Because it is symmetric only need to check one half    
    for( int i = 0; i < matrix_->rows(); ++i) {
      for( int j = i; j < matrix_->cols(); ++j) {
        if( std::abs(matrix_->operator()(i,j)) > threshold ) return false;
      }
    }
    return true;
  }

  void Covariance::update(const BaseDescriptorWrapper * desc_wrap) {

    auto new_mean = createVector(mean_->rows());
    for( int row=0; row< mean_->rows(); ++row){
      new_mean->operator()(row) = mean_->operator()(row);
    }

    correlated::updateMean(
        new_mean.get(),
        desc_wrap,
        total_number_data_pts_);

    correlated::updateCovariance(
        matrix_.get(),
        mean_.get(),
        new_mean.get(),
        total_number_data_pts_,
        desc_wrap);

    mean_ = std::move(new_mean);
    total_number_data_pts_ += desc_wrap->getNumberPoints();
   
  }

  double Covariance::operator()(const int row, const int col) const {
    return matrix_->operator()(row,col);
  }

  int Covariance::rows() const {
    return matrix_->rows();
  }

  int Covariance::cols() const {
    return matrix_->cols();
  }

  double Covariance::getDeterminant() const {
    return matrix_->getDeterminant();
  }

  double Covariance::getMean(const int index) const {
    return mean_->operator()(index);
  }

  int Covariance::getCummulativeDescPoints() const {
    return total_number_data_pts_;
  }

  bool Covariance::is(const NormalizationState & state) const noexcept {
    return normalized_ == state;
  }

  const NormalizationState & Covariance::getNormalizationState() const noexcept {
    return normalized_;
  }

  void Covariance::set(PassKey<Normalizer>, NormalizationState state) {
    normalized_ = state;
  }

  double & Covariance::operator()(PassKey<Normalizer>, const int row, const int col) {
    return matrix_->operator()(row,col);
  }

  void Covariance::print() const {
    std::cout << "Covariance Matrix\n";
    matrix_->print();
    std::cout << "\n";
    std::cout << "Mean\n";
    mean_->print();
    std::cout << "\n";
    std::cout << "Total Number of Points\n";
    std::cout << total_number_data_pts_;
    std::cout << std::endl;
  }
}
