
#ifndef PANACEA_PRIVATE_ENTROPY_FACTORY
#define PANACEA_PRIVATE_ENTROPY_FACTORY
#pragma once

// Local private PANACEA includes
#include "passkey.hpp"

// Standard includes
#include <memory>
#include <string>
#include <unordered_map>

namespace panacea {

  class BaseDescriptorWrapper;
  class EntropySettings;
  class EntropyTerm;
  class MemoryManager;

  namespace settings {
    enum class EntropyType;
  }

  class EntropyFactory {
    public:
      using EntropyCreateMethod = std::unique_ptr<EntropyTerm>(*)(
          const PassKey<EntropyFactory> & key,
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          EntropySettings * settings,
          std::string name);

    private:

      static std::unordered_map<settings::EntropyType,EntropyCreateMethod>
        create_methods_;

    public:

      template<class T, settings::EntropyType entropy_type>
        static bool registerEntropyTerm() {
          if( create_methods_.count(entropy_type) ) {
            return false;
          } else {
            create_methods_[entropy_type] = T::create;
          }
          return true;
        }

      std::unique_ptr<EntropyTerm> create(
          BaseDescriptorWrapper * descriptor_wrapper,
          MemoryManager & mem_manager,
          EntropySettings * settings,
          std::string name = "") const;

  };
}

#endif // PANACEA_PRIVATE_ENTROPY_FACTORY
