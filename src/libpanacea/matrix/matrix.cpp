
// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Local private PANACEA includes
#include "matrix_eigen.hpp"
#include "error.hpp"

// Standard includes
#include <any>
#include <iomanip>
#include <iostream>
#include <vector>

namespace panacea {

  std::ostream& operator<<(std::ostream& os, const MatrixType& mat_type)
  {
    if( mat_type == MatrixType::Eigen ) {
      os << "Eigen";
    } else if(mat_type == MatrixType::Default ) {
      os << "Eigen";
    }
    return os;
  }

  std::vector<std::any> Matrix::write(
      settings::FileType file_type,
      std::ostream & os,
      std::any matrix_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      Matrix * mat = std::any_cast<Matrix *>(matrix_instance);
      os << "[Matrix Type]\n";
      os << mat->type() << "\n\n";
      os << "[Matrix]\n";
      os << mat->rows() << " " << mat->cols() << "\n";
      for( int row = 0; row < mat->rows(); ++row ) {
        for( int col = 0; col < mat->cols(); ++col ) {
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << mat->operator()(row,col) << " ";
        }
        os << "\n";
      }
      os << "\n";
    } else {
      std::string error_msg = "Matrix cannot be written to the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return nested_values;
  }


  std::unique_ptr<Matrix> createMatrix(
      const int rows,
      const int cols,
      const MatrixType type) {

    auto mat_eig = std::unique_ptr<MatrixEigen>(new MatrixEigen);
    mat_eig->resize(rows,cols);
    return mat_eig;
  }

  std::unique_ptr<Matrix> pseudoInverse(const Matrix * mat, const MatrixType type) {
      if(MatrixType::Default != mat->type() && MatrixType::Eigen != mat->type()){
        PANACEA_FAIL("Pseudo inverse is not supported for specified matrix type."); 
      }
      auto new_mat = createMatrix(mat->rows(), mat->cols(), type);
      pseudoInverse(new_mat.get(), static_cast<const MatrixEigen *>(mat));
      return new_mat;
    }
}
