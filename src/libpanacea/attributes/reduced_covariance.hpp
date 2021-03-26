
#ifndef PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#pragma once

// Local private PANACEA includes
#include "passkey.hpp"

// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Standard includes
#include <memory>

namespace panacea {

  class Inverter;
  class Reducer;

  class ReducedCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;

    public:
      ReducedCovariance(PassKey<Reducer>);
      ReducedCovariance(PassKey<Reducer> key, std::unique_ptr<Matrix> matrix) :
        matrix_(std::move(matrix)) {};

      const Matrix * get(PassKey<Inverter>) const;

      ReducedCovariance() = delete;

      double operator()(const int row, const int col) const;

      double getDeterminant() const;
      int getNumberDimensions() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
