
// Local private includes
#include "entropy/entropy_terms/self_entropy.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "entropy/entropy_factory.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "entropy/entropy_terms/entropy_decorators/numerical_grad.hpp"
#include "entropy/entropy_terms/entropy_decorators/weight.hpp"
#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <memory>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:self entropy","[unit,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

  KernelDistributionSettings kernel_settings;
  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Strict
      ));

  EntropySettings settings;
  settings.type = settings::EntropyType::Self;
  settings.dist_settings = std::make_unique<KernelDistributionSettings>(kernel_settings);

  EntropyFactory entropy_factory;

  auto entropy_term = entropy_factory.create(
      &dwrapper_init,
      &settings);

  WHEN("Testing Numerical Decorator") {
    double val1 = entropy_term->compute(&dwrapper_init);
    std::cout << "Self entropy is " << val1 << " at initial positions." << std::endl;

    data.at(0).at(0) = 1.5;
    double val2 = entropy_term->compute(&dwrapper_init);
    std::cout << "Self entropy should decrease when descriptors moved closer to each other " << val2 << std::endl;

    REQUIRE( val2 < val1 );

    auto analy_grad = entropy_term->compute_grad(&dwrapper_init,0, settings);
    std::cout << "Analytical self entropy grad of first descriptor " << analy_grad.at(0) << std::endl;

    entropy_term = std::make_unique<NumericalGrad>(std::move(entropy_term));

    auto numer_grad = entropy_term->compute_grad(&dwrapper_init, 0, settings);
    std::cout << "Numerical self entropy grad of first descriptor " << numer_grad.at(0) << std::endl;

    REQUIRE( numer_grad.at(0) == Approx(analy_grad.at(0)));
    REQUIRE( numer_grad.at(0) < 0.0 );
  }
  WHEN("Testing Numerical Decorator with Weighting Decorator") {

    double val1 = entropy_term->compute(&dwrapper_init);
    auto grad1 = entropy_term->compute_grad(&dwrapper_init,0, settings);

    // Just flip the sign
    entropy_term = std::make_unique<Weight>(std::move(entropy_term),-1.0);
    double val2 = entropy_term->compute(&dwrapper_init);
    auto grad2 = entropy_term->compute_grad(&dwrapper_init,0, settings);

    REQUIRE(std::abs(val1) == Approx(std::abs(val2)));
    REQUIRE(val1 != Approx(val2));

    REQUIRE(std::abs(grad1.at(0)) == Approx(std::abs(grad2.at(0))));
    REQUIRE(grad1.at(0) != Approx(grad2.at(0)));
  }
}
