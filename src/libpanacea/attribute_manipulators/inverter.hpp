#ifndef PANACEA_PRIVATE_INVERTER_H
#define PANACEA_PRIVATE_INVERTER_H
#pragma once

namespace panacea {

class ReducedCovariance;
class ReducedInvCovariance;

class Inverter {
public:
  ReducedInvCovariance invert(const ReducedCovariance &reduced_cov) const;
};
} // namespace panacea

#endif // PANACEA_PRIVATE_INVERTER_H
