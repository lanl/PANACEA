
#ifndef PANACEA_PRIVATE_FILETXT_DESCRIPTOR_H
#define PANACEA_PRIVATE_FILETXT_DESCRIPTOR_H
#pragma once

// Local private PANACEA includes
#include "file_io.hpp"

// Standard includes
#include <any>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace panacea {

  class FileDescriptorTXT : public FileIO {

    private:

    using WriteMethod = std::vector<std::any>(*)(
        settings::FileType,std::ostream &, std::any ); 
 //   using ReadMethod = std::map<std::type_index, std::any>(*)(
  //      settings::FileType,std::istream, std::any &); 
      static std::unordered_map<std::type_index,WriteMethod> write_methods_;
//      static std::unordered_map<std::type_index,ReadMethod> read_methods_;

      void write_(std::vector<std::any> & objs, std::ostream & os);
//      void read_(std::map<std::type_index, std::any> objs, std::istream is);

    public:
      
      settings::FileType type() const noexcept final { return settings::FileType::TXTDescriptors; }

      /**
       * Will always register methods to the pointer type index
       **/
      template<class T>
      static bool registerWriteMethod(){
        if( write_methods_.count(std::type_index(typeid(T *))) ) {
          return false;
        } else {
          write_methods_[std::type_index(typeid(T *))] = T::write;
        }
        return true;
      }

      FileDescriptorTXT();
 /*     template<class T>
      static bool registerWriteMethod(){
        if( read_methods_.count(std::type_index(typeid(T))) ) {
          return false;
        } else {
          read_methods_[std::type_index(typeid(T))] = T::read;
        }
        return true;
      }*/

//      virtual void read(std::any & obj, const std::string & filename) final;
      virtual void write(std::any  obj, const std::string & filename) final;
  };
}


#endif // PANACEA_PRIVATE_FILETXT_DESCRIPTOR_H
