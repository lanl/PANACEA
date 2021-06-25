
// Local private includes
#include "base_kernel_wrapper.hpp"

#include "data_point_template.hpp"
#include "error.hpp"
#include "private_settings.hpp"
#include "type_map.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <typeindex>

namespace panacea {

  std::vector<std::any> BaseKernelWrapper::write(
      const settings::FileType file_type,
      std::ostream & os,
      std::any kwrapper_instance) {

    const BaseKernelWrapper & kwrapper = [&]() -> const BaseKernelWrapper & {
      if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(BaseKernelWrapper *))){
        return const_cast<const BaseKernelWrapper &>(
            *std::any_cast<BaseKernelWrapper *>(kwrapper_instance));

      } else if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(BaseKernelWrapper &))) {
        return const_cast<const BaseKernelWrapper &>(
            std::any_cast<BaseKernelWrapper &>(kwrapper_instance));

      } else if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(const BaseKernelWrapper *))){
        return *std::any_cast<const BaseKernelWrapper *>(kwrapper_instance);

      } else if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(const BaseKernelWrapper &))) {
        return std::any_cast<const BaseKernelWrapper &>(kwrapper_instance);

      } else {
        std::string error_msg = "Unsupported Base kernel wrapper type.";
        PANACEA_FAIL(error_msg);
      }
      return std::any_cast<const BaseKernelWrapper &>(kwrapper_instance);
    }();

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart || 
        file_type == settings::FileType::TXTKernelDistribution ) {

      os << "[Kernel]\n";
      os << kwrapper.center() << "\n";
      os << kwrapper.count() << "\n";
      os << "[Meta Data]\n";
      // Write out derived class specific data
      kwrapper.getWriteFunction_()(kwrapper, os);
      os << "[Data]\n";
      os << kwrapper.arrangement() << "\n";
      os << kwrapper.rows() << " " << kwrapper.cols() << "\n";
      for( int row = 0; row < kwrapper.rows(); ++row ) {
        for( int col = 0; col < kwrapper.cols(); ++col ) {
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << kwrapper.at(row,col) << " ";
        }
        os << "\n";
      }
    } 
    return nested_values;
  }

  io::ReadInstantiateVector BaseKernelWrapper::read(
      const settings::FileType file_type,
      std::istream & is,
      std::any kwrapper_instance) {

    BaseKernelWrapper & kwrapper = [&]() -> BaseKernelWrapper & {
      if(std::type_index(kwrapper_instance.type()) == 
          std::type_index(typeid(BaseKernelWrapper *))){
        return *std::any_cast<BaseKernelWrapper *>(kwrapper_instance);

      } else if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(BaseKernelWrapper &))) {
        return std::any_cast<BaseKernelWrapper &>(kwrapper_instance);

      } else if(std::type_index(kwrapper_instance.type()) ==
          std::type_index(typeid(std::unique_ptr<BaseKernelWrapper> *))) {
        return *((std::any_cast<std::unique_ptr<BaseKernelWrapper> *>(kwrapper_instance))->get());
      } else {
        std::string error_msg = "Unsupported Base kernel wrapper type.";
        if( type_map.count(std::type_index(kwrapper_instance.type())) ){
          error_msg += "\nType identified as: ";
          error_msg += type_map.at(std::type_index(kwrapper_instance.type()));
          error_msg += "\n";
        }
        PANACEA_FAIL(error_msg);
      }
      return std::any_cast<BaseKernelWrapper &>(kwrapper_instance);
    }();

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart || 
        file_type == settings::FileType::TXTKernelDistribution ) {

      // Before doing any reading ensure that the underlying descriptor data type is
      // actually compatible, right now the Kernel must be of type 
      // std::vector<std::vector<double>> or std::vector<double> as in the data must 
      // be owned by the kernel wrapper and not simply a pointer to it
      if( 
          kwrapper.getTypeIndex() != 
          std::type_index(typeid(std::vector<std::vector<double>>)) && 
          kwrapper.getTypeIndex() != 
          std::type_index(typeid(std::vector<double>))
        ) {
        std::string error_msg = "Reading kernel data in requires that the kernel ";
        error_msg += "wrapper actually own the data so the underlying type must be of type";
        error_msg += " std::vector<std:vector<double>>, or std::vector<double>, ";
        error_msg += "it cannot be a pointer.";
      }

      std::string line = "";
      // First line should be header
      while(line.find("[Kernel]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Warning while reading kernel section of restart file";
          error_msg += ", header does not contain the [Kernel] word.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      settings::KernelCenterCalculation center_calc_method;
      try {
        is >> center_calc_method;
      } catch (...) {
        std::string error_msg = "Unable to read in kernel center calculation method ";
        error_msg += " from restart file file.\n";
        PANACEA_FAIL(error_msg);
      }
      if( center_calc_method != kwrapper.center() ) {
        std::string error_msg = "Inconsistency between restart file kernel center ";
        error_msg += "method and the kernel who's state is being loaded from the ";
        error_msg += "restart file.";
        PANACEA_FAIL(error_msg); 
      }

      settings::KernelCount kernel_count;
      try {
        is >> kernel_count;
      } catch (...) {
        std::string error_msg = "Unable to read in kernel count setting ";
        error_msg += " from restart file file.\n";
        PANACEA_FAIL(error_msg);
      }
      if( kernel_count != kwrapper.count() ) {
        std::string error_msg = "Inconsistency between restart file kernel count ";
        error_msg += "setting and the kernel who's state is being loaded from the ";
        error_msg += "restart file.";
        PANACEA_FAIL(error_msg); 
      }
      // Look for derived class specific data tag
      while(line.find("[Meta Data]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Warning while reading kernel section of restart ";
          error_msg += "file, header does not contain the [Meta Data] word.";
          error_msg += "\nline is: " + line + "\n";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      // Read in derived class specific data
      kwrapper.getReadFunction_()(kwrapper, is);

      std::getline(is, line);
      // First line should be header
      while(line.find("[Data]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Warning while reading kernel section of restart file,";
          error_msg +=  " header does not contain the [Data] word.\n";
          error_msg += "line is: " + line  + "\n";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      std::getline(is, line);
      if( line.find("Points along rows dimensions along columns", 0) != std::string::npos) {
        kwrapper.set(Arrangement::PointsAlongRowsDimensionsAlongCols);
      } else if (line.find("Dimensions along rows points along columns", 0) != std::string::npos) {
        kwrapper.set(Arrangement::DimensionsAlongRowsPointsAlongCols);
      } else {
        std::cout << "Warning alignment line missing from kernel section of ";
        std::cout << "restart file. The available options are:\n";
        std::cout << "Points along rows dimensions along columns\n";
        std::cout << "Dimensions along rows points along columns\n";
        std::cout << "\nAssuming:\n";
        std::cout << "Points along rows dimensions along columns\n";
        kwrapper.set(Arrangement::PointsAlongRowsDimensionsAlongCols);
      }

      std::getline(is, line);
      std::istringstream ss(line); 
      int rows;
      try { 
        ss >> rows;
      } catch (...) {
        std::string error_msg = "Unable to read in rows from [Kernel] section ";
        error_msg += "of restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }
      int cols;
      try {
        ss >> cols;
      } catch (...) {
        std::string error_msg = "Unable to read in columns from [Kernel} ";
        error_msg += "section of restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }

      try { 
        kwrapper.resize(rows,cols);
        for( int row = 0; row < rows; ++row) {
          std::getline(is, line);
          std::istringstream ss_data(line);
          for( int col = 0; col < cols; ++col) {
            double value;
            ss_data >> value;
            kwrapper.at(row,col) = value;
          }
        } 
      } catch (...) {
        std::string error_msg = "Error encountered while attempting to read in kernel ";
        error_msg += "coefficients from Kernel data file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }
    } 

    return nested_values;
  }


  BaseKernelWrapper::~BaseKernelWrapper() {};
}
