/*
 * Class for stroing the covariance matrix
 */

// Local private PANACEA includes
#include "covariance.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"

// Local public PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Standard includes
#include <cassert>
#include <iostream>
namespace panacea {

  namespace correlated {
    void calculateMean(Vector * mean, BaseDescriptorWrapper * desc_wrap) {
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

    void calculateA(Matrix * A, BaseDescriptorWrapper * desc_wrap) {
      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      A->resize(num_dims, num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        { // account for diagonal
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim); 
          }
          (*A)(dim,dim) = sum;
        } // end account for diagonal
        for( int dim2=dim+1; dim2<num_dims; ++dim2) { // account for off diagonal elements
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim2); 
          }  
          (*A)(dim,dim2) = sum;
          (*A)(dim2,dim) = sum;
        } // account for off diagonal elements
      }
    }

    void calculateB(Vector * B, BaseDescriptorWrapper * desc_wrap) {
      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      B->resize(num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double sum = 0.0;
        for( int pt = 0; pt<num_pts; ++pt) {
          sum += (*desc_wrap)(pt,dim); 
        }
        (*B)(dim) = sum; 
      }
    }

    /*
     * A, B and the mean must have previously been calculated, current_num_pts refers
     * to the number of points that have already been used in previous calculations of 
     * the covariance matrix.
     */
    void updateCovariance(
        Matrix * covariance,
        const Matrix * A,
        const Vector * B,
        const Vector * mean,
        const int current_num_pts,
        BaseDescriptorWrapper * desc_wrap) {

      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      assert(covariance->rows() == num_dims);
      assert(covariance->cols() == num_dims);
      assert(A->rows() == num_dims);
      assert(A->cols() == num_dims);
      assert(B->rows() == num_dims);
      const int total_num_pts = current_num_pts + num_pts;
      for( int dim=0; dim<num_dims; ++dim){
        { // account for diagonal
          (*covariance)(dim,dim) = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          (*covariance)(dim,dim) *= ((*A)(dim,dim) - 2.0 * (*mean)(dim)*(*B)(dim) + 
              static_cast<double>(total_num_pts)*(*mean)(dim)*(*mean)(dim));

        } // end account for diagonal
        for( int dim2=dim+1; dim2<num_dims; ++dim2) { // account for off diagonal elements
          (*covariance)(dim,dim2) = 1.0/(static_cast<double>(total_num_pts) - 1.0);
          (*covariance)(dim,dim2) *= ((*A)(dim,dim2) - (*mean)(dim)*(*B)(dim2) - (*mean)(dim2) * (*B)(dim) +
              static_cast<double>(total_num_pts)*(*mean)(dim) * (*mean)(dim2));
          (*covariance)(dim2,dim) = (*covariance)(dim,dim2);
        } // account for off diagonal elements
      }
    }

    void updateMean(Vector * mean, BaseDescriptorWrapper * desc_wrap, const int current_num_desc_pts) {
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

    void updateA(Matrix * A, BaseDescriptorWrapper * desc_wrap) {
      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      assert(A->rows() == num_dims);
      assert(A->cols() == num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        { // account for diagonal
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim); 
          }
          (*A)(dim,dim) += sum;
        } // end account for diagonal
        for( int dim2=dim+1; dim2<num_dims; ++dim2) { // account for off diagonal elements
          double sum = 0.0;
          for( int pt = 0; pt<num_pts; ++pt) {
            sum += (*desc_wrap)(pt,dim) * (*desc_wrap)(pt,dim2); 
          }  
          (*A)(dim,dim2) += sum;
          (*A)(dim2,dim) += sum;
        } // account for off diagonal elements
      }
    }

    void updateB(Vector * B, BaseDescriptorWrapper * desc_wrap) {
      const int num_dims = desc_wrap->getNumberDimensions();
      const int num_pts = desc_wrap->getNumberPoints();
      assert(B->rows() == num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double sum = 0.0;
        for( int pt = 0; pt<num_pts; ++pt) {
          sum += (*desc_wrap)(pt,dim); 
        }
        (*B)(dim) = (*B)(dim) + sum; 
      }
    }
  } // namespace correlated

  Covariance::Covariance(BaseDescriptorWrapper * desc_wrap) {
    // Resize the covariance matrix based on the number of descriptor dimensions
    assert(desc_wrap != nullptr);
    const int num_dims = desc_wrap->getNumberDimensions();
    matrix_ = createMatrix(num_dims, num_dims);
    mean_ = createVector(num_dims);
    A_ = createMatrix(num_dims, num_dims);
    B_ = createVector(num_dims);

    correlated::calculateMean(mean_.get(), desc_wrap);
    correlated::calculateA(A_.get(), desc_wrap);
    correlated::calculateB(B_.get(), desc_wrap);

    matrix_->resize(num_dims, num_dims);
    matrix_->setZero();
    correlated::updateCovariance(matrix_.get(), A_.get(), B_.get(), mean_.get(), total_number_data_pts_, desc_wrap);

    // Record the total number of data points used to create the covariance matrix
    total_number_data_pts_ = desc_wrap->getNumberPoints();
    
  }

  void Covariance::update(BaseDescriptorWrapper * desc_wrap) {

    correlated::updateMean(mean_.get(), desc_wrap, total_number_data_pts_);
    correlated::updateA(A_.get(), desc_wrap);
    correlated::updateB(B_.get(), desc_wrap);
    correlated::updateCovariance(matrix_.get(), A_.get(), B_.get(), mean_.get(), total_number_data_pts_, desc_wrap);
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
}
