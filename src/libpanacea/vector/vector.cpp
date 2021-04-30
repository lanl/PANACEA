// Local public PANACEA includes
#include "panacea/vector.hpp"

// Local private PANACEA includes
#include "error.hpp"
#include "private_settings.hpp"
#include "vector_eigen.hpp"

// Standard includes
#include <any>
#include <iomanip>
#include <iostream>
#include <vector>

namespace panacea {

  std::ostream& operator<<(std::ostream& os, const VectorType& vec_type)
  {
    if( vec_type == VectorType::Eigen ) {
      os << "Eigen";
    } else if(vec_type == VectorType::Default ) {
      os << "Eigen";
    }
    return os;
  }


  std::unique_ptr<Vector> createVector(const int rows, const VectorType type) {
    auto vec_eig = std::unique_ptr<VectorEigen>(new VectorEigen);
    vec_eig->resize(rows);
    return vec_eig;
  }

  std::vector<std::any> Vector::write(
      settings::FileType file_type,
      std::ostream & os,
      std::any vector_instance) {
    
    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      Vector * vec = std::any_cast<Vector *>(vector_instance);
      os << "[Vector Type]\n";
      os << vec->type() << "\n";
      os << vec->direction() << "\n\n";
      os << "[Vector]\n";
      os << vec->rows() << " " << vec->cols() << "\n";
      for( int row = 0; row < vec->rows(); ++row ) {
        os << std::setfill(' ') 
          << std::setw(14) 
          << std::setprecision(8) 
          << std::right
          << vec->operator()(row) << " ";
        os << "\n";
      }
      os << "\n";
    } else {
      std::string error_msg = "Vector cannot be written to the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return nested_values;
  }


}
