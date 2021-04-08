
#ifndef PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#pragma once

// Local public includes
#include "panacea/matrix.hpp"

#include "passkey.hpp"
#include "data_settings.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class Inverter;

  class ReducedInvCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
      std::vector<int> chosen_dimension_indices_; 

      NormalizationState normalized_ = NormalizationState::Unnormalized;
    public:
      ReducedInvCovariance() = delete;
      ReducedInvCovariance(PassKey<Inverter>,
          std::unique_ptr<Matrix> matrix,
          const std::vector<int> & chosen_dimension_indices,
          const NormalizationState & normalized) :
        matrix_(std::move(matrix)),
        chosen_dimension_indices_(chosen_dimension_indices),
        normalized_(normalized) {};

      double operator()(const int row, const int col) const;

      void print() const;
      int getNumberDimensions() const;
      const std::vector<int> & getChosenDimensionIndices() const;

      bool is(const NormalizationState & state) const noexcept;
      const NormalizationState & getNormalizationState() const noexcept;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
