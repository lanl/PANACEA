
// Local private PANACEA includes
#include "normalization_methods/normalization_method_factory.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "normalization_methods/normalization_method_variance.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:normalization_method_factory","[unit,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data, 3, 2);

  NormalizationMethodFactory norm_method_factory;

  norm_method_factory.registerNormalizationMethod<
    NormalizationMethodVariance,
    settings::KernelNormalization::Variance>();

  auto norm_method = norm_method_factory.create(&dwrapper, settings::KernelNormalization::Variance);

  std::vector<double> norm_coefficients = norm_method->generateCoefficients(&dwrapper);

  // Only be two dimensions
  REQUIRE(norm_coefficients.size() == 2);

  std::cout << "Variance of dimensions " << std::endl;
  for ( auto & coef : norm_coefficients) {
    std::cout << coef << std::endl;
  }

}

