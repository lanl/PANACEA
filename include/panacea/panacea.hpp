
#ifndef PANACEA_H
#define PANACEA_H
#pragma once

// Public PANACEA includes
#include "file_io.hpp"
#include "settings.hpp"

// Standard includes
#include <any>
#include <memory>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropyTerm;

  class PANACEA {

    public:

      std::unique_ptr<BaseDescriptorWrapper> wrap(
          std::any ,
          const int rows,
          const int cols) const;

      std::unique_ptr<EntropyTerm> create(
          const BaseDescriptorWrapper &,
          const PANACEASettings & settings) const;

      std::unique_ptr<EntropyTerm> create(
          const PANACEASettings & settings) const;

      std::unique_ptr<EntropyTerm> create(const std::string & file_name) const;

      std::unique_ptr<io::FileIO> create(const settings::FileType) const;

  };

};

#endif // PANACEA_H
