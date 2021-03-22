
#ifndef PANACEA_PRIVATE_ERROR_H
#define PANACEA_PRIVATE_ERROR_H
#pragma once

// Standard includes
#include <sstream>

#define PANACEA_FAIL(message)                                                          \
  panacea::error::fail(message, __FILE__, __LINE__);

namespace panacea {
  namespace error {
    template<typename T>
    inline void fail(T message, const char *const filename, int const linenumber) {
      std::stringstream stream;
      stream << "PANACEA ERROR" << std::endl;
      stream << "  Message:     " << message << std::endl;
      stream << "  File:        " << filename << std::endl;
      stream << "  Line number: " << linenumber << std::endl;
      throw std::runtime_error(stream.str());
    }
  } // error
} // panacea

#endif // PANACEA_PRIVATE_ERROR_H
