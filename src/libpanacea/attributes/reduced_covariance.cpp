
/*
 * This class is a type of covariance matrix but with guaranteed linear independence
 */

// Private local PANACEA includes
#include "reduced_covariance.hpp"

namespace panacea {

  ReducedCovariance::ReducedCovariance() {
    matrix_ = createMatrix(0,0);
  }

  double ReducedCovariance::getDeterminant() const {
    return matrix_->getDeterminant();
  }

  int ReducedCovariance::getNumberDimensions() const {
    // Because the matrix should be square should be
    // able to return the rows or columns
    return matrix_->rows();
  }
}

