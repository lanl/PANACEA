
#ifndef PANACEA_PRIVATE_COVARIANCE_H
#define PANACEA_PRIVATE_COVARIANCE_H
#pragma once

// Local PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"
#include "passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  class BaseDescriptorWrapper;

  enum class NormalizationState {
    Normalized,
    Unnormalized
  };

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
      explicit Covariance(BaseDescriptorWrapper * desc_wrap);
      Covariance(std::unique_ptr<Matrix> matrix, std::unique_ptr<Vector> mean, int total_num_pts);

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

      bool is(const NormalizationState state) const;
      // Specific to Normalizer class
      void set(PassKey<Normalizer>, NormalizationState state);
      double & operator()(PassKey<Normalizer>, const int row, const int col);

      void print() const;
  };
}
#endif // PANACEA_PRIVATE_COVARIANCE_H
