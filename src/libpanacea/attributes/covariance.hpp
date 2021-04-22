
#ifndef PANACEA_PRIVATE_COVARIANCE_H
#define PANACEA_PRIVATE_COVARIANCE_H
#pragma once

// Local PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Loca private includes
#include "data_settings.hpp"
#include "passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  /**
   * If 0.0 are encountered:
   * Flexible - will set normalization constant to 1.0
   * Strict - will set to 0.0
   **/
  enum class CovarianceOption {
    Flexible,
    Strict
  };

  class BaseDescriptorWrapper;
  class Normalizer;
  /*
   * Class for storing the covariance matrix
   */
  class Covariance {
    private:
      /// The covariance matrix
      std::unique_ptr<Matrix> matrix_;
      
      /// Mean of the values used to create the covariance matrix
      std::unique_ptr<Vector> mean_;

      /// Total number of data points used in the creation of the covariance matrix and in updating it
      int total_number_data_pts_ = 0;

      NormalizationState normalized_ = NormalizationState::Unnormalized;
    public:
      Covariance(BaseDescriptorWrapper * desc_wrap, const CovarianceOption opt = CovarianceOption::Strict);
      Covariance(std::unique_ptr<Matrix> matrix, std::unique_ptr<Vector> mean, int total_num_pts, const CovarianceOption opt = CovarianceOption::Strict);

      /// Designed to update the covariance matrix
      void update(BaseDescriptorWrapper * desc_wrap);

      /// Don't want to allow for the matrix to be arbitrarily changed but do want to
      /// provide access to the actual covariance matrix elements
      double operator()(const int row, const int col) const;

      int rows() const;
      int cols() const;

      double getDeterminant() const;

      double getMean(const int index) const;

      int getCummulativeDescPoints() const;

      bool is(const NormalizationState & state) const noexcept;

      /**
       * Check if the covariance matrix is full of 0's, this can occur if all 
       * data passed in is stacked on top of its self. 
       **/
      bool isZero(const double threshold = 1E-9) const noexcept;

      const NormalizationState & getNormalizationState() const noexcept;

      // Specific to Normalizer class
      void set(PassKey<Normalizer>, NormalizationState state);
      double & operator()(PassKey<Normalizer>, const int row, const int col);

      void print() const;
  };
}
#endif // PANACEA_PRIVATE_COVARIANCE_H
