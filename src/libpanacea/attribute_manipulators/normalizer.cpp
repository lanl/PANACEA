
// Local private PANACEA includes
#include "normalizer.hpp"
#include "passkey.hpp"

// Standard includes
#include <cassert>

namespace panacea {
  
  void Normalizer::normalize(Covariance & cov) const {
    assert(cov.cols() == normalization_coeffs_.size());

    // Division is expensive so do it once and then use the inverse
    std::vector<double> inv_coeffs;
    inv_coeffs.reserve(normalization_coeffs_.size());
    for(int col = 0; col < cov.cols(); ++col) {
      inv_coeffs.push_back(1.0/normalization_coeffs_.at(col));
    }
    
    for( int row = 0; row < cov.rows(); ++row){
      for(int col = 0; col < cov.cols(); ++col) {
        cov(PassKey<Normalizer>(), row,col) *= inv_coeffs.at(col);
      }
    }
    cov.set(PassKey<Normalizer>(), NormalizationState::Normalized);
  }

  void Normalizer::unnormalize(Covariance & cov) const {

    assert(cov.cols() == normalization_coeffs_.size());
    for( int row = 0; row < cov.rows(); ++row){
      for(int col = 0; col < cov.cols(); ++col) {
        cov(PassKey<Normalizer>(), row,col) *= normalization_coeffs_.at(col);
      }
    }
    cov.set(PassKey<Normalizer>(), NormalizationState::Unnormalized);
  }

  const std::vector<double> & Normalizer::getNormalizationCoeffs() const noexcept {
    return normalization_coeffs_;
  }
}

