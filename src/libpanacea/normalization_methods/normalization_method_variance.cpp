
// Local private PANACEA includes
#include "normalization_method_variance.hpp"

#include "variance.hpp"

// Standard includes
#include <vector>

namespace panacea {

 std::vector<double> NormalizationMethodVariance::generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const {
    
   Variance variance;
   return variance.calculate(desc_wrapper);
  }
}
