
// Local private PANACEA includes
#include "matrix_eigen.hpp"

// Third party includes
#include <Eigen/Dense>

// Standard includes
#include <cassert>
#include <iostream>
#include <memory>

namespace panacea {

MatrixEigen::MatrixEigen() { matrix_ = std::make_unique<Eigen::MatrixXd>(); }

const MatrixType MatrixEigen::type() const { return MatrixType::Eigen; }

double MatrixEigen::getDeterminant() const {
  assert(matrix_->rows() > 0 || matrix_->cols() > 0);
  return matrix_->determinant();
}

void MatrixEigen::resize(const int rows, const int cols) {
  assert(rows >= 0);
  assert(cols >= 0);
  matrix_->resize(rows, cols);
}

MatrixEigen &MatrixEigen::operator=(const MatrixEigen &mat) {
  this->resize(mat.rows(), mat.cols());
  for (int row = 0; row < mat.rows(); ++row) {
    for (int col = 0; col < mat.cols(); ++col) {
      this->operator()(row, col) = mat(row, col);
    }
  }
  return *this;
}

MatrixEigen &MatrixEigen::operator=(const Matrix *mat) {
  this->resize(mat->rows(), mat->cols());
  for (int row = 0; row < mat->rows(); ++row) {
    for (int col = 0; col < mat->cols(); ++col) {
      this->operator()(row, col) = mat->operator()(row, col);
    }
  }
  return *this;
}

double &MatrixEigen::operator()(const int row, const int col) {
  assert(row >= 0);
  assert(col >= 0);
  assert(row < matrix_->rows());
  assert(col < matrix_->cols());
  return (*matrix_)(row, col);
}

double MatrixEigen::operator()(const int row, const int col) const {
  assert(row >= 0);
  assert(col >= 0);
  assert(row < matrix_->rows());
  assert(col < matrix_->cols());
  return (*matrix_)(row, col);
}

void MatrixEigen::makeIdentity() {
  for (int row = 0; row < matrix_->rows(); ++row) {
    this->operator()(row, row) = 1.0;
    for (int col = row + 1; col < matrix_->cols(); ++col) {
      this->operator()(row, col) = 0.0;
      this->operator()(col, row) = 0.0;
    }
  }
}

void MatrixEigen::setZero() { matrix_->setZero(); }

int MatrixEigen::rows() const { return matrix_->rows(); }

int MatrixEigen::cols() const { return matrix_->cols(); }

void MatrixEigen::print() const { std::cout << *matrix_ << std::endl; }

Eigen::MatrixXd MatrixEigen::pseudoInverse() const {
  return matrix_->completeOrthogonalDecomposition().pseudoInverse();
}

void pseudoInverse(Matrix *return_mat, const MatrixEigen *mat) {
  assert(return_mat != nullptr);
  assert(return_mat->rows() == mat->rows());
  assert(return_mat->cols() == mat->cols());

  auto temp_mat = mat->pseudoInverse();
  for (int row = 0; row < temp_mat.rows(); ++row) {
    for (int col = 0; col < temp_mat.cols(); ++col) {
      return_mat->operator()(row, col) = temp_mat(row, col);
    }
  }
}
} // namespace panacea
