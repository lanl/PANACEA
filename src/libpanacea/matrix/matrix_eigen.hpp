
#ifndef PANACEA_PRIVATE_MATRIXEIGEN_H
#define PANACEA_PRIVATE_MATRIXEIGEN_H
#pragma once

// Local PANACEA includes
#include "panacea/matrix.hpp"

// Third party includes
#include <Eigen/Dense>

namespace panacea {

  /**
   * Here the underlying matrix is owned by the memory manager so we only have a pointer
   * here.
   */
  class MatrixEigen : public Matrix {
    private: 
      Eigen::MatrixXd * matrix_;
    public:
      MatrixEigen(Eigen::MatrixXd * matrix) : matrix_(matrix) {};

      virtual double& operator()(const int row, const int col) final;
      virtual double operator()(const int row, const int col) const final;

      virtual double getDeterminant() const final;

      virtual void resize(const int rows, const int cols) final;

      virtual void setZero() final;
  };
}

#endif // PANACEA_PRIVATE_MATRIXEIGEN_H
