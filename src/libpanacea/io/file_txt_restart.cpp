
// Local private PANACEA includes
#include "file_txt_restart.hpp"

#include "attributes/covariance.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "distribution/distributions/distribution.hpp"
#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_specifications.hpp"
#include "primitives/primitive_group.hpp"
#include "type_map.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

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
      // Alaways registers as a pointer
      registerWriteMethod<Covariance>();
      registerWriteMethod<Matrix>();
      registerWriteMethod<Vector>();
      registerWriteMethod<BaseKernelWrapper>();
      registerWriteMethod<KernelSpecification>();
      registerWriteMethod<Normalizer>();
      registerWriteMethod<PrimitiveGroup>();
      registerWriteMethod<Distribution>();

      registerReadMethod<Covariance>();
      registerReadMethod<Matrix>();
      registerReadMethod<Vector>();
      registerReadMethod<BaseKernelWrapper>();
      registerReadMethod<KernelSpecification>();
      registerReadMethod<Normalizer>();
      registerReadMethod<PrimitiveGroup>();
      registerReadMethod<Distribution>();
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
        std::any parent,
        io::ReadInstantiateVector & objs,
        std::istream & is) {

      for( auto & obj : objs ) {

        std::cout << "reading obj" << std::endl;
        if(type_map.count(std::type_index(obj.instance.type()))){
          std::cout << "Reading in " << type_map.at(std::type_index(obj.instance.type())) << std::endl;
        }
        if( read_methods_.count(obj.instance.type()) == 0 ) {
          std::string error_msg = "Unable to read object from restart txt file, read ";
          error_msg += "method is missing.";
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
        read_(obj, data, fs);
      } else {
        std::string error_msg = "Unable to read object it does not contain ";
        error_msg += "a registered read method.";
        PANACEA_FAIL(error_msg);
      }
      fs.close();

      if( post_read_initialization_.count(obj.type())){
        post_read_initialization_[obj.type()](type(),obj);
      }
    }

  }
}