
// Public PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Local private PANACEA includes
#include "file_txt_restart.hpp"

#include "attributes/covariance.hpp"
#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"

// Standard includes
#include <fstream>
#include <iostream>

namespace panacea {

  /***************************************************************
   * Declaring private static Member function maps
   **************************************************************/

  std::unordered_map<std::type_index,FileRestartTXT::WriteMethod> 
    FileRestartTXT::write_methods_; 

  std::unordered_map<std::type_index,FileRestartTXT::ReadMethod>
    FileRestartTXT::read_methods_;

  FileRestartTXT::FileRestartTXT() {
    // Alaways registers as a pointer
    registerWriteMethod<Covariance>();
    registerWriteMethod<Matrix>();
    registerWriteMethod<Vector>();
    registerWriteMethod<BaseKernelWrapper>();

    registerReadMethod<Covariance>();
    registerReadMethod<Matrix>();
    registerReadMethod<Vector>();
    registerReadMethod<BaseKernelWrapper>();
  }

  void FileRestartTXT::write_(
      std::vector<std::any> & objs,
      std::ostream & os) {

    for( auto & obj : objs ) {
      std::cout << "writing data from objs" << std::endl;
      if( write_methods_.count(obj.type()) == 0 ) {
        std::string error_msg = "Unable to write object to restart file, write ";
        error_msg += "method is missing.";
        PANACEA_FAIL(error_msg);
      }
      auto data = write_methods_[obj.type()](type(), os, obj);
      write_(data, os);

    }
  }

  void FileRestartTXT::read_(
      std::vector<std::any> & objs,
      std::istream & is) {

    for( auto & obj : objs ) {
      if( read_methods_.count(obj.type()) == 0 ) {
        std::string error_msg = "Unable to read object from restart txt file, read ";
        error_msg += "method is missing.";
        PANACEA_FAIL(error_msg);
      }
      auto data = read_methods_[obj.type()](type(), is, obj);
      read_(data, is);
    }
  }


  void FileRestartTXT::write(std::any obj, const std::string & filename) {

    std::fstream fs;
    fs.open(filename, std::fstream::out);

    // Check if object type is registered
    if( write_methods_.count(obj.type()) ) {
      auto data = write_methods_[obj.type()](type(),fs,obj);
      write_(data, fs);
    } else {
      std::string error_msg = "Unable to write object it does not contain ";
      error_msg += "a registered write method.";
      PANACEA_FAIL(error_msg);
    }
    fs.close();
  }

  void FileRestartTXT::read(std::any obj, const std::string & filename) {
    // Not implemented
    //
    std::fstream fs;
    fs.open(filename, std::fstream::in);

    // Check if object type is registered
    if( read_methods_.count(obj.type()) ) {
      auto data = read_methods_[obj.type()](type(),fs,obj);
      read_(data, fs);
    } else {
      std::string error_msg = "Unable to read object it does not contain ";
      error_msg += "a registered read method.";
      PANACEA_FAIL(error_msg);
    }
    fs.close();

  }
}
