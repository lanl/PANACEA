
#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
#pragma once

// Local private includes
#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <memory>
#include <unordered_map>

namespace panacea {

  class BaseDescriptorWrapper;
  class BaseNormalizationMethod;

  class NormalizationMethodFactory {

    public:
      using NormalizationMethod = std::unique_ptr<BaseNormalizationMethod>(*)(const PassKey<NormalizationMethodFactory> &);
    private:
      static std::unordered_map<
        settings::KernelNormalization,
        NormalizationMethod> normalization_methods_;
    public:
      template<class T>
      static bool registerNormalizationMethod(){
        if( normalization_methods_.count(T::type) ) {
          return false;
        } else {
          normalization_methods_[T::type] = T::create;
        }
        return true;
      }

      std::unique_ptr<BaseNormalizationMethod> create(
          BaseDescriptorWrapper * desc_wrapper,
          const settings::KernelNormalization & norm_method) const;
  };
}

#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_FACTORY_H
