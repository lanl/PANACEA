
// Local private PANACEA includes
#include "matrix.hpp"
#include "error.hpp"
#include "matrix_eigen.hpp"

// Local public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <iomanip>
#include <iostream>
#include <typeindex>
#include <vector>

namespace panacea {

std::ostream &operator<<(std::ostream &os, const MatrixType &mat_type) {
  if (mat_type == MatrixType::Eigen) {
    os << "Eigen";
  } else if (mat_type == MatrixType::Default) {
    os << "Eigen";
  }
  return os;
}

std::vector<std::any> Matrix::write(const settings::FileType file_type,
                                    std::ostream &os,
                                    std::any matrix_instance) {

  const Matrix &mat = [&]() -> const Matrix & {
    if (std::type_index(matrix_instance.type()) ==
        std::type_index(typeid(Matrix *))) {
      return const_cast<const Matrix &>(
          *std::any_cast<Matrix *>(matrix_instance));
    } else if (std::type_index(matrix_instance.type()) ==
               std::type_index(typeid(Matrix &))) {
      return const_cast<const Matrix &>(
          std::any_cast<Matrix &>(matrix_instance));
    } else if (std::type_index(matrix_instance.type()) ==
               std::type_index(typeid(const Matrix *))) {
      return *std::any_cast<const Matrix *>(matrix_instance);
    } else if (std::type_index(matrix_instance.type()) ==
               std::type_index(typeid(const Matrix &))) {
      return std::any_cast<const Matrix &>(matrix_instance);
    } else {
      std::string error_msg = "Unsupported Matrix type encountered.\n";
      PANACEA_FAIL(error_msg);
    }
    return std::any_cast<const Matrix &>(matrix_instance);
  }();

  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    os << "[Matrix Type]\n";
    os << mat.type() << "\n\n";
    os << "[Matrix]\n";
    os << mat.rows() << " " << mat.cols() << "\n";
    for (int row = 0; row < mat.rows(); ++row) {
      for (int col = 0; col < mat.cols(); ++col) {
        os << std::setfill(' ') << std::setw(14) << std::setprecision(15)
           << std::right << mat(row, col) << " ";
      }
      os << "\n";
    }
    os << "\n";
  }
  return std::vector<std::any>();
}

io::ReadInstantiateVector Matrix::read(const settings::FileType file_type,
                                       std::istream &is,
                                       std::any matrix_instance) {

  Matrix &mat = [&]() -> Matrix & {
    if (std::type_index(matrix_instance.type()) ==
        std::type_index(typeid(Matrix *))) {
      return *std::any_cast<Matrix *>(matrix_instance);
    } else if (std::type_index(matrix_instance.type()) ==
               std::type_index(typeid(Matrix &))) {
      return std::any_cast<Matrix &>(matrix_instance);
    } else {
      std::string error_msg = "Unsupported Matrix type encountered.\n";
      PANACEA_FAIL(error_msg);
    }
    return std::any_cast<Matrix &>(matrix_instance);
  }();

  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    std::string line = "";
    while (line.find("[Matrix Type]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg =
            "Did not find [Matrix Type] header while trying ";
        error_msg += "to read restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    std::getline(is, line);
    if (line.find("Eigen", 0) != std::string::npos) {
      if (mat.type() != MatrixType::Eigen &&
          mat.type() != MatrixType::Default) {
        std::cout << "Note reading matrix data into a different type.";
        std::cout << " Restart file indicates an Eigen Matrix was used.";
        std::cout << " Data is being loaded into a " << mat.type()
                  << " matrix.\n";
      }
    } else if (line.find("Default", 0) != std::string::npos) {
      if (mat.type() != MatrixType::Eigen &&
          mat.type() != MatrixType::Default) {
        std::cout << "Note reading matrix data into a different type.";
        std::cout << " Restart file indicates an Default Matrix was used.";
        std::cout << " Data is being loaded into a " << mat.type()
                  << " matrix.\n";
      }
    } else {
      std::cout << "Warning matrix type value was not specified under the ";
      std::cout << "[Matrix Type] header, or it was unrecognized. ";
      std::cout << "Attempting to load data into a matrix of type ";
      std::cout << mat.type() << "\n";
    }

    while (line.find("[Matrix]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg = "Did not find [Matrix] header while trying ";
        error_msg += "to read restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    std::getline(is, line);
    int rows;
    int cols;
    {
      std::istringstream ss(line);
      try {
        ss >> rows;
      } catch (...) {
        std::string error_msg =
            "Unable to read in matrix rows from Restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }
      try {
        ss >> cols;
      } catch (...) {
        std::string error_msg =
            "Unable to read in matrix columns from Restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }
      mat.resize(rows, cols);
    }

    try {
      for (int row = 0; row < rows; ++row) {
        std::getline(is, line);
        std::istringstream ss_data(line);
        for (int col = 0; col < cols; ++col) {
          double value;
          ss_data >> value;
          mat(row, col) = value;
        }
      }
    } catch (...) {
      std::string error_msg =
          "Error encountered while attempting to read in matrix ";
      error_msg += "coefficients from restart file.\n";
      error_msg += "line is: " + line + "\n";
      PANACEA_FAIL(error_msg);
    }

  } else {
    std::string error_msg =
        "Matrix cannot be read from the specified file type.";
    PANACEA_FAIL(error_msg);
  }
  return io::ReadInstantiateVector();
}

std::unique_ptr<Matrix> createMatrix(const int rows, const int cols,
                                     const MatrixType type) {

  if (rows < 0) {
    std::string error_msg = "Error cannot create matrix with negative rows.";
    PANACEA_FAIL(error_msg);
  }
  if (cols < 0) {
    std::string error_msg = "Error cannot create matrix with negative columns.";
    PANACEA_FAIL(error_msg);
  }
  auto mat_eig = std::unique_ptr<MatrixEigen>(new MatrixEigen);
  mat_eig->resize(rows, cols);
  return mat_eig;
}

std::unique_ptr<Matrix> pseudoInverse(const Matrix &mat,
                                      const MatrixType type) {
  auto new_mat = createMatrix(mat.rows(), mat.cols(), type);
  if (mat.type() == MatrixType::Eigen or mat.type() == MatrixType::Default) {
    pseudoInverse(*new_mat, dynamic_cast<const MatrixEigen &>(mat));
  } else {
    PANACEA_FAIL("Unsupported matrix type for pseudo inverse.");
  }
  return new_mat;
}
} // namespace panacea
