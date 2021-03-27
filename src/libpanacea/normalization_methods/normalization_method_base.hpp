#ifndef PANACEA_PRIVATE_NORMALIZATION_METHOD_BASE_H
#define PANACEA_PRIVATE_NORMALIZATION_METHOD_BASE_H
#pragma once

namespace panacea {

  class BaseNormalizationMethod {
    public:
      virtual std::vector<double> generateCoefficients( 
          BaseDescriptorWrapper * desc_wrapper) const = 0;

  };
}

#endif // PANACEA_PRIVATE_NORMALIZATION_METHOD_BASE_H
