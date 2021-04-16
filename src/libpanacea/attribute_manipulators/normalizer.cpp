
// Local private PANACEA includes
#include "normalizer.hpp"
#include "passkey.hpp"

// Standard includes
#include <cassert>

namespace panacea {
  
  Normalizer::Normalizer(const std::vector<double> & normalization_coeffs, const NormalizerOption opt) {

    if( opt == NormalizerOption::Strict ) {
      normalization_coeffs_ = normalization_coeffs;
    } else {
      normalization_coeffs_.reserve(normalization_coeffs.size());
      for( const auto & coef : normalization_coeffs ) {
        if( coef == 0.0 ) {
          normalization_coeffs_.push_back(1.0);
        } else {
          normalization_coeffs_.push_back(coef);
        }
      }
    }
  }

  void Normalizer::normalize(Covariance & cov) const {
    assert(cov.cols() == normalization_coeffs_.size());

    // Division is expensive so do it once and then use the inverse
    std::vector<double> inv_coeffs;
    inv_coeffs.reserve(normalization_coeffs_.size());
    for(int col = 0; col < cov.cols(); ++col) {
      assert(normalization_coeffs_.at(col) != 0.0);
      inv_coeffs.push_back(1.0/normalization_coeffs_.at(col));
    }
    
    for( int row = 0; row < cov.rows(); ++row){
      for(int col = row; col < cov.cols(); ++col) {
        cov(PassKey<Normalizer>(), row,col) *= inv_coeffs.at(col);
        cov(PassKey<Normalizer>(), row,col) *= inv_coeffs.at(row);
        cov(PassKey<Normalizer>(), col,row) = cov(row,col);
      }
    }
    cov.set(PassKey<Normalizer>(), NormalizationState::Normalized);
  }

  void Normalizer::unnormalize(Covariance & cov) const {

    assert(cov.cols() == normalization_coeffs_.size());
    for( int row = 0; row < cov.rows(); ++row){
      for(int col = 0; col < cov.cols(); ++col) {
        cov(PassKey<Normalizer>(), row,col) *= normalization_coeffs_.at(col);
        cov(PassKey<Normalizer>(), row,col) *= normalization_coeffs_.at(row);
      }
    }
    cov.set(PassKey<Normalizer>(), NormalizationState::Unnormalized);
  }

  const std::vector<double> & Normalizer::getNormalizationCoeffs() const noexcept {
    return normalization_coeffs_;
  }
}

