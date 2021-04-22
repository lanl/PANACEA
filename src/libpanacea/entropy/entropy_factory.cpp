
// Local private PANACEA includes
#include "entropy_factory.hpp"

#include "distribution/distribution_factory.hpp"
#include "entropy_settings/entropy_settings.hpp"
#include "entropy_terms/entropy_term.hpp"
#include "error.hpp"
#include "settings.hpp"

// Standard includes
#include <string>
#include <unordered_map>

namespace panacea {

  std::unordered_map<settings::EntropyType,
    EntropyFactory::EntropyCreateMethod>
        EntropyFactory::create_methods_;

  std::unique_ptr<EntropyTerm> EntropyFactory::create(
      BaseDescriptorWrapper * descriptor_wrapper,
      MemoryManager & mem_manager,
      EntropySettings * settings,
      std::string name) const {

    assert(settings != nullptr);

    if(create_methods_.count(settings->type) == 0){
      std::string error_msg = "Entropy type is not registered with the factory.";
      PANACEA_FAIL(error_msg);
    }

    return create_methods_.at(settings->type)(
        PassKey<EntropyFactory>(),
        descriptor_wrapper,
        mem_manager,
        settings,
        name);
  }

}
