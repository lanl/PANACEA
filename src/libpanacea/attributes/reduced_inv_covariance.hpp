
#ifndef PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
#pragma once

// Local public includes
#include "panacea/matrix.hpp"

#include "passkey.hpp"

// Standard includes
#include <memory>

namespace panacea {

  class Inverter;

  class ReducedInvCovariance {
    private:
      std::unique_ptr<Matrix> matrix_;
    public:
      ReducedInvCovariance() = delete;
      ReducedInvCovariance(PassKey<Inverter>, std::unique_ptr<Matrix> matrix) :
        matrix_(std::move(matrix)) {};
      double operator()(const int row, const int col) const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDINVCOVARIANCE_H
