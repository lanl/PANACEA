#ifndef PANACEA_FILEIO_H
#define PANACEA_FILEIO_H
#pragma once

// Local private PANACEA settings
#include "private_settings.hpp"

// Standard includes
#include <any>
#include <string>
#include <vector>

namespace panacea {

  namespace io {
    class FileIO {
      public:
        virtual settings::FileType type() const noexcept = 0;
        virtual void read(std::any obj, const std::string & filename) = 0;
        virtual void write(std::any  obj, const std::string & filename) = 0;

        virtual ~FileIO() = 0;
    };
  }

}
#endif // PANACEA_FILEIO_H
