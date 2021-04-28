
// Public PANACEA includes
#include "panacea/panacea.hpp"

#include "panacea/entropy_term.hpp"

// Local private PANACEA includes
#include "descriptors/descriptor_wrapper.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "entropy/entropy_factory.hpp"
#include "error.hpp"

// Standard includes
#include <memory>
#include <string>
#include <vector>

namespace panacea {

  /**
   * Only certain data types can be wrapped
   **/
  std::unique_ptr<BaseDescriptorWrapper> PANACEA::wrap(std::any data, const int rows, const int cols) const {
/*    if(data.type() == typeid(double **) ) {
      return std::make_unique<DescriptorWrapper<double **>>(
          std::any_cast<double **>(data),
          rows,
          cols);
    }*/
    if(data.type() == typeid(std::vector<std::vector<double>> *) ) {
      return std::make_unique<DescriptorWrapper<std::vector<std::vector<double>> *>>(
          std::any_cast<std::vector<std::vector<double>> *>(data),
          rows,
          cols);
    }
    std::string error_msg = "Tried to wrap an unsuppored data type";
    PANACEA_FAIL(error_msg);
  
    std::vector<std::vector<double>> empty_vec;
    return std::make_unique<DescriptorWrapper<std::vector<std::vector<double>>>>(
        empty_vec,0,0);
  }

  std::unique_ptr<EntropyTerm> PANACEA::create(
      const BaseDescriptorWrapper & dwrapper,
      const PANACEASettings & settings) const {

    EntropySettings entropy_settings(settings);
    EntropyFactory entropy_factory;
    return entropy_factory.create(&dwrapper, &entropy_settings);
  }
}
