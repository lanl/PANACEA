
#ifndef PANACEA_MATRIX_H
#define PANACEA_MATRIX_H
#pragma once

namespace panacea {

  class Matrix {

    public:

      virtual double& operator()(const int row, const int col) = 0;
      virtual double operator()(const int row, const int col) const = 0;

      virtual double getDeterminant() const = 0;

      virtual void resize(const int rows, const int cols) = 0;

      virtual void setZero() = 0;
  };
}

#endif // PANACEA_MATRIX_H
