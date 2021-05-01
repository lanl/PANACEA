
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "file_txt_descriptor.hpp"

#include "error.hpp"

// Standard includes
#include <fstream>
#include <iostream>

namespace panacea {

  /***************************************************************
   * Declaring private static Member function maps
   **************************************************************/

  std::unordered_map<std::type_index,FileDescriptorTXT::WriteMethod> 
    FileDescriptorTXT::write_methods_; 

  std::unordered_map<std::type_index,FileDescriptorTXT::ReadMethod>
    FileDescriptorTXT::read_methods_;

  FileDescriptorTXT::FileDescriptorTXT() {
    // Alaways registers as a pointer
    registerWriteMethod<BaseDescriptorWrapper>();
    registerReadMethod<BaseDescriptorWrapper>();
  }

  void FileDescriptorTXT::write_(
      std::vector<std::any> & objs,
      std::ostream & os) {

    for( auto & obj : objs ) {
      if( write_methods_.count(obj.type()) == 0 ) {
        std::string error_msg = "Unable to write object to descriptor txt file, write ";
        error_msg += "method is missing.";
        PANACEA_FAIL(error_msg);
      }
      auto data = write_methods_[obj.type()](type(), os, obj);
      write_(data, os);

    }
  }

  void FileDescriptorTXT::read_(
      std::vector<std::any> & objs,
      std::istream & is) {

    for( auto & obj : objs ) {
      if( read_methods_.count(obj.type()) == 0 ) {
        std::string error_msg = "Unable to read object from descriptor txt file, read ";
        error_msg += "method is missing.";
        PANACEA_FAIL(error_msg);
      }
      auto data = read_methods_[obj.type()](type(), is, obj);
      read_(data, is);

    }
  }

  void FileDescriptorTXT::write(std::any obj, const std::string & filename) {

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

  void FileDescriptorTXT::read(std::any obj, const std::string & filename) {

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
