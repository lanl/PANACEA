
// Local private PANACEA includes
#include "file_txt_restart.hpp"

#include "attributes/covariance.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "distribution/distributions/distribution.hpp"
#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_specifications.hpp"
#include "matrix/matrix.hpp"
#include "primitives/primitive_group.hpp"
#include "type_map.hpp"
#include "vector/vector.hpp"

// Public PANACEA includes
#include "panacea/entropy_term.hpp"
#include "panacea/file_io_types.hpp"

// Standard includes
#include <fstream>
#include <iostream>

namespace panacea {

  namespace io {
    /***************************************************************
     * Declaring private static Member function maps
     **************************************************************/

    std::unordered_map<std::type_index,FileRestartTXT::WriteMethod>
      FileRestartTXT::write_methods_;

    std::unordered_map<std::type_index,FileRestartTXT::ReadMethod>
      FileRestartTXT::read_methods_;

    std::unordered_map<std::type_index,FileRestartTXT::PostReadInitialization>
      FileRestartTXT::post_read_initialization_;

    FileRestartTXT::FileRestartTXT() {
      // Alaways registers as a pointer and reference
      registerWriteMethod<Covariance>();
      registerWriteMethod<Matrix>();
      registerWriteMethod<Vector>();
      registerWriteMethod<BaseKernelWrapper>();
      registerWriteMethod<KernelSpecification>();
      registerWriteMethod<Normalizer>();
      registerWriteMethod<PrimitiveGroup>();
      registerWriteMethod<Distribution>();
      registerWriteMethod<EntropyTerm>();

      registerReadMethod<Covariance>();
      registerReadMethod<Matrix>();
      registerReadMethod<Vector>();
      registerReadMethod<BaseKernelWrapper>();
      registerReadMethod<KernelSpecification>();
      registerReadMethod<Normalizer>();
      registerReadMethod<PrimitiveGroup>();
      registerReadMethod<Distribution>();
      registerReadMethod<EntropyTerm>();
    }

    void FileRestartTXT::write_(
        std::vector<std::any> & objs,
        std::ostream & os) {

      for( auto & obj : objs ) {
        if( write_methods_.count(std::type_index(obj.type())) == 0 ) {
          std::string error_msg = "Unable to write object to restart file, write ";
          error_msg += "method is missing.";
          if( type_map.count(std::type_index(obj.type()))){
            error_msg += "\nType identified as: ";
            error_msg += type_map.at(std::type_index(obj.type())) + "\n";
          }
          PANACEA_FAIL(error_msg);
        }
        auto data = write_methods_[std::type_index(obj.type())](type(), os, obj);
        write_(data, os);

      }
    }

    void FileRestartTXT::read_(
        std::any parent,
        io::ReadInstantiateVector & objs,
        std::istream & is) {

      for( auto & obj : objs ) {

        if( read_methods_.count(std::type_index(obj.instance.type())) == 0 ) {
          std::string error_msg = "Unable to read object from restart txt file, read ";
          error_msg += "method is missing.";
          if( type_map.count(std::type_index(obj.instance.type()))){
            error_msg += "\nType identified as: ";
            error_msg += type_map.at(std::type_index(obj.instance.type())) + "\n";
          }
          PANACEA_FAIL(error_msg);
        }
        auto data = read_methods_[obj.instance.type()](type(), is, obj.instance);
        read_(obj.instance, data, is);


        // Below we have the options to call two different functions
        // The first option allows further initialization of a object parent
        // The second post read option allows further initialization of the class
        // that was just read in, it will not have access to the state of the parent

        // First option
        if(auto method = obj.method){
          (*method)(parent);
        }

        // Second option
        if( post_read_initialization_.count(obj.instance.type())){
          post_read_initialization_[obj.instance.type()](type(),obj.instance);
        }
      }
    }


    void FileRestartTXT::write(std::any obj, std::ostream & os) {

      // Check if object type is registered
      if( write_methods_.count(std::type_index(obj.type())) ) {
        auto data = write_methods_[obj.type()](type(),os,obj);
        write_(data, os);
      } else {
        std::string error_msg = "Unable to write object it does not contain ";
        error_msg += "a registered write method.";
        if( type_map.count(std::type_index(obj.type()))){
          error_msg += "\nType identified as: ";
          error_msg += type_map.at(std::type_index(obj.type())) + "\n";
        }
        PANACEA_FAIL(error_msg);
      }
    }

    void FileRestartTXT::write(std::any obj, const std::string & filename) {

      std::fstream fs;
      fs.open(filename, std::fstream::out);

      write(obj, fs);
      fs.close();
    }


    void FileRestartTXT::read(std::any obj, std::istream & is) {

      // Check if object type is registered
      if( read_methods_.count(std::type_index(obj.type())) ) {
        auto data = read_methods_[std::type_index(obj.type())](type(),is,obj);
        read_(obj, data, is);
      } else {
        std::string error_msg = "Unable to read object it does not contain ";
        error_msg += "a registered read method.";
        if( type_map.count(std::type_index(obj.type()))){
          error_msg += "\nType identified as: ";
          error_msg += type_map.at(std::type_index(obj.type())) + "\n";
        }
        PANACEA_FAIL(error_msg);
      }

      if( post_read_initialization_.count(std::type_index(obj.type()))){
        post_read_initialization_[obj.type()](type(),obj);
      }
    }

    void FileRestartTXT::read(std::any obj, const std::string & filename) {
      // Not implemented
      std::fstream fs;
      fs.open(filename, std::fstream::in);

      read(obj, fs);
      fs.close();
    }

  }
}
