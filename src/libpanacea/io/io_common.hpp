
#ifndef PANACEA_PRIVATE_IO_COMMON_H
#define PANACEA_PRIVATE_IO_COMMON_H
#pragma once

// Standard includes
#include <type_traits>

namespace panacea {

  /**
   * This file provides functionality that is shared across io readers and writers
   **/
  namespace io {


    /**
     * The following two structures provide a means for checking if a class contains
     * a static method, will return a value at compile time indicating if it is true
     * or false. 
     *
     * In particular will look to see if a class T has a postReadInitialization method.
     **/
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

  }
}

#endif // PANACEA_PRIVATE_IO_COMMON_H
