// Local private PANACEA includes
#include "normalization_method_none.hpp"

// Standard includes
#include <numeric>
#include <vector>

namespace panacea {

  std::vector<double> NormalizationMethodNone::generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const {
    std::vector<double> coeffs(desc_wrapper->getNumberReducedDimensions(), 1.0);
    return coeffs; 
  }
}
