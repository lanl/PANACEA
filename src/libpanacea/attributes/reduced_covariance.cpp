
/*
 * This class is a type of covariance matrix but with guaranteed linear
 * independence
 */

// Private local PANACEA includes
#include "reduced_covariance.hpp"
#include "dimensions.hpp"

// Standard includes
#include <iostream>

namespace panacea {

double ReducedCovariance::operator()(const int row, const int col) const {
  return matrix_->operator()(row, col);
}

const Matrix &ReducedCovariance::get(PassKey<Inverter>) const {
  return *matrix_.get();
}

double ReducedCovariance::getDeterminant() const {
  return matrix_->getDeterminant();
}

int ReducedCovariance::getNumberDimensions() const {
  // Because the matrix should be square should be
  // able to return the rows or columns
  return matrix_->rows();
}

const Dimensions &ReducedCovariance::getReducedDimensions() const noexcept {
  return chosen_dimension_indices_;
}

bool ReducedCovariance::is(const NormalizationState &state) const noexcept {
  return state == normalized_;
}

void ReducedCovariance::print() const {
  std::cout << "Reduced Covariance Matrix\n";
  matrix_->print();
  std::cout << "Reduced Covariance Dimensions\n";
  for (const int dim : chosen_dimension_indices_) {
    std::cout << dim << " ";
  }
  std::cout << std::endl;
}

const NormalizationState &ReducedCovariance::getNormalizationState() const
    noexcept {
  return normalized_;
}
} // namespace panacea
