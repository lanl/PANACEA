
#ifndef PANACEA_PRIVATE_MATRIXEIGEN_H
#define PANACEA_PRIVATE_MATRIXEIGEN_H
#pragma once

// Local PANACEA includes
#include "matrix/matrix.hpp"

// Third party includes
#include <Eigen/Dense>

// Standard includes
#include <memory>

namespace panacea {

class MatrixEigen : public Matrix {
private:
  std::unique_ptr<Eigen::MatrixXd> matrix_;

public:
  MatrixEigen();
  virtual ~MatrixEigen() final{};
  virtual const MatrixType type() const final;
  virtual MatrixEigen &operator=(const MatrixEigen &mat) final;
  virtual MatrixEigen &operator=(const Matrix *mat) final;
  virtual double &operator()(const int row, const int col) final;
  virtual double operator()(const int row, const int col) const final;

  virtual double getDeterminant() const final;

  virtual void resize(const int rows, const int cols) final;

  virtual void makeIdentity() final;
  virtual void setZero() final;

  virtual int rows() const final;
  virtual int cols() const final;

  virtual void print() const final;

  // Local method should not be part of the interface
  Eigen::MatrixXd pseudoInverse() const;
};

void pseudoInverse(Matrix *return_mat, const MatrixEigen *mat);

} // namespace panacea

#endif // PANACEA_PRIVATE_MATRIXEIGEN_H
