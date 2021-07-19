
// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "entropy_factory.hpp"

#include "distribution/distribution_factory.hpp"
#include "entropy_settings/entropy_settings.hpp"
#include "entropy_terms/cross_entropy.hpp"
#include "entropy_terms/entropy_decorators/numerical_grad.hpp"
#include "entropy_terms/entropy_decorators/weight.hpp"
#include "entropy_terms/self_entropy.hpp"
#include "error.hpp"
#include "private_settings.hpp"

// Standard includes
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

namespace panacea {

namespace {
std::unique_ptr<EntropyTerm> decorate(std::unique_ptr<EntropyTerm> ent_term,
                                      const EntropySettings &settings) {
  // Add decorators
  if (settings.weight) {
    ent_term = std::make_unique<Weight>(std::move(ent_term),
                                        settings.weight.value_or(1.0));
  }
  if (settings.numerical_grad_switch) {
    ent_term = std::make_unique<NumericalGrad>(std::move(ent_term));
  }
  return ent_term;
}
} // unnamed namespace

/**********************************************
 * Static members
 **********************************************/
std::unordered_map<settings::EntropyType, EntropyFactory::EntropyCreateMethod>
    EntropyFactory::create_methods_;

std::unordered_map<settings::EntropyType,
                   EntropyFactory::EntropyCreateShellMethod>
    EntropyFactory::create_shell_methods_;

/**********************************************
 * Public Methods
 **********************************************/
EntropyFactory::EntropyFactory() {

  EntropyFactory::registerEntropyTerm<SelfEntropy,
                                      settings::EntropyType::Self>();

  EntropyFactory::registerEntropyTerm<CrossEntropy,
                                      settings::EntropyType::Cross>();
}

std::unique_ptr<EntropyTerm>
EntropyFactory::create(const BaseDescriptorWrapper &descriptor_wrapper,
                       const EntropySettings &settings) const {

  if (create_methods_.count(settings.type) == 0) {
    std::string error_msg = "Entropy type is not registered with the factory.";
    PANACEA_FAIL(error_msg);
  }

  std::unique_ptr<EntropyTerm> ent_term = create_methods_.at(settings.type)(
      PassKey<EntropyFactory>(), descriptor_wrapper, settings);

  ent_term = decorate(std::move(ent_term), settings);

  return ent_term;
}

std::unique_ptr<EntropyTerm>
EntropyFactory::create(const EntropySettings &settings) const {

  if (create_methods_.count(settings.type) == 0) {
    std::string error_msg = "Entropy type is not registered with the factory.";
    PANACEA_FAIL(error_msg);
  }
  std::unique_ptr<EntropyTerm> ent_term = create_shell_methods_.at(
      settings.type)(PassKey<EntropyFactory>(), settings);

  ent_term = decorate(std::move(ent_term), settings);

  return ent_term;
}

} // namespace panacea
