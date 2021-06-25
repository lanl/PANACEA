// Local private PANACEA includes
#include "attribute_manipulators/inverter.hpp"

#include "attributes/reduced_inv_covariance.hpp"
#include "attributes/reduced_covariance.hpp"

// Local public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <iostream>

namespace panacea {

      ReducedInvCovariance
        Inverter::invert(const ReducedCovariance & reduced_cov) const {
          auto inv_matrix = pseudoInverse(reduced_cov.get(PassKey<Inverter>()));

          ReducedInvCovariance reduced_inv_cov(
              PassKey<Inverter>(),
              std::move(inv_matrix),
              reduced_cov.getReducedDimensions(),
              reduced_cov.getNormalizationState());

          return reduced_inv_cov;
      }
}

