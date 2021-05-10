
#ifndef PANACEA_PRIVATE_FILETXT_RESTART_H
#define PANACEA_PRIVATE_FILETXT_RESTART_H
#pragma once

// Public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/file_io.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <memory>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>

namespace panacea {

  namespace io {

    template<typename, typename T>
      struct has_post_read_initialization_method {
        static_assert(
            std::integral_constant<T, false>::value,
            "Second template parameter needs to be of function type.");
      };

    // specialization that does the checking
    template<typename C, typename Ret, typename... Args>
      struct has_post_read_initialization_method<C, Ret(Args...)> {
        private:
          template<typename T>
            static constexpr auto check(T*)
            -> typename
            std::is_same<
            decltype( std::declval<T>().postReadInitialization( std::declval<Args>()... ) ),
            Ret>::type;

          template<typename>
            static constexpr std::false_type check(...);

          typedef decltype(check<C>(0)) type;

        public:
          static constexpr bool value = type::value;
      };

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
            const settings::FileType ,
            std::ostream &,
            std::any ); 

        using ReadMethod = io::ReadInstantiateVector(*)(
            const settings::FileType ,
            std::istream &,
            std::any ); 

        using PostReadInitialization = void (*) (
            const settings::FileType ,
            std::any);

        static std::unordered_map<std::type_index,WriteMethod> write_methods_;
        static std::unordered_map<std::type_index,ReadMethod> read_methods_;
        static std::unordered_map<std::type_index,PostReadInitialization> 
          post_read_initialization_;

        void write_(std::vector<std::any> & objs, std::ostream & os);

        void read_(
            std::any parent,
            ReadInstantiateVector & objs,
            std::istream & is);

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


        /**
         * Will register a static read method from the provided class
         *
         * Will also register a static postReadInitialization method if
         * one is detected with the correct function signature.
         **/
        template<class T>
          static bool registerReadMethod(){
            if( read_methods_.count(std::type_index(typeid(T *))) ) {
              return false;
            } else {
              read_methods_[std::type_index(typeid(T *))] = T::read;
            }
            if( read_methods_.count(std::type_index(typeid(std::unique_ptr<T> *))) ) {
              return false;
            } else {
              read_methods_[std::type_index(typeid(std::unique_ptr<T> *))] = T::read;
            }

            if constexpr( has_post_read_initialization_method<
                T,void(const settings::FileType , std::any)>::value ){

              if( post_read_initialization_.count(std::type_index(typeid(T *))) ) {
                return false;
              } else {
                post_read_initialization_[std::type_index(typeid(T *))] = T::postReadInitialization;
              }
            }

            return true;
          }

        virtual void read(std::any obj, std::istream & is) final;
        virtual void write(std::any  obj, std::ostream & os) final;
        virtual void read(std::any obj, const std::string & filename) final;
        virtual void write(std::any  obj, const std::string & filename) final;
    };

  }
}

#endif // PANACEA_PRIVATE_FILETXT_RESTART_H
