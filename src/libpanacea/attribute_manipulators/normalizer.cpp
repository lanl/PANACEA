
// Local private PANACEA includes
#include "normalizer.hpp"

#include "error.hpp"
#include "normalization_methods/normalization_method_factory.hpp"
#include "passkey.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <cassert>
#include <iostream>
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

  Normalizer::Normalizer(
      const settings::KernelNormalization & norm_method, 
      const NormalizerOption opt) : norm_option_(opt) {

    NormalizationMethodFactory norm_method_factory;
    norm_method_ = norm_method_factory.create(norm_method);
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

  std::vector<std::any> Normalizer::write(
      const settings::FileType & file_type,
      std::ostream & os,
      std::any norm_instance) {


    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      auto normalizer = std::any_cast<Normalizer *>(norm_instance);

      os << "[Normalization]\n";
      os << normalizer->norm_option_ << "\n";
      os << normalizer->normalization_coeffs_.size() << "\n";
      os << "[Normalization Coefficients]\n";
      for( const auto & coef : normalizer->normalization_coeffs_){
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << coef << "\n";
      }
    }
    return nested_values;
  }

  io::ReadInstantiateVector Normalizer::read(
      const settings::FileType & file_type,
      std::istream & is,
      std::any norm_instance) {

    if( file_type == settings::FileType::TXTRestart ) {
      auto normalizer = std::any_cast<Normalizer *>(norm_instance);
      
      std::string line;
      std::getline(is, line);
      // First line should be header
      while(line.find("[Normalization]", 0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Missing [Normalization] tag while reading normalization ";
          error_msg += "section of restart file.\n";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }
      is >> normalizer->norm_option_;

      int rows;
      try { 
        is >> rows;
      } catch (...) {
        std::string error_msg = "Unable to read in rows from [Normalization] section ";
        error_msg += "of restart file.\n";
        error_msg += "line is: " + line + "\n";
        PANACEA_FAIL(error_msg);
      }

      while(line.find("[Normalization Coefficients]", 0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Missing [Normalization Coefficients] tag while reading";
          error_msg += " normalization section of restart file.\n";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      try {
        normalizer->normalization_coeffs_.resize(rows);
        for( int row = 0; row < rows; ++row){
          double value;
          is >> value;
          normalizer->normalization_coeffs_.at(row) = value;
        }
      } catch (...) {
        std::string error_msg = "Error encountered while attempting to read in normalization ";
        error_msg += "coefficients from Normalization section of restart file.\n";
        PANACEA_FAIL(error_msg);
      }
    } 
    io::ReadInstantiateVector nested_values;
    return nested_values;
  }

  std::ostream& operator<<(std::ostream& os, const NormalizerOption & norm_opt) {
    if( norm_opt == NormalizerOption::Strict ) {
      os << "Strict";
    } else if (norm_opt == NormalizerOption::Flexible ) {
      os << "Flexible";
    }
    return os;
  }

  std::istream& operator>>(std::istream& is, NormalizerOption & norm_opt) {
    std::string line;
    std::getline(is,line);
    if( line.find("Strict", 0) != std::string::npos ) {
      norm_opt = NormalizerOption::Strict;
    }else if( line.find("Flexible", 0) != std::string::npos ) {
      norm_opt = NormalizerOption::Flexible;
    } else {
      std::string error_msg = "Unrecognized normalization option while reading istream.\n";
      error_msg += "Accepted normalization option settings are:\n";
      error_msg += "Strict\nFlexible\n";
      error_msg += "Line is: " + line + "\n";
      PANACEA_FAIL(error_msg);
    }
    
    return is;
  }
}

