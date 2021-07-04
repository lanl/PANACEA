
// Private local PANACEA includes
#include "dimension_limiter.hpp"

#include "attributes/dimensions.hpp"

// Public PANACEA includes
#include "panacea/passkey.hpp"

// Standard includes
#include <cassert>
#include <vector>

namespace panacea {

  void DimensionLimiter::limit(
      Dimensions & dimensions,
      const int dim_limit) const {

    assert(dim_limit >= 1);
    auto & dims = dimensions.get(PassKey<DimensionLimiter>());
    dims.resize(dim_limit);
  }
}

