// Local private includes
#include "distribution.hpp"

#include "error.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <iostream>
#include <unordered_map>
#include <vector>
namespace panacea {
Distribution::~Distribution(){};

std::vector<std::any> Distribution::write(const settings::FileType file_type,
                                          std::ostream &os,
                                          std::any dist_instance) {

  std::vector<std::any> nested_values;
  if (file_type == settings::FileType::TXTRestart) {
    auto dist = std::any_cast<Distribution *>(dist_instance);
    os << "[Distribution]\n";
    os << dist->type() << "\n";

    // Write out derived class specific data
    nested_values = dist->getWriteFunction_()(file_type, os, dist);

  } else {
    auto dist = std::any_cast<Distribution *>(dist_instance);
    nested_values = dist->getWriteFunction_()(file_type, os, dist);
  }
  return nested_values;
}

io::ReadInstantiateVector Distribution::read(const settings::FileType file_type,
                                             std::istream &is,
                                             std::any dist_instance) {

  io::ReadInstantiateVector nested_values;
  if (file_type == settings::FileType::TXTRestart) {
    auto dist = std::any_cast<Distribution *>(dist_instance);

    std::string line = "";
    while (line.find("[Distribution]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg =
            "While reading kernel distribution section of restart file";
        error_msg += ", file does not contain the [Distribution] tag.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    std::getline(is, line);

    // Write out derived class specific data
    nested_values = dist->getReadFunction_()(file_type, is, dist);

  } else {
    auto dist = std::any_cast<Distribution *>(dist_instance);
    nested_values = dist->getReadFunction_()(file_type, is, dist);
  }
  return nested_values;
}

} // namespace panacea
