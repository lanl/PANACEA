
// Public PANACEA includes
#include "panacea/panacea.hpp"

#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "descriptors/descriptor_wrapper.hpp"
#include "entropy/entropy_factory.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "error.hpp"
#include "io/file_io_factory.hpp"

// Standard includes
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace panacea {

/**
 * Only certain data types can be wrapped
 *
 * Note we do not want to make a copy of the underlying data type which is why
 *if 2d data is passed in we actually create a pointer. This is because this is
 *a wrapper method.
 **/
std::unique_ptr<BaseDescriptorWrapper>
PANACEA::wrap(std::any data, const int rows, const int cols) const {

  if (data.type() == typeid(double ***)) {
    return std::make_unique<DescriptorWrapper<double ***>>(
        std::any_cast<double ***>(data), rows, cols);
  }
  if (data.type() == typeid(std::vector<std::vector<double>> *)) {
    return std::make_unique<
        DescriptorWrapper<std::vector<std::vector<double>> *>>(
        std::any_cast<std::vector<std::vector<double>> *>(data), rows, cols);
  }
  if (data.type() == typeid(std::vector<std::vector<double>>)) {
    return std::make_unique<
        DescriptorWrapper<std::vector<std::vector<double>>>>(
        std::any_cast<std::vector<std::vector<double>>>(data), rows, cols);
  }
  if (data.type() == typeid(std::vector<double>)) {
    return std::make_unique<DescriptorWrapper<std::vector<double>>>(
        std::any_cast<std::vector<double>>(data), rows, cols);
  }
  std::string error_msg = "Tried to wrap an unsuppored data type";
  PANACEA_FAIL(error_msg);

  return nullptr;
}

std::unique_ptr<EntropyTerm>
PANACEA::create(const BaseDescriptorWrapper &dwrapper,
                const PANACEASettings &settings) const {

  EntropySettings entropy_settings(settings);
  EntropyFactory entropy_factory;
  return entropy_factory.create(dwrapper, entropy_settings);
}

std::unique_ptr<EntropyTerm>
PANACEA::create(const PANACEASettings &settings) const {

  EntropySettings entropy_settings(settings);
  EntropyFactory entropy_factory;
  return entropy_factory.create(entropy_settings);
}

std::unique_ptr<io::FileIO>
PANACEA::create(const settings::FileType type) const {
  io::FileIOFactory file_factory;
  return file_factory.create(type);
}
// 1. Determine file type by looking at extension
// Each object should have it's own read and write methods that are registered
// with the file type
// 2. Search for entropy settings in the file
// 3. Call creation methods with file handle

} // namespace panacea
