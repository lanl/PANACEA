
// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Local private PANACEA includes
#include "matrix_eigen.hpp"

namespace panacea {

  std::unique_ptr<Matrix> createMatrix(const int rows, const int cols, const MatrixTypes type) {
    auto mat_eig = std::unique_ptr<MatrixEigen>(new MatrixEigen);
    mat_eig->resize(rows,cols);
    return mat_eig;
  }
}
