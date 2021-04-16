
#ifndef PANACEA_PRIVATE_NORMALIZER_H
#define PANACEA_PRIVATE_NORMALIZER_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"

// Standard includes
#include <vector>

namespace panacea {

  /**
   * If 0.0 are encountered:
   * Flexible - will set normalization constant to 1.0
   * Strict - will set to 0.0
   **/
  enum class NormalizerOption {
    Flexible,
    Strict
  };

  /*
   * The only thing that gets normalized and unnormalized is
   * the covariance matrix. 
   *
   * It would be unnormalized before the covariance matrix could
   * be updated and normalized afterwards. 
   **/
  class Normalizer {
      std::vector<double> normalization_coeffs_;
    public:
      Normalizer() = default;
      Normalizer(const std::vector<double> & normalization_coeffs, 
          const NormalizerOption opt = NormalizerOption::Strict);
        
      const std::vector<double> & getNormalizationCoeffs() const noexcept;

      void normalize(Covariance & cov) const; 
      void unnormalize(Covariance & cov) const; 
  };
}
#endif // PANACEA_PRIVATE_NORMALIZER_H
