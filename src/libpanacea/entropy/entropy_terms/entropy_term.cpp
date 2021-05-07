
// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <fstream>
#include <vector>

namespace panacea {
  EntropyTerm::~EntropyTerm() {};

  std::vector<std::any> EntropyTerm::write(
      const settings::FileType file_type,
      std::ostream & os,
      std::any entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      auto entropy_term = std::any_cast<EntropyTerm *>(entropy_term_instance);
      os << "[Entropy]\n";
      os << entropy_term->type() << "\n";

      // Write out derived class specific data
      nested_values = entropy_term->getWriteFunction(entropy_term->key)(file_type, os, entropy_term);

    } else {
      std::string error_msg = "Descriptors cannot be written to the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return nested_values;

  }

  io::ReadInstantiateVector EntropyTerm::read(
      const settings::FileType file_type,
      std::istream & is,
      std::any entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      auto entropy_term = std::any_cast<EntropyTerm *>(entropy_term_instance);
      
      std::string line = "";
      while(line.find("[Entropy]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading entropy_term section of restart file";
          error_msg += ", file does not contain the [Entropy] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      settings::EntropyType ent_type;
      is >> ent_type;
      assert(ent_type == entropy_term->type());
      nested_values = entropy_term->getReadFunction(entropy_term->key)(file_type, is, entropy_term);
    }
    return nested_values;
  }




}
