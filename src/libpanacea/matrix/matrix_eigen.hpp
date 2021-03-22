
#ifndef PANACEA_PRIVATE_MATRIXEIGEN_H
#define PANACEA_PRIVATE_MATRIXEIGEN_H
#pragma once

// Local PANACEA includes
#include "panacea/matrix.hpp"

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
      virtual ~MatrixEigen() final {};

      virtual double& operator()(const int row, const int col) final;
      virtual double operator()(const int row, const int col) const final;

      virtual double getDeterminant() const final;

      virtual void resize(const int rows, const int cols) final;

      virtual void setZero() final;

      virtual int rows() const final;
      virtual int cols() const final;

      virtual void print() const final;
  };
}

#endif // PANACEA_PRIVATE_MATRIXEIGEN_H
