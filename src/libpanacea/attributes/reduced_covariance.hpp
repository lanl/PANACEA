
#ifndef PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#pragma once

// Local public PANACEA includes
#include "panacea/matrix.hpp"

// Standard includes
#include <memory>

namespace panacea {

  class ReducedCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
    public:
      ReducedCovariance();
      ReducedCovariance(std::unique_ptr<Matrix> matrix) :
        matrix_(std::move(matrix)) {};

      double getDeterminant() const;
      int getNumberDimensions() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
