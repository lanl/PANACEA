
#ifndef PANACEA_FILEIO_TYPES_H
#define PANACEA_FILEIO_TYPES_H
#pragma once

// Standard includes
#include <any>
#include <optional>
#include <vector>

namespace panacea {

namespace io {

using ExtraInstantiationMethod = void (*)(std::any);

struct IOElement {
  IOElement(std::any val1, std::optional<ExtraInstantiationMethod> val2)
      : instance(val1), method(val2){};
  std::any instance;
  std::optional<ExtraInstantiationMethod> method;
};

using ReadInstantiateVector = std::vector<IOElement>;
} // namespace io
} // namespace panacea

#endif // PANACEA_FILEIO_TYPES_H
