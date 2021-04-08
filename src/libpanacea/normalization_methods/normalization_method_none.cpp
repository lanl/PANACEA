// Local private PANACEA includes
#include "normalization_method_none.hpp"

#include "descriptors/base_descriptor_wrapper.hpp"

// Standard includes
#include <numeric>
#include <vector>

namespace panacea {

  std::vector<double> NormalizationMethodNone::generateCoefficients(
          BaseDescriptorWrapper * desc_wrapper) const {
    std::vector<double> coeffs(desc_wrapper->getNumberReducedDimensions(), 1.0);
    return coeffs; 
  }

  const settings::KernelNormalization NormalizationMethodNone::type = 
        settings::KernelNormalization::None;

}
