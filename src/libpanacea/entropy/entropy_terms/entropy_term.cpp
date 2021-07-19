
// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <fstream>
#include <iostream>
#include <typeindex>
#include <vector>

namespace panacea {
EntropyTerm::~EntropyTerm(){};

std::vector<std::any> EntropyTerm::write(const settings::FileType file_type,
                                         std::ostream &os,
                                         std::any entropy_term_instance) {

  const EntropyTerm &entropy_term = [&]() -> const EntropyTerm & {
    if (std::type_index(entropy_term_instance.type()) ==
        std::type_index(typeid(EntropyTerm &))) {
      // We are adding const because we are writing data there is no reason
      // that the entropy term needs to be non const
      return const_cast<const EntropyTerm &>(
          std::any_cast<EntropyTerm &>(entropy_term_instance));

    } else if (std::type_index(entropy_term_instance.type()) ==
               std::type_index(typeid(EntropyTerm *))) {
      return const_cast<const EntropyTerm &>(
          *std::any_cast<EntropyTerm *>(entropy_term_instance));

    } else if (std::type_index(entropy_term_instance.type()) ==
               std::type_index(typeid(const EntropyTerm &))) {
      return std::any_cast<const EntropyTerm &>(entropy_term_instance);

    } else if (std::type_index(entropy_term_instance.type()) ==
               std::type_index(typeid(const EntropyTerm *))) {
      return *std::any_cast<const EntropyTerm *>(entropy_term_instance);

    } else {
      PANACEA_FAIL("Unsupported EntropyTerm encountered.");
    }
    return std::any_cast<const EntropyTerm &>(entropy_term_instance);
  }();

  std::vector<std::any> nested_values;
  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    if (file_type == settings::FileType::TXTRestart) {
      if (entropy_term.state() != EntropyTerm::State::Initialized) {
        std::string error_msg =
            "Only entropy terms that have been initialized can";
        error_msg +=
            " be written to a restart file. If you want to still output ";
        error_msg +=
            "partial content of the term consider using a different file ";
        error_msg += "type other than restart.";
        PANACEA_FAIL(error_msg);
      }
    }
    os << "[Entropy]\n";
    os << entropy_term.type() << "\n";
    auto write_elements = entropy_term.getWriteElements(entropy_term.key);
    for (auto &write_element : write_elements) {
      std::vector<std::any> new_values =
          write_element.write(file_type, os, write_element.term);
      nested_values.insert(nested_values.end(), new_values.begin(),
                           new_values.end());
    }
  }
  return nested_values;
}

io::ReadInstantiateVector EntropyTerm::read(const settings::FileType file_type,
                                            std::istream &is,
                                            std::any entropy_term_instance) {

  EntropyTerm &entropy_term = [&]() -> EntropyTerm & {
    if (std::type_index(entropy_term_instance.type()) ==
        std::type_index(typeid(EntropyTerm &))) {
      return std::any_cast<EntropyTerm &>(entropy_term_instance);
    } else if (std::type_index(entropy_term_instance.type()) ==
               std::type_index(typeid(EntropyTerm *))) {
      return *std::any_cast<EntropyTerm *>(entropy_term_instance);
    } else {
      PANACEA_FAIL("Unsupported EntropyTerm encountered.");
    }
    return std::any_cast<EntropyTerm &>(entropy_term_instance);
  }();

  io::ReadInstantiateVector nested_values;
  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    std::string line = "";
    while (line.find("[Entropy]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg = "While reading entropy_term section of ";
        error_msg += "restart file, file does not contain the [Entropy] tag.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    settings::EntropyType ent_type;
    is >> ent_type;
    assert(ent_type == entropy_term.type());
    auto read_elements = entropy_term.getReadElements(entropy_term.key);
    for (auto &read_element : read_elements) {
      io::ReadInstantiateVector new_values =
          read_element.read(file_type, is, read_element.term);
      nested_values.insert(nested_values.end(), new_values.begin(),
                           new_values.end());
    }
  }
  return nested_values;
}
} // namespace panacea
