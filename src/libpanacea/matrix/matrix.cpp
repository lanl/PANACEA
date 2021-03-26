
// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Local private PANACEA includes
#include "matrix_eigen.hpp"
#include "error.hpp"

namespace panacea {

  std::unique_ptr<Matrix> createMatrix(
      const int rows,
      const int cols,
      const MatrixTypes type) {

    auto mat_eig = std::unique_ptr<MatrixEigen>(new MatrixEigen);
    mat_eig->resize(rows,cols);
    return mat_eig;
  }

  std::unique_ptr<Matrix> pseudoInverse(const Matrix * mat, const MatrixTypes type) {
      if(MatrixTypes::Default != mat->type() || MatrixTypes::Eigen != mat->type()){
        PANACEA_FAIL("Pseudo inverse is not supported for specified matrix type."); 
      }
      auto new_mat = createMatrix(mat->rows(), mat->cols(), type);
      pseudoInverse(new_mat.get(), static_cast<const MatrixEigen *>(mat));
      return new_mat;
    }
}
