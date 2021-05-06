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
      const settings::FileType & file_type,
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
      if( vec->direction() == Direction::AlongRows ) {
        for( int row = 0; row < vec->rows(); ++row ) {
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << vec->operator()(row);
          os << "\n";
        }
      } else {
        for( int col = 0; col < vec->cols(); ++col ) {
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << vec->operator()(col);
          os << " ";
        }
      }
      
      os << "\n";
    } else {
      std::string error_msg = "Vector cannot be written to the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return nested_values;
  }

  io::ReadInstantiateVector Vector::read(
      const settings::FileType & file_type,
      std::istream & is,
      std::any vector_instance) {

    if( file_type == settings::FileType::TXTRestart ) {
      Vector * vec = std::any_cast<Vector *>(vector_instance);
      std::string line = "";
      while(line.find("[Vector Type]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Vector Type] header while trying ";
          error_msg += "to read restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      std::getline(is, line);
      if( line.find("Eigen",0) != std::string::npos){
        if(vec->type() != VectorType::Eigen && vec->type() != VectorType::Default) {
          std::cout << "Note reading vector data into a different type.";
          std::cout << " Restart file indicates an Eigen Vector was used.";
          std::cout << " Data is being loaded into a " << vec->type() << " vector.\n";
        }
      }else if(line.find("Default",0) != std::string::npos ) {
        if(vec->type() != VectorType::Eigen && vec->type() != VectorType::Default) {
          std::cout << "Note reading vector data into a different type.";
          std::cout << " Restart file indicates an Default Vector was used.";
          std::cout << " Data is being loaded into a " << vec->type() << " vector.\n";
        }
      } else {
        std::cout << "Warning vector type value was not specified under the ";
        std::cout << "[Vector Type] header, or it was unrecognized. ";
        std::cout << "Attempting to load data into a vector of type ";
        std::cout << vec->type() << "\n";
      }
 
      std::getline(is, line);
      if( line.find("Along Rows",0) != std::string::npos){
        vec->direction(Direction::AlongRows);  
      }else if(line.find("Along Columns",0) != std::string::npos ) {
        vec->direction(Direction::AlongColumns);  
      } else {
        std::cout << "Warning vector direction value was not specified under the ";
        std::cout << "[Vector Type] header, or it was unrecognized. ";
        std::cout << "Assuming Row vector.\n";
        vec->direction(Direction::AlongRows);  
      }

      while(line.find("[Vector]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Vector] header while trying ";
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
          std::string error_msg = "Unable to read in vector rows from Restart file.\n";
          error_msg += "line is: " + line + "\n";
          PANACEA_FAIL(error_msg);
        }
        try {
          ss >> cols;
        } catch (...) {
          std::string error_msg = "Unable to read in vector cols from Restart file.\n";
          error_msg += "line is: " + line + "\n";
          PANACEA_FAIL(error_msg);
        }
        if( vec->direction() == Direction::AlongRows ) {
          vec->resize(rows);
        } else {
          vec->resize(cols);
        }
      }

      try { 
        for( int row = 0; row < rows; ++row) {
          std::getline(is, line);
          std::istringstream ss_data(line);
          for( int col = 0; col < cols; ++col) {
            double value;
            ss_data >> value;
            if( vec->direction() == Direction::AlongRows ) {
              vec->operator()(row) = value;
            } else {
              vec->operator()(col) = value;
            }
          }
        } 
      } catch (...) {
        std::string error_msg = "Error encountered while attempting to read in vector ";
        error_msg += "coefficients from restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }

    } else {
      std::string error_msg = "Vector cannot be read from the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    io::ReadInstantiateVector nested_values;
    return nested_values;
  }
}
