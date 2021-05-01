
// Public PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Local private PANACEA includes
#include "file_txt_restart.hpp"

#include "attributes/covariance.hpp"
#include "error.hpp"

// Standard includes
#include <fstream>
#include <iostream>

namespace panacea {

  /***************************************************************
   * Declaring private static Member function maps
   **************************************************************/

  std::unordered_map<std::type_index,FileRestartTXT::WriteMethod> 
    FileRestartTXT::write_methods_; 

//  std::unordered_map<std::type_index,ReadMethod>
//    FileRestartTXT::read_methods_;

  FileRestartTXT::FileRestartTXT() {
    // Alaways registers as a pointer
    registerWriteMethod<Covariance>();
    registerWriteMethod<Matrix>();
    registerWriteMethod<Vector>();
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
  }
}
