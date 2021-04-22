
#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
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

  class NormalizationMethodVariance : public BaseNormalizationMethod {
    public:
      explicit NormalizationMethodVariance(const PassKey<NormalizationMethodFactory> &) {};
      virtual std::vector<double> generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const final;

      static std::unique_ptr<BaseNormalizationMethod> create(const PassKey<NormalizationMethodFactory> &);

      static const settings::KernelNormalization type;
  };

  inline std::unique_ptr<BaseNormalizationMethod> NormalizationMethodVariance::create(
      const PassKey<NormalizationMethodFactory> & key) {
    return std::make_unique<NormalizationMethodVariance>(key);
  }
}
#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
