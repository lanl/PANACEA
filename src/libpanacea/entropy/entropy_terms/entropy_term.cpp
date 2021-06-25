
// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <fstream>
#include <vector>
#include <typeindex>

namespace panacea {
  EntropyTerm::~EntropyTerm() {};

  std::vector<std::any> EntropyTerm::write(
      const settings::FileType file_type,
      std::ostream & os,
      std::any entropy_term_instance) {

    const EntropyTerm & entropy_term = [&]() -> const EntropyTerm & {
      if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(EntropyTerm &))){
        // We are adding const because we are writing data there is no reason 
        // that the entropy term needs to be non const
        return const_cast<const EntropyTerm &>(
            std::any_cast<EntropyTerm &>(entropy_term_instance));

      } else if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(EntropyTerm *))){
        return const_cast<const EntropyTerm &>(
            *std::any_cast<EntropyTerm *>(entropy_term_instance));

      } else if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(const EntropyTerm &))){
        return std::any_cast<const EntropyTerm &>(entropy_term_instance);

      } else if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(const EntropyTerm *))){
        return *std::any_cast<const EntropyTerm *>(entropy_term_instance);

      } else {
        PANACEA_FAIL("Unsupported EntropyTerm encountered.");
      }
      return std::any_cast<const EntropyTerm &>(entropy_term_instance);
    }();

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart || 
        file_type == settings::FileType::TXTKernelDistribution ) {
      os << "[Entropy]\n";
      os << entropy_term.type() << "\n";
      nested_values = entropy_term.getWriteFunction(entropy_term.key)(file_type, os, entropy_term);
    } 
    return nested_values;
  }

  io::ReadInstantiateVector EntropyTerm::read(
      const settings::FileType file_type,
      std::istream & is,
      std::any entropy_term_instance) {

    EntropyTerm & entropy_term = [&]() -> EntropyTerm & {
      if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(EntropyTerm &))){
        return std::any_cast<EntropyTerm &>(entropy_term_instance);
      } else if(std::type_index(entropy_term_instance.type()) ==
          std::type_index(typeid(EntropyTerm *))){
        return *std::any_cast<EntropyTerm *>(entropy_term_instance);
      } else {
        PANACEA_FAIL("Unsupported EntropyTerm encountered.");
      }
      return std::any_cast<EntropyTerm &>(entropy_term_instance);
    }();

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart || 
        file_type == settings::FileType::TXTKernelDistribution ) {

      std::string line = "";
      while(line.find("[Entropy]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading entropy_term section of ";
          error_msg += "restart file, file does not contain the [Entropy] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      settings::EntropyType ent_type;
      is >> ent_type;
      assert(ent_type == entropy_term.type());
      nested_values = entropy_term.getReadFunction(entropy_term.key)(file_type, is, entropy_term);
    } 
    return nested_values;
  }
}
