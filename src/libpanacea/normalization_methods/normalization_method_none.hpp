#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_NONE_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_NONE_H
#pragma once

// Local private includes
#include "normalization_method_base.hpp"
#include "passkey.hpp"
#include "settings.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class BaseDescriptorWrapper;
  class NormalizationMethodFactory;

  class NormalizationMethodNone : public BaseNormalizationMethod {
    public:
      explicit NormalizationMethodNone(const PassKey<NormalizationMethodFactory> &) {};
      virtual std::vector<double> generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const final;

      static std::unique_ptr<BaseNormalizationMethod> create(const PassKey<NormalizationMethodFactory> &);

      static const settings::KernelNormalization type;
  };
  
  inline std::unique_ptr<BaseNormalizationMethod> NormalizationMethodNone::create(
      const PassKey<NormalizationMethodFactory> & key) {
    return std::make_unique<NormalizationMethodNone>(key);
  }
}
#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_NONE_H
