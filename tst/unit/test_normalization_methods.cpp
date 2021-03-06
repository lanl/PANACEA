
// Local private PANACEA includes
#include "normalization_methods/normalization_method_factory.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "private_settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:normalization_method_factory", "[unit,panacea]") {
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{{1.0, 3.0}, {2.0, 5.0}, {3.0, 7.0}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(&data, 3, 2);

  NormalizationMethodFactory norm_method_factory;

  auto norm_method =
      norm_method_factory.create(settings::KernelNormalization::Variance);

  auto extra_args = settings::None::None;
  std::vector<double> norm_coefficients = norm_method(dwrapper, extra_args);

  // Only be two dimensions
  REQUIRE(norm_coefficients.size() == 2);

  REQUIRE(norm_coefficients.at(0) == Approx(1.0));
  // 2 not 4 because it is the sqrt of the variance
  REQUIRE(norm_coefficients.at(1) == Approx(2.0));
}
