// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "attributes/covariance.hpp"
#include "error.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

namespace panacea {
  namespace covariance {

    void calculateMean(Vector & mean, const BaseDescriptorWrapper & desc_wrap) {
      const int num_dims = desc_wrap.getNumberDimensions();
      const int num_pts = desc_wrap.getNumberPoints();
      mean.resize(num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double sum = 0.0;
        for( int pt = 0; pt<num_pts; ++pt) {
          sum += desc_wrap(pt, dim);
        }
        mean(dim) = sum/static_cast<double>(num_pts);
      }
    }

    void updateMean(
        Vector & mean,
        const BaseDescriptorWrapper & desc_wrap,
        const int current_num_desc_pts) {

      const int num_dims = desc_wrap.getNumberDimensions();
      const int num_pts = desc_wrap.getNumberPoints();
      assert(mean.rows() == num_dims);
      for( int dim=0; dim<num_dims; ++dim){
        double new_sum = 0.0;
        for( int pt = 0; pt<num_pts; ++pt) {
          new_sum += desc_wrap(pt, dim);
        }
        double old_sum = mean(dim) * static_cast<double>(current_num_desc_pts);
        mean(dim) = (old_sum + new_sum)/static_cast<double>(num_pts + current_num_desc_pts);
      }
    }

    void checkForSymmetry(
        Matrix & matrix,
        const double threshold) {

#ifndef NDEBUG
      for( int i = 0; i < matrix.rows(); ++i) {
        for( int j = i+1; j < matrix.rows(); ++j) {
          // Don't need to check the diagonal
          if( std::abs(matrix(i,j)) > threshold) {
            // Only check values that are not too close to 0.0
            const double diff = std::abs(matrix(i,j) - matrix(j,i));
            const double avg =  std::abs(matrix(i,j) + matrix(j,i))/2.0;
            if(diff/avg > threshold){
              PANACEA_FAIL("Covariance symmetry criteria not satisfied");
            }
          }
        }
      }
#endif
    }

    void accountForZeroMatrix(
        const CovarianceOption opt,
        Matrix & matrix){

      if( opt == CovarianceOption::Flexible ) {
        matrix.makeIdentity();
      } else {
        std::string error_msg = "Covariance matrix is 0 everywhere, likely because";
        error_msg += " your data is stacked. Either get a better starting data set";
        error_msg += " or allow flexibility in the algorithm.";
        PANACEA_FAIL(error_msg);
      }
    }

    void accountForZeroOnDiagonal(
        const CovarianceOption opt,
        Matrix & matrix) {
      for( int dim1 = 0; dim1 < matrix.rows(); ++dim1){
        if( std::abs(matrix(dim1,dim1)) < 1E-9 ){
          if ( opt == CovarianceOption::Flexible ) {
            matrix(dim1,dim1) = 1.0;
          } else {
            std::string error_msg = "Zero's detected in the diagonal of covariance matrix. ";
            error_msg += " Either get a better starting data set or allow flexibility in the";
            error_msg += " covariance matrix algorithm.";
            PANACEA_FAIL(error_msg);
          }
        }
      }
    }

    void printCovariance(
        const Matrix & matrix,
        const Vector & mean,
        const int total_number_data_pts) {
      std::cout << "Covariance Matrix\n";
      matrix.print();
      std::cout << "\n";
      std::cout << "Mean\n";
      mean.print();
      std::cout << "\n";
      std::cout << "Total Number of Points\n";
      std::cout << total_number_data_pts;
      std::cout << std::endl;
    }

  } // namespace covariance
} // namespace panacea
