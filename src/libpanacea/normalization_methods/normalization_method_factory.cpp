
// Local private includes
#include "normalization_method_factory.hpp"

#include "error.hpp"
#include "normalization_method_base.hpp"
#include "passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  std::unordered_map<settings::KernelNormalization,
    NormalizationMethodFactory::NormalizationMethod>
      NormalizationMethodFactory::normalization_methods_;

  std::unique_ptr<BaseNormalizationMethod> NormalizationMethodFactory::create(
          BaseDescriptorWrapper * desc_wrapper,
          const settings::KernelNormalization & norm_method) const {

    if( normalization_methods_.count(norm_method) == 0) {
      std::string error_msg = "Normalization Method is not supported.";
      PANACEA_FAIL(error_msg);
    }
   
    const PassKey<NormalizationMethodFactory> key;
    return normalization_methods_[norm_method](key);
  }

}
