#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
#pragma once

// Local private includes
#include "normalization_method_base.hpp"
#include "passkey.hpp"

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

      static std::unique_ptr<BaseNormalizationMethod> create();
  };
}
#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
