
// Private local PANACEA includes
#include "row_echelon.hpp"

#include "matrix/matrix.hpp"

// Standard includes
#include <algorithm>
#include <numeric>

namespace panacea {

namespace {

bool makeElementNonZero_(const int i, const int m, Matrix *tmp,
                         const double threshold, std::vector<int> &ind_rows) {

  // Check to see if current column is 0 and if we
  // need to swap with a row that is lower down in the matrix
  if (std::abs(tmp->operator()(i, m)) < threshold) {
    // This means current column is close to 0.0 we will start by going
    // ahead and rounding it to 0.0
    tmp->operator()(i, m) = 0.0;

    // Ensure there are as many columns as rows before proceeding
    if (m < tmp->cols()) {
      for (int j = i + 1; j < tmp->rows(); ++j) {
        // Find the first row j underneath row i that has a non 0
        // element in the same column (in column i).
        if (std::abs(tmp->operator()(j, m)) > threshold) {
          // Swap the rows
          for (int k = m; k < tmp->cols(); ++k) {
            std::swap(tmp->operator()(i, k), tmp->operator()(j, k));
          }
          std::swap(ind_rows.at(i), ind_rows.at(j));
          return true;
        }
      }
    } // rows j
  } else {
    return true;
  }
  return false;
}

void scaleAllRowsUnderElement_(const int i, const int m, Matrix *tmp,
                               const double threshold) {

  for (int j = i + 1; j < tmp->rows(); ++j) {
    double scale;
    if (std::abs(tmp->operator()(i, m)) < threshold) {
      scale = 0.0;
    } else {
      scale = tmp->operator()(j, m) / tmp->operator()(i, m);
    }
    for (int k = m; k < tmp->cols(); ++k) {
      tmp->operator()(j, k) =
          tmp->operator()(j, k) - scale * tmp->operator()(i, k);
      if (std::abs(tmp->operator()(j, k)) < threshold) {
        // Just make it 0.0 if it is small
        tmp->operator()(j, k) = 0.0;
      }
    }
  } // rows j
}

} // namespace

std::vector<int> RowEchelon::operate(Matrix *tmp) {
  // Used to keep track of the rows
  std::vector<int> ind_rows(tmp->rows());
  std::iota(ind_rows.begin(), ind_rows.end(), 0);

  for (int i = 0; i < tmp->rows(); ++i) {

    // Start at the diagonal but if no suitable swap is possible move
    // to the next column
    int m = i;
    for (; m < tmp->cols(); ++m) {
      if (makeElementNonZero_(i, m, tmp, threshold_, ind_rows)) {
        scaleAllRowsUnderElement_(i, m, tmp, threshold_);
        break;
      }
    }
  } // rows i
  return ind_rows;
}

} // namespace panacea
