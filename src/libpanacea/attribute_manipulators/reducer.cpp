
// Local private PANACEA includes
#include "reducer.hpp"

#include "attribute_manipulators/row_echelon.hpp"
#include "error.hpp"

// Public PANACEA includes
#include "panacea/matrix.hpp"

// Standard includes
#include <algorithm>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <utility>
#include <vector>

namespace panacea {

  namespace {

    /*
     * Runs a number of checks to ensure the input is allowed
     */
    void runChecks_(
          const Covariance & cov,
          const std::vector<int> & preferred_dimensions) {

      // The first two checks are unnecessary because by definition of how the
      // covariance matrix is created, it shoudl be square and symmetric
      // Check 1 ensure the matrix is square
      // Check 2 ensure the matrix is symmetric
      // Check 3 ensure the preferred_dimensions exist within the matrix, as
      //         in the indices are in bounds
      // Check 4 ensure that the prefferred dimensions are unique, there
      //         are not repeated entries
      // Check 5 ensure that the covariance matrix does not contain 0's on the diagonal
      std::string error_msg = "";
      bool error_detected = false;
      for( const auto & dim : preferred_dimensions) {
        if( dim < 0 || dim >= cov.rows()) {
          error_detected = true;
          error_msg += "Dimension " + std::to_string(dim) + " is out of bounds for covariance ";
          error_msg += "matrix of size " + std::to_string(cov.rows()) + ".\n";
        }
      }
      if(error_detected){
        PANACEA_FAIL(error_msg);
      }
      if(cov.rows() == 0) {
        PANACEA_FAIL("Reducer cannot reduce a covariance matrix of size 0");
      }

      std::unordered_set<int> unique_vals;
      int count = 0;
      for( const auto & dim : preferred_dimensions) {
        unique_vals.insert(dim);
        ++count;
        if(unique_vals.size() != count) {
          std::string error_msg = "Preferred dimensions are not unique ";
          error_msg += "cannot reduce matrix when specified dimensions ";
          error_msg += "have duplicates.";
          PANACEA_FAIL(error_msg); 
        }
      } 

      for ( int dim = 0; dim < cov.rows(); ++dim ){
        if( cov(dim,dim) == 0.0 ) {
          cov.print();
          std::string error_msg = "Trying to reduce covariance matrix, ";
          error_msg += "but candidate covariance matrix has 0's along ";
          error_msg += "the diagonal.";
          PANACEA_FAIL(error_msg);
        }
      }
    }

    // After calling this function the matrix should be reordered such that 
    // the priorty rows columns are moved to the top and left side of the matrix
    //
    // E.g. 
    //
    // Mat1 
    //
    // 0 0 0 4 0
    // 0 0 0 4 0
    // 0 0 0 4 0
    // 4 4 4 5 4
    // 0 0 0 4 0
    //
    // If priority row is 3 (starts at 0) then the matrix should rearrange so 
    // that we have
    //
    // 5 4 4 4 4
    // 4 0 0 0 0
    // 4 0 0 0 0
    // 4 0 0 0 0
    // 4 0 0 0 0
    //
    std::vector<int> reorderSymmetricMatrix_(
        Matrix * mat, const std::vector<int> & priority_rows) {

      // Next we need to keep track of where the original rows are located in the
      // covariance matrix 
      std::vector<int> ind_rows;
      ind_rows.reserve(mat->rows());
      for( int ind =0; ind< mat->rows(); ++ind){
        ind_rows.push_back(ind);
      }
      std::vector<int> ind_cols = ind_rows;

      // Priority rows
      // ind 0, 1, 2, 3, 4
      // row 4, 0, 5, 2, 3

      // ind_rows
      // 0, 1, 2, 3, 4, 5 ..
      std::vector<int> priority_rows_tmp1 = priority_rows;
      for(int index=0; index < priority_rows_tmp1.size(); ++index ){
        int chosen_row = priority_rows_tmp1.at(index);

        int index_of_chosen = 0;
        for(;index_of_chosen<mat->rows();++index_of_chosen){ 
          if(ind_rows.at(index_of_chosen) == chosen_row) { break; }
        }

        if( index_of_chosen != index ) {
          for( int col = 0; col < mat->cols(); ++col){
            std::swap(mat->operator()(index,col),mat->operator()(index_of_chosen,col));
          }
          std::swap(ind_rows.at(index), ind_rows.at(index_of_chosen));
        }
      }

      std::vector<int> priority_rows_tmp2 = priority_rows;
      for(int index=0; index < priority_rows_tmp2.size(); ++index ){
        int chosen_col = priority_rows_tmp2.at(index);
        int index_of_chosen = 0;
        for(;index_of_chosen<mat->cols();++index_of_chosen){
          if(ind_cols.at(index_of_chosen) == chosen_col) { break; }
        }
        if ( index_of_chosen != index ) {
          for( int row = 0; row < mat->rows(); ++row){
            std::swap(mat->operator()(row, index),mat->operator()(row,index_of_chosen));
          }
          std::swap(ind_cols.at(index), ind_cols.at(index_of_chosen));
        }
      }
      return ind_rows;
    }

    std::vector<int> detectIndependentRows_(
        Matrix * tmp,
        const std::vector<int> & ind_rows,
        const double threshold
        ) {

      // Now we will determine which rows are actually independent
      // The order does not matter
      std::vector<int> independent_rows;
      for(int i = 0; i<tmp->rows(); ++i){
        for(int k=0; k<tmp->cols(); ++k){
          if( std::abs(tmp->operator()(i,k)) > threshold){
            independent_rows.push_back(ind_rows.at(i));
            break;
          }
        } // cols k
      } // rows i
      return independent_rows;
    }

    /*
     * Will detect linearly independent rows, the threshold determines
     * what constitutes a 0. 
     */
    std::vector<int> rowEchelonDimensionDependenceDetection_(
        Matrix * tmp,
        const double threshold) {

      RowEchelon row_echelon(threshold);
      const std::vector<int> ind_rows = row_echelon.operate(tmp);
      return detectIndependentRows_(tmp, ind_rows, threshold);
    }

    /*
     * We need to check the covariance matrix and see if it is singular
     * if it is we need to identify the descriptors we are most interested in 
     *
     * We will then rearrange the covariance matrix to give priority to the 
     * descriptor dimensions we are most interested in 
     * We will then put the covariance matrix in row echelon form, while identifying which
     * descriptor dimensions are linearly independent.
     *
     * Once we have got the descriptors we will build a smaller covariance matrix
     * with the linearly independent descriptors. 
     *
     * When we rearrange the covariance matrix rows we will also have to rearrange the
     * columns (because it is a covariance matrix. 
     */ 
    std::vector<int> findLinearlyIndependentDescDimensions_(
        const Covariance & cov,
        const std::vector<int> & priority_rows,
        const double threshold){

      auto tmp = createMatrix(cov.rows(),cov.cols());

      for ( int ind1=0;ind1<cov.rows();++ind1){
        for ( int ind2=ind1;ind2<cov.rows();++ind2){
          tmp->operator()(ind1,ind2) = cov(ind1,ind2);
          tmp->operator()(ind2,ind1) = tmp->operator()(ind1,ind2);
        }
      }

      // Next we are going to reorder the covariance matrix
      std::vector<int> order = reorderSymmetricMatrix_(tmp.get(), priority_rows);

      tmp->print();
      // E.g. 
      // order.at(0) is row of reordered tmp that points back to the original tmp matrix
      // before it was changed which in this case is equivalent to covariance_
      // now we are going to get the rows that are linearly indpendent

      // Note that placing in row Echelon form may result in some of the rows being
      // swapped, because the diagonal may not be non zero at the dimension of interest
      // this does not constitute an error 
      const std::vector<int> relative_independent_rows =
        rowEchelonDimensionDependenceDetection_(tmp.get(), threshold);

      // relative_independent_rows has not accounted for the changes in rows that occurred
      // during the call to reorderSymmetricMatrix this corrects the row indices
      std::unordered_set<int> converted; 
      for( const auto & rel_ind : relative_independent_rows){
        converted.insert(order.at(rel_ind));
      }
      // where  relative_independent_rows.at(0) points back to the original index in tmp 
      // before it was put into row echelon form

      // We need to translate the independent rows from tmp back to the covariance matrix
      std::vector<int> independent_rows;
      for( const auto & prior : priority_rows) {
        if(converted.count(prior) ) {  
          independent_rows.push_back(prior);
        }
      }
      return independent_rows;
    }

    std::unique_ptr<Matrix> createRedCovarRawMatrix_(
      const Covariance & cov,
      std::vector<int> independent_rows) {

      const int len = independent_rows.size();

      auto reduced_covar_raw_mat = createMatrix(len, len);
      // Also need to know which reduced rows map to which dimensions of the original
      // covariance matrix
      int reduced_row = 0;
      for( int ind_row : independent_rows){
        int reduced_col = 0;
        for( int ind_col : independent_rows){
          reduced_covar_raw_mat->operator()(reduced_row, reduced_col) = cov(ind_row,ind_col);
          reduced_covar_raw_mat->operator()(reduced_col, reduced_row) = cov(ind_col,ind_row);
          ++reduced_col; 
        }
        ++reduced_row; 
      }
      return reduced_covar_raw_mat;
    }
  } // namespace

  ReducedCovariance Reducer::reduce(
      const Covariance & cov,
      std::vector<int> priority_rows) const {

    // If priority rows are empty create some assume the current order is fine
    if(priority_rows.size() == 0){
      priority_rows.resize(cov.rows());
      std::iota(priority_rows.begin(), priority_rows.end(), 0);
    }

    runChecks_(cov, priority_rows);

    double threshold = starting_threshold_;
    do {
      std::vector<int> independent_dims = findLinearlyIndependentDescDimensions_(
          cov,
          priority_rows,
          threshold);

      if( independent_dims.size() == 0) {
        independent_dims.push_back(priority_rows.at(0));
      }

      auto raw_mat = createRedCovarRawMatrix_(cov, independent_dims);

      auto reduced_covar = ReducedCovariance(
          PassKey<Reducer>(),
          std::move(raw_mat),
          independent_dims,
          cov.getNormalizationState());

      if( reduced_covar.getDeterminant() > 0.0 ) {
        return reduced_covar;
      }

      threshold *= 10.0;
    } while(1);

    PANACEA_FAIL("FATAL error in reducer, this should never occur, no reduced covariance matrix is possible.");
  }
}

