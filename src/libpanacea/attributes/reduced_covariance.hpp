
#ifndef PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#pragma once

// Local private PANACEA includes
#include "passkey.hpp"

// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class Inverter;
  class Reducer;

  class ReducedCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
      std::vector<int> chosen_dimension_indices_; 
    public:
      ReducedCovariance(PassKey<Reducer>);
      ReducedCovariance(PassKey<Reducer> key, 
          std::unique_ptr<Matrix> matrix,
          std::vector<int> chosen_dimension_indices) :
        matrix_(std::move(matrix)),
        chosen_dimension_indices_(chosen_dimension_indices) {};

      const Matrix * get(PassKey<Inverter>) const;

      ReducedCovariance() = delete;

      double operator()(const int row, const int col) const;

      double getDeterminant() const;
      int getNumberDimensions() const;
      const std::vector<int> & getChosenDimensionIndices() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
