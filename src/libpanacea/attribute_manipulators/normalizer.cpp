
// Local private PANACEA includes
#include "normalizer.hpp"

#include "normalization_methods/normalization_method_factory.hpp"
#include "passkey.hpp"

// Standard includes
#include <cassert>
#include <vector>

namespace panacea {

  /********************************************************
   * Local functions
   ********************************************************/ 
  namespace {

    /**
     * If set to flexible will ensure that none of the norm
     * coefficients are 0.0.
     **/
    void trim(
        const std::vector<double> & in,
        const NormalizerOption & opt,
        std::vector<double> & out){

      out.clear();
      if( opt == NormalizerOption::Strict ) {
        out = in;
      } else {
        out.reserve(in.size());
        for( const auto & coef : in ) {
          if( coef == 0.0 ) {
            out.push_back(1.0);
          } else {
            out.push_back(coef);
          }
        }
      }
    }

  }

  /********************************************************
   * Public methods
   ********************************************************/ 

  Normalizer::Normalizer(const std::vector<double> & normalization_coeffs,
      const NormalizerOption opt) : norm_option_(opt) {
    trim(normalization_coeffs, norm_option_, normalization_coeffs_);
  }

  Normalizer::Normalizer(const BaseDescriptorWrapper * dwrapper,
      const settings::KernelNormalization & norm_method, 
      const NormalizerOption opt) : norm_option_(opt) {

    NormalizationMethodFactory norm_method_factory;
    norm_method_ = norm_method_factory.create(norm_method);
    auto extra_settings = settings::None::None;
    const auto normalization_coeffs = norm_method_(dwrapper, extra_settings);
    trim(normalization_coeffs, norm_option_, normalization_coeffs_);

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

  void Normalizer::update(const BaseDescriptorWrapper * dwrapper, std::any extra_args) {

    assert(norm_method_ != nullptr);
    const auto normalization_coeffs = norm_method_(dwrapper, extra_args);
    trim(normalization_coeffs, norm_option_, normalization_coeffs_);
  }
}

