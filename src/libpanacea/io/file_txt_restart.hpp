
#ifndef PANACEA_PRIVATE_FILETXT_RESTART_H
#define PANACEA_PRIVATE_FILETXT_RESTART_H
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

  /**
   * Writes restart files in text format
   *
   * Formatting rules, each data group must begin with a group header 
   * with the form.
   *
   * [Header name]
   * Data
   *
   * The spaces between the headers are not important but the order they appear in is.
   * The data must appear on the line directly under each header. 
   *
   **/
  class FileRestartTXT : public FileIO {

    private:

    using WriteMethod = std::vector<std::any>(*)(
        const settings::FileType &,
        std::ostream &,
        std::any ); 

    using ReadMethod = std::vector<std::any>(*)(
        const settings::FileType &,
        std::istream &,
        std::any ); 

      static std::unordered_map<std::type_index,WriteMethod> write_methods_;
      static std::unordered_map<std::type_index,ReadMethod> read_methods_;

      void write_(std::vector<std::any> & objs, std::ostream & os);
      void read_(std::vector<std::any> & objs, std::istream & is);

    public:
      
      FileRestartTXT();
      settings::FileType type() const noexcept final { return settings::FileType::TXTRestart; }

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

      template<class T>
      static bool registerReadMethod(){
        if( read_methods_.count(std::type_index(typeid(T *))) ) {
          return false;
        } else {
          read_methods_[std::type_index(typeid(T *))] = T::read;
        }
        return true;
      }

      virtual void read(std::any obj, const std::string & filename) final;
      virtual void write(std::any  obj, const std::string & filename) final;
  };
}

#endif // PANACEA_PRIVATE_FILETXT_RESTART_H
