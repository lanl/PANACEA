
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

  class Reducer;

  class ReducedCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;

    public:
      ReducedCovariance(PassKey<Reducer>);
      ReducedCovariance(PassKey<Reducer> key, std::unique_ptr<Matrix> matrix) :
        matrix_(std::move(matrix)) {};

      ReducedCovariance() = delete;

      double getDeterminant() const;
      int getNumberDimensions() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
