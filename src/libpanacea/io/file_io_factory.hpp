
#ifndef PANACEA_PRIVATE_FILEIO_FACTORY_H
#define PANACEA_PRIVATE_FILEIO_FACTORY_H
#pragma once

// Local private PANACEA includes
#include "private_settings.hpp"

// Standard includes
#include <memory>

namespace panacea {

namespace io {
class FileIO;

class FileIOFactory {
public:
  std::unique_ptr<FileIO> create(const settings::FileType &type) const;
};

} // namespace io
} // namespace panacea

#endif // PANACEA_PRIVATE_FILEIO_FACTORY_H
