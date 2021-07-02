
#ifndef PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#pragma once

// Local private PANACEA includes
#include "data_settings.hpp"
#include "matrix/matrix.hpp"
#include "dimensions.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {
  class Inverter;
  class Reducer;

  class ReducedCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
      Dimensions chosen_dimension_indices_;

      NormalizationState normalized_ = NormalizationState::Unnormalized;
    public:
      ReducedCovariance(PassKey<Reducer>);
      ReducedCovariance(PassKey<Reducer> key,
          std::unique_ptr<Matrix> matrix,
          const Dimensions & chosen_dimension_indices,
          const NormalizationState & normalized) :
        matrix_(std::move(matrix)),
        chosen_dimension_indices_(chosen_dimension_indices),
        normalized_(normalized) {};

      const Matrix & get(PassKey<Inverter>) const;

      ReducedCovariance() = delete;

      double operator()(const int row, const int col) const;

      void print() const;

      bool is(const NormalizationState & state) const noexcept;
      const NormalizationState & getNormalizationState() const noexcept;

      double getDeterminant() const;
      int getNumberDimensions() const;
      const Dimensions & getReducedDimensions() const noexcept;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
