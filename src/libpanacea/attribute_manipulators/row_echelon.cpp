
// Private local PANACEA includes
#include "row_echelon.hpp"

// Public local PANACEA includes
#include "panacea/matrix.hpp"

namespace panacea {

  namespace {

    void makeDiagonalElementNonZero_(
        const int index,
        Matrix * tmp,
        const double threshold,
        std::vector<int> ind_rows
        ){

      // Check to see if current column is 0 and if we
      // need to swap with a row that is lower down in the matrix
      if( std::abs(tmp->operator(i,i)) < threshold ) {
        // This means current column is close to 0.0 we will start by going 
        // ahead and rounding it to 0.0
        tmp->operator(i,i) = 0.0;
      
        // Ensure there are as many columns as rows before proceeding
        if(i < tmp->cols() ) { 
          for(int j = i+1; j<tmp->rows(); ++j){
            // Find the first row j underneath row i that has a non 0 
            // element in the same column (in column i).
            if( std::abs(tmp->operator(j,i)) > threshold) {
              // Swap the rows
              for( int k = 0; k <tmp->cols(); ++k){
                std::swap(tmp->operator(i,k), tmp->operator(j,k));
              }
              std::swap(ind_rows.at(i)<ind_rows.at(j));
            }
          }
        } // rows j
      }
    }

    void scaleAllRowsUnderDiagonal_(
        const int i, 
        Matrix * tmp,
        const double threshold
        ){

      for(int j = i+1; j<tmp->rows(); ++j) {
        double scale;
        if ( std::abs(tmp->operator(j,i)) < threshold &&
            std::abs(tmp->operator(i,i)) < threshold ) {
          scale = 0.0;
        } else { 
          scale = tmp->operator(j,i) / tmp->operator(i,i);
        }
        for( int k = i; k<tmp->cols(); ++k) {
          tmp->operator(j,k) = tmp->operator(j,k) - scale * tmp->operator(i,k);
          if( std::abs(tmp->operator(j,k)) < threshold ) {
            // Just make it 0.0 if it is small
            tmp->operator(j,k) = 0.0;
          }
        } 
      } // rows j
    }

  } // namespace 

  std::vector<int> RowEchelon::operate(Matrix * tmp) {
      // Used to keep track of the rows
      std::vector<int> ind_rows(tmp->rows());
      std::iota(ind_rows.begin(), ind_rows.end(), 0);

      for(int i = 0; i< tmp->rows(); ++i){
        makeDiagonalElementNonZero_(i, tmp, threshold_, ind_rows);
        scaleAllRowsUnderDiagonal_(i, tmp, threshold_);
      } // rows i
      return ind_rows;
  }

} // namespace panacea
