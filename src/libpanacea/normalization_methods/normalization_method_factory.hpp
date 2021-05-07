
#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
#pragma once

// Local private includes
#include "private_settings.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <algorithm>
#include <any>
#include <memory>
#include <unordered_map>

namespace panacea {

  class BaseDescriptorWrapper;
  class BaseNormalizationMethod;

  class NormalizationMethodFactory {
    public:
      typedef std::vector<double> (*NormalizationMethod)(const BaseDescriptorWrapper * desc_wrapper, std::any);
    private:
      static std::unordered_map<
        settings::KernelNormalization,
        NormalizationMethod> normalization_methods_;
    public:
      NormalizationMethodFactory();

      template<NormalizationMethod norm_method,settings::KernelNormalization opt>
      static bool registerNormalizationMethod(){
        if( normalization_methods_.count(opt) ) {
          return false;
        } else {
          normalization_methods_[opt] = norm_method;
        }
        return true;
      }
  
      NormalizationMethod create(
          const settings::KernelNormalization & norm_method) const;
  };
}

#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
