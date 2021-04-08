
// Local private PANACEA includes
#include "reduced_inv_covariance.hpp"

// Standard includes
#include <iostream>

namespace panacea {

  double ReducedInvCovariance::operator()(const int row, const int col) const {
    return (*matrix_)(row,col);
  }

  int ReducedInvCovariance::getNumberDimensions() const {
    // Because the matrix should be square should be
    // able to return the rows or columns
    return matrix_->rows();
  }

  const std::vector<int> & ReducedInvCovariance::getChosenDimensionIndices() const {
    return chosen_dimension_indices_;
  }

  void ReducedInvCovariance::print() const {
    std::cout << "Reduced Inverse Covariance Matrix\n";
    matrix_->print();
    std::cout << "Reduced Inverse Covariance Dimensions\n";
    for( int dim : chosen_dimension_indices_) {
      std::cout << dim << " ";
    } 
    std::cout << std::endl;
  }

  bool ReducedInvCovariance::is(const NormalizationState & state) const noexcept {
    return normalized_ == state;
  }

  const NormalizationState & ReducedInvCovariance::getNormalizationState() const noexcept {
    return normalized_;
  }
}
