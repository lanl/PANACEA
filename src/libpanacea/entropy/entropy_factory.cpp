
// Public PANACEA includes
#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "entropy_factory.hpp"

#include "distribution/distribution_factory.hpp"
#include "entropy_settings/entropy_settings.hpp"
#include "entropy_terms/cross_entropy.hpp"
#include "entropy_terms/self_entropy.hpp"
#include "error.hpp"
#include "private_settings.hpp"

// Standard includes
#include <string>
#include <unordered_map>

namespace panacea {

  std::unordered_map<settings::EntropyType,
    EntropyFactory::EntropyCreateMethod>
        EntropyFactory::create_methods_;

  EntropyFactory::EntropyFactory() {

    EntropyFactory::registerEntropyTerm<
      SelfEntropy,
      settings::EntropyType::Self>();

    EntropyFactory::registerEntropyTerm<
      CrossEntropy,
      settings::EntropyType::Cross>();
  }

  std::unique_ptr<EntropyTerm> EntropyFactory::create(
      const BaseDescriptorWrapper * descriptor_wrapper,
      EntropySettings * settings) const {

    assert(settings != nullptr);

    if(create_methods_.count(settings->type) == 0){
      std::string error_msg = "Entropy type is not registered with the factory.";
      PANACEA_FAIL(error_msg);
    }

    return create_methods_.at(settings->type)(
        PassKey<EntropyFactory>(),
        descriptor_wrapper,
        settings);
  }

}
