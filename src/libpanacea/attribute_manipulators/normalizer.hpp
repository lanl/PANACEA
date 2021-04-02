
#ifndef PANACEA_PRIVATE_NORMALIZER_H
#define PANACEA_PRIVATE_NORMALIZER_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"

// Standard includes
#include <vector>

namespace panacea {

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
      Normalizer(const std::vector<double> & normalization_coeffs) :
        normalization_coeffs_(normalization_coeffs) {};

      void normalize(Covariance & cov) const; 
      void unnormalize(Covariance & cov) const; 
  };
}
#endif // PANACEA_PRIVATE_NORMALIZER_H
