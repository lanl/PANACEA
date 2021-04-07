#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
#pragma once

namespace panacea {

  class NormalizationMethodVariance : public BaseNormalizationMethod {
    public:
      virtual std::vector<double> generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const final;

      static std::unique_ptr<BaseNormalizationMethod> create();
  };
}
#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_VARIANCE_H
