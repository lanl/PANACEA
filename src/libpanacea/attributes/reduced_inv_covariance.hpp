
#ifndef PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#pragma once

// Local public includes
#include "panacea/matrix.hpp"

#include "passkey.hpp"

// Standard includes
#include <memory>
#include <vector>

namespace panacea {

  class Inverter;

  class ReducedInvCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
      std::vector<int> chosen_dimension_indices_; 
    public:
      ReducedInvCovariance() = delete;
      ReducedInvCovariance(PassKey<Inverter>,
          std::unique_ptr<Matrix> matrix,
          const std::vector<int> & chosen_dimension_indices) :
        matrix_(std::move(matrix)),
        chosen_dimension_indices_(chosen_dimension_indices) {};

      double operator()(const int row, const int col) const;

      void print() const;
      int getNumberDimensions() const;
      const std::vector<int> & getChosenDimensionIndices() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
