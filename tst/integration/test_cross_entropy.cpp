
// Local private includes
#include "entropy/entropy_terms/cross_entropy.hpp"

#include "constants.hpp"
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

TEST_CASE("Testing:cross entropy","[integration,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 3.0},
    {2.0, 5.0},
    {6.0, 7.0}};

  // This should end up putting the cross entropy term centered at
  //
  // col1  col2
  // 3.0   5.0

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

  GIVEN("A single correlated primitive.") {
    KernelDistributionSettings kernel_settings;
    kernel_settings.dist_settings = std::move(KernelSpecification(
          settings::KernelCorrelation::Correlated,
          settings::KernelCount::Single,
          settings::KernelPrimitive::Gaussian,
          settings::KernelNormalization::None,
          settings::KernelMemory::Own,
          settings::KernelCenterCalculation::Mean,
          settings::KernelAlgorithm::Flexible,
          settings::RandomizeDimensions::No,
          settings::RandomizeNumberDimensions::No,
          constants::automate
        ));

    EntropySettings settings;
    settings.type = settings::EntropyType::Cross;
    settings.setDistributionSettings(std::move(std::make_unique<KernelDistributionSettings>(kernel_settings)));

    EntropyFactory entropy_factory;

    WHEN("Testing Numerical Decorator centered over kernel") {
      // entropy term needs to be created here because it is moved when decorated
      auto entropy_term = entropy_factory.create(
          dwrapper_init,
          settings);

      REQUIRE(entropy_term != nullptr);
      REQUIRE(entropy_term.get() != nullptr);
      std::vector<std::vector<double>> data_sample_max{
      {3.0, 5.0}};
      DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_sample_max(&data_sample_max, 1, 2);

      double val1 = entropy_term->compute(dwrapper_sample_max, settings);
      std::cout << "Cross entropy is " << val1 << " at what should be the center of the cross entropy term." << std::endl;

      data_sample_max.at(0).at(0) = 2.8;
      double val2 = entropy_term->compute(dwrapper_sample_max, settings);
      std::cout << "Cross entropy should increase when descriptors move away from the center of the cross entropy term: " << val2 << std::endl;

      REQUIRE( val2 > val1 );

      // Recenter the sample data point
      data_sample_max.at(0).at(0) = 3.0;
      auto analy_grad = entropy_term->compute_grad(dwrapper_sample_max,0, settings);
      std::cout << "Analytical cross entropy grad of sample point should be located at the kernel center should be 0.0 it is: " << analy_grad.at(0) << std::endl;

      auto entropy_term_numer = std::make_unique<NumericalGrad>(std::move(entropy_term));

      auto numer_grad = entropy_term_numer->compute_grad(dwrapper_sample_max, 0, settings);
      std::cout << "Numerical cross entropy grad of sample point located at the kernel center is: " << numer_grad.at(0) << std::endl;

      REQUIRE( numer_grad.at(0) == Approx(analy_grad.at(0)));

      // Now if we use the location of the sample point such that it is a little removed

    }

    WHEN("Testing Numerical Decorator to the side of the kernel") {

      // entropy term needs to be created here because it is moved when decorated
      auto entropy_term = entropy_factory.create(
          dwrapper_init,
          settings);

      REQUIRE(entropy_term != nullptr);
      REQUIRE(entropy_term.get() != nullptr);
      std::vector<std::vector<double>> data_sample_max{
      {2.8, 5.0}};
      DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_sample_max(&data_sample_max, 1, 2);

      auto analy_grad = entropy_term->compute_grad(dwrapper_sample_max,0, settings);
      std::cout << "Analytical cross entropy grad of sample point should to the side of the kernel center should be negative: " << analy_grad.at(0) << std::endl;

      auto entropy_term_numer2 = std::make_unique<NumericalGrad>(std::move(entropy_term));

      auto numer_grad = entropy_term_numer2->compute_grad(dwrapper_sample_max, 0, settings);
      std::cout << "Numerical cross entropy grad of sample point located to the side of the kernel center is: " << numer_grad.at(0) << std::endl;

      REQUIRE( numer_grad.at(0) == Approx(analy_grad.at(0)));

      REQUIRE( numer_grad.at(0) < 0.0);
      // Now if we use the location of the sample point such that it is a little removed
    }
  }
  GIVEN("A single uncorrelated primitive.") {
    KernelDistributionSettings kernel_settings;
    kernel_settings.dist_settings = std::move(KernelSpecification(
          settings::KernelCorrelation::Uncorrelated,
          settings::KernelCount::Single,
          settings::KernelPrimitive::Gaussian,
          settings::KernelNormalization::None,
          settings::KernelMemory::Own,
          settings::KernelCenterCalculation::Mean,
          settings::KernelAlgorithm::Flexible,
          settings::RandomizeDimensions::No,
          settings::RandomizeNumberDimensions::No,
          constants::automate
        ));

    EntropySettings settings;
    settings.type = settings::EntropyType::Cross;
    settings.setDistributionSettings(std::move(std::make_unique<KernelDistributionSettings>(kernel_settings)));

    EntropyFactory entropy_factory;

    WHEN("Testing Numerical Decorator centered over kernel") {
      // entropy term needs to be created here because it is moved when decorated
      auto entropy_term = entropy_factory.create(
          dwrapper_init,
          settings);

      REQUIRE(entropy_term != nullptr);
      REQUIRE(entropy_term.get() != nullptr);
      std::vector<std::vector<double>> data_sample_max{
      {3.0, 5.0}};
      DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_sample_max(&data_sample_max, 1, 2);

      double val1 = entropy_term->compute(dwrapper_sample_max, settings);
      std::cout << "Cross entropy is " << val1 << " at what should be the center of the cross entropy term." << std::endl;

      data_sample_max.at(0).at(0) = 2.8;
      double val2 = entropy_term->compute(dwrapper_sample_max, settings);
      std::cout << "Cross entropy should increase when descriptors move away from the center of the cross entropy term: " << val2 << std::endl;

      REQUIRE( val2 > val1 );

      // Recenter the sample data point
      data_sample_max.at(0).at(0) = 3.0;
      auto analy_grad = entropy_term->compute_grad(dwrapper_sample_max,0, settings);
      std::cout << "Analytical cross entropy grad of sample point should be located at the kernel center should be 0.0 it is: " << analy_grad.at(0) << std::endl;

      auto entropy_term_numer = std::make_unique<NumericalGrad>(std::move(entropy_term));

      auto numer_grad = entropy_term_numer->compute_grad(dwrapper_sample_max, 0, settings);
      std::cout << "Numerical cross entropy grad of sample point located at the kernel center is: " << numer_grad.at(0) << std::endl;

      REQUIRE( numer_grad.at(0) == Approx(analy_grad.at(0)));

      // Now if we use the location of the sample point such that it is a little removed

    }

    WHEN("Testing Numerical Decorator to the side of the kernel") {

      // entropy term needs to be created here because it is moved when decorated
      auto entropy_term = entropy_factory.create(
          dwrapper_init,
          settings);

      REQUIRE(entropy_term != nullptr);
      REQUIRE(entropy_term.get() != nullptr);
      std::vector<std::vector<double>> data_sample_max{
      {2.8, 5.0}};
      DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_sample_max(&data_sample_max, 1, 2);

      auto analy_grad = entropy_term->compute_grad(dwrapper_sample_max,0, settings);
      std::cout << "Analytical cross entropy grad of sample point should to the side of the kernel center should be negative: " << analy_grad.at(0) << std::endl;

      auto entropy_term_numer2 = std::make_unique<NumericalGrad>(std::move(entropy_term));

      auto numer_grad = entropy_term_numer2->compute_grad(dwrapper_sample_max, 0, settings);
      std::cout << "Numerical cross entropy grad of sample point located to the side of the kernel center is: " << numer_grad.at(0) << std::endl;

      REQUIRE( numer_grad.at(0) == Approx(analy_grad.at(0)));

      REQUIRE( numer_grad.at(0) < 0.0);
      // Now if we use the location of the sample point such that it is a little removed
    }
  }

}

