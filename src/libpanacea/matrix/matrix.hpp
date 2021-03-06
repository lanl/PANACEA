
#ifndef PANACEA_MATRIX_H
#define PANACEA_MATRIX_H
#pragma once

// Public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/settings.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <memory>
#include <vector>

namespace panacea {

// Supported matrix types
enum class MatrixType { Default, Eigen };

class Matrix {

public:
  virtual ~Matrix(){};
  virtual const MatrixType type() const = 0;
  virtual Matrix &operator=(const Matrix &mat) = 0;
  virtual double &operator()(const int row, const int col) = 0;
  virtual double operator()(const int row, const int col) const = 0;

  virtual double getDeterminant() const = 0;

  /**
   * Check if all elements in a matrix are 0, a threshold value can be
   * specefied to help define what constitutes 0.
   **/
  virtual bool isZero(const double threshold) const noexcept = 0;

  /**
   * Resize the matrix to the specified rows and columns
   *
   * If rows or cols is less than 0 assertions will trigger, note that no
   *guarantees are placed on the contents of the matrix, you will need to call
   *setZero if you want the values of the matrix to be set to 0.0.
   **/
  virtual void resize(const int rows, const int cols) = 0;

  /**
   * Turns matrix into identity matrix.
   **/
  virtual void makeIdentity() = 0;
  virtual void setZero() = 0;

  virtual int rows() const = 0;
  virtual int cols() const = 0;

  virtual void print() const = 0;

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &, std::any matrix_instance);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &,
                                        std::any matrix_instance);
};

std::ostream &operator<<(std::ostream &os, const MatrixType &mat_type);

// Will create a matrix
std::unique_ptr<Matrix>
createMatrix(const int rows, const int cols,
             const MatrixType type = MatrixType::Default);

// Create pseudo inverse of a matrix, will return matrix of the same type
// as is passed in
std::unique_ptr<Matrix>
pseudoInverse(const Matrix &mat, const MatrixType type = MatrixType::Default);

} // namespace panacea

#endif // PANACEA_MATRIX_H
