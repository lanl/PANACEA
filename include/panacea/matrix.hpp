
#ifndef PANACEA_MATRIX_H
#define PANACEA_MATRIX_H

// Standard includes
#include <memory>

namespace panacea {

  // Supported matrix types
  enum class MatrixTypes {
    Default,
    Eigen
  };

  class Matrix {

    public:
      virtual ~Matrix() {};
      virtual const MatrixTypes type() const = 0;
      virtual Matrix& operator=(const Matrix * mat) = 0;
      virtual double& operator()(const int row, const int col) = 0;
      virtual double operator()(const int row, const int col) const = 0;

      virtual double getDeterminant() const = 0;

      virtual void resize(const int rows, const int cols) = 0;

      /**
       * Turns matrix into identity matrix.
       **/
      virtual void makeIdentity() = 0;
      virtual void setZero() = 0;

      virtual int rows() const = 0;
      virtual int cols() const = 0;

      virtual void print() const = 0;
  };

  // Will create a matrix 
  std::unique_ptr<Matrix> 
    createMatrix(
        const int rows, 
        const int cols, 
        const MatrixTypes type = MatrixTypes::Default);

  // Create pseudo inverse of a matrix, will return matrix of the same type
  // as is passed in
  std::unique_ptr<Matrix> pseudoInverse(const Matrix * mat, 
      const MatrixTypes type = MatrixTypes::Default );

}

#endif // PANACEA_MATRIX_H
