
// Local private PANACEA includes
#include "matrix_eigen.hpp"

// Third party includes
#include <Eigen/Dense>

// Standard includes
#include <cassert>

namespace panacea {

  double MatrixEigen::getDeterminant() const {
    assert(matrix_->rows() > 0 || matrix_->cols() > 0 );
    return matrix_->determinant();
  }

  void MatrixEigen::resize(const int rows, const int cols) {
    assert(rows>=0);
    assert(cols>=0);
    matrix_->resize(rows,cols);
  }

  double & MatrixEigen::operator()(const int row, const int col) {
    assert(row>=0);
    assert(col>=0);
    assert(row<matrix_->rows());
    assert(col<matrix_->cols());
    return matrix_->(row,col);
  }

  double MatrixEigen::operator()(const int row, const int col) const {
    assert(row>=0);
    assert(col>=0);
    assert(row<matrix_->rows());
    assert(col<matrix_->cols());
    return matrix_->(row,col);
  }

  void MatrixEigen::setZero() {
    matrix_->setZero();
  }
}

