
// Local private PANACEA includes
#include "kernel_specifications.hpp"

#include "error.hpp"
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <iostream>
#include <string>
#include <vector>

namespace panacea {

  std::vector<std::any> KernelSpecification::write(
      const settings::FileType & file_type,
      std::ostream & os,
      std::any kern_spec_instance) {

    if( file_type == settings::FileType::TXTRestart ) {
      KernelSpecification * kern_spec;
      try {
        kern_spec = std::any_cast<KernelSpecification *>(kern_spec_instance);
      } catch (...) {
        std::string error_msg = "Unable to cast to KernelSpecification * while";
        error_msg += " trying to write kernel specs to restart file.";
        PANACEA_FAIL(error_msg);
      }
      os << "[Kernel Specifications]\n"; 
      os << kern_spec->kern_correlation_ << "\n"; 
      os << kern_spec->kern_count_ << "\n"; 
      os << kern_spec->kern_prim_ << "\n";
      os << kern_spec->kern_normalization_ << "\n";
      os << kern_spec->kern_memory_ << "\n";
      os << kern_spec->kern_center_ << "\n";
      os << kern_spec->kern_algorithm_ << "\n";
      os << "\n";
    } else {
      std::string error_msg = "Kernel specification cannot be written to ";
      error_msg += "the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return std::vector<std::any> {};
  }

  io::ReadInstantiateVector KernelSpecification::read(
      const settings::FileType & file_type,
      std::istream & is,
      std::any kern_spec_instance) {

    if( file_type == settings::FileType::TXTRestart ) {
      KernelSpecification * kern_spec;
      try {
        kern_spec = std::any_cast<KernelSpecification *>(kern_spec_instance);
      } catch (...) {
        std::string error_msg = "Unable to cast to KernelSpecification * while";
        error_msg += " trying to read kernel specs from restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::string line = "";
      while(line.find("[Kernel Specifications]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Kernel Specifications] header while trying ";
          error_msg += "to read in kernel specifications from restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::cout << "Line is " << line << std::endl;
        std::getline(is, line);
      }
      is >> kern_spec->kern_correlation_; 
      is >> kern_spec->kern_count_; 
      is >> kern_spec->kern_prim_;
      is >> kern_spec->kern_normalization_;
      is >> kern_spec->kern_memory_;
      is >> kern_spec->kern_center_;
      is >> kern_spec->kern_algorithm_;
    }
    io::ReadInstantiateVector nested_values;
    return nested_values;
  }


  
}
