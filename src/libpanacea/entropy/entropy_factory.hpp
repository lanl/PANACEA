
#ifndef PANACEA_PRIVATE_ENTROPY_FACTORY
#define PANACEA_PRIVATE_ENTROPY_FACTORY
#pragma once

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <memory>
#include <string>
#include <unordered_map>

namespace panacea {

class BaseDescriptorWrapper;
class EntropySettings;
class EntropyTerm;

namespace settings {
enum class EntropyType;
}

class EntropyFactory {
public:
  using EntropyCreateMethod = std::unique_ptr<EntropyTerm> (*)(
      const PassKey<EntropyFactory> &key,
      const BaseDescriptorWrapper &descriptor_wrapper,
      const EntropySettings &settings);

  using EntropyCreateShellMethod = std::unique_ptr<EntropyTerm> (*)(
      const PassKey<EntropyFactory> &key, const EntropySettings &settings);

private:
  static std::unordered_map<settings::EntropyType, EntropyCreateMethod>
      create_methods_;

  static std::unordered_map<settings::EntropyType, EntropyCreateShellMethod>
      create_shell_methods_;

public:
  EntropyFactory();

  template <class T, settings::EntropyType entropy_type>
  static bool registerEntropyTerm() {
    if (create_methods_.count(entropy_type)) {
      return false;
    } else {
      create_methods_[entropy_type] = T::create;
    }
    if (create_shell_methods_.count(entropy_type)) {
      return false;
    } else {
      create_shell_methods_[entropy_type] = T::create;
    }

    return true;
  }

  std::unique_ptr<EntropyTerm>
  create(const BaseDescriptorWrapper &descriptor_wrapper,
         const EntropySettings &settings) const;

  std::unique_ptr<EntropyTerm> create(const EntropySettings &settings) const;
};
} // namespace panacea

#endif // PANACEA_PRIVATE_ENTROPY_FACTORY
