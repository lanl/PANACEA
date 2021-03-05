
#ifndef PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#define PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
#pragma once

namespace panacea {

  class ReducedCovariance {
    private:
      Matrix matrix_;
    public:
      double getDeterminant() const;
  };
}

#endif // PANACEA_PRIVATE_REDUCEDCOVARIANCE_H
