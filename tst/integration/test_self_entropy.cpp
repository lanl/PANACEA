
// Local private includes
#include "entropy/entropy_terms/self_entropy.hpp"

#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "entropy/entropy_factory.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "entropy/entropy_terms/entropy_decorators/numerical_grad.hpp"
#include "entropy/entropy_terms/entropy_decorators/weight.hpp"
#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"

#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <cmath>
#include <memory>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:self entropy", "[integration,panacea]") {
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper_init(&data, 3,
                                                                      2);

  KernelDistributionSettings kernel_settings;
  kernel_settings.dist_settings = std::move(KernelSpecification(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None, settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Strict, settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No, constants::automate));

  EntropySettings settings;
  settings.type = settings::EntropyType::Self;
  settings.setDistributionSettings(
      std::move(std::make_unique<KernelDistributionSettings>(kernel_settings)));

  EntropyFactory entropy_factory;

  auto entropy_term = entropy_factory.create(dwrapper_init, settings);

  REQUIRE(entropy_term->state() == EntropyTerm::State::Initialized);

  REQUIRE(entropy_term->getDimensions().size() == 1);

  std::cout << "Entropy type " << entropy_term->type() << std::endl;

  WHEN("Testing Numerical Decorator") {
    double val1 = entropy_term->compute(dwrapper_init, settings);
    std::cout << "Self entropy is " << val1 << " at initial positions."
              << std::endl;

    data.at(0).at(0) = 1.5;
    double val2 = entropy_term->compute(dwrapper_init, settings);
    std::cout << "Self entropy should decrease when descriptors moved closer "
                 "to each other "
              << val2 << std::endl;

    REQUIRE(val2 < val1);

    auto analy_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
    std::cout << "Analytical self entropy grad of first descriptor "
              << analy_grad.at(0) << std::endl;
    auto analy_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
    std::cout << "Analytical self entropy grad of second descriptor "
              << analy_grad2.at(0) << std::endl;

    entropy_term = std::make_unique<NumericalGrad>(std::move(entropy_term));

    auto numer_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
    std::cout << "Numerical self entropy grad of first descriptor "
              << numer_grad.at(0) << std::endl;
    auto numer_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
    std::cout << "Numerical self entropy grad of second descriptor "
              << numer_grad2.at(0) << std::endl;

    REQUIRE(numer_grad.at(0) == Approx(analy_grad.at(0)));
    REQUIRE(numer_grad.at(0) < 0.0);
  }
  WHEN("Testing Numerical Decorator with Weighting Decorator") {

    double val1 = entropy_term->compute(dwrapper_init, settings);
    auto grad1 = entropy_term->compute_grad(dwrapper_init, 0, settings);

    // Just flip the sign
    entropy_term = std::make_unique<Weight>(std::move(entropy_term), -1.0);
    double val2 = entropy_term->compute(dwrapper_init, settings);
    auto grad2 = entropy_term->compute_grad(dwrapper_init, 0, settings);

    REQUIRE(std::fabs(val1) == Approx(std::fabs(val2)));
    REQUIRE(val1 != Approx(val2));

    // Check that values are the same
    REQUIRE(std::fabs(grad1.at(0)) == Approx(std::fabs(grad2.at(0))));
    // Check that opposite in direction
    REQUIRE(grad1.at(0) == Approx(grad2.at(0) * -1.0));
  }
}

TEST_CASE("Testing:self entropy with Array Data2", "[integration,panacea]") {
  // 3 points 2 dimensions
  test::ArrayData2 arr;
  // 1.0, 4.0
  // 2.0, 5.0
  // 3.0, 6.0

  DescriptorWrapper<double ***> dwrapper_init(&arr.data, 3, 2);

  GIVEN("An uncorrelated kernel") {

    KernelDistributionSettings kernel_settings;
    kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Strict, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate));

    EntropySettings settings;
    settings.type = settings::EntropyType::Self;
    settings.setDistributionSettings(std::move(
        std::make_unique<KernelDistributionSettings>(kernel_settings)));

    EntropyFactory entropy_factory;

    auto entropy_term = entropy_factory.create(dwrapper_init, settings);

    REQUIRE(entropy_term->state() == EntropyTerm::State::Initialized);

    WHEN("Testing Numerical Decorator") {
      double val1 = entropy_term->compute(dwrapper_init, settings);
      std::cout << "Self entropy is " << val1 << " at initial positions."
                << std::endl;

      arr.data[0][0] = 1.5;
      double val2 = entropy_term->compute(dwrapper_init, settings);
      std::cout << "Self entropy should decrease when descriptors moved closer "
                   "to each other "
                << val2 << std::endl;

      REQUIRE(val2 < val1);

      auto analy_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
      std::cout << "Analytical self entropy grad of first descriptor "
                << analy_grad.at(0) << std::endl;
      auto analy_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
      std::cout << "Analytical self entropy grad of second descriptor "
                << analy_grad2.at(0) << std::endl;

      entropy_term = std::make_unique<NumericalGrad>(std::move(entropy_term));

      REQUIRE(entropy_term->state() == EntropyTerm::State::Initialized);

      auto numer_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
      std::cout << "Numerical self entropy grad of first descriptor "
                << numer_grad.at(0) << std::endl;
      auto numer_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
      std::cout << "Numerical self entropy grad of second descriptor "
                << numer_grad2.at(0) << std::endl;

      REQUIRE(numer_grad.at(0) == Approx(analy_grad.at(0)));
      REQUIRE(numer_grad.at(0) < 0.0);
    }
    WHEN("Testing Numerical Decorator with Weighting Decorator") {

      double val1 = entropy_term->compute(dwrapper_init, settings);
      auto grad1 = entropy_term->compute_grad(dwrapper_init, 0, settings);

      // Just flip the sign
      entropy_term = std::make_unique<Weight>(std::move(entropy_term), -1.0);
      REQUIRE(entropy_term->state() == EntropyTerm::State::Initialized);
      double val2 = entropy_term->compute(dwrapper_init, settings);
      auto grad2 = entropy_term->compute_grad(dwrapper_init, 0, settings);

      REQUIRE(std::fabs(val1) == Approx(std::fabs(val2)));
      REQUIRE(val1 != Approx(val2));

      REQUIRE(std::fabs(grad1.at(0)) == Approx(std::fabs(grad2.at(0))));
      REQUIRE(grad1.at(0) != Approx(grad2.at(0)));
    }
  }
  GIVEN("A correlated kernel") {

    KernelDistributionSettings kernel_settings;
    kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Strict, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate));

    EntropySettings settings;
    settings.type = settings::EntropyType::Self;
    settings.setDistributionSettings(std::move(
        std::make_unique<KernelDistributionSettings>(kernel_settings)));

    EntropyFactory entropy_factory;

    auto entropy_term = entropy_factory.create(dwrapper_init, settings);

    WHEN("Testing Numerical Decorator") {
      double val1 = entropy_term->compute(dwrapper_init, settings);
      std::cout << "Self entropy is " << val1 << " at initial positions."
                << std::endl;

      arr.data[0][0] = 1.5;
      double val2 = entropy_term->compute(dwrapper_init, settings);
      std::cout << "Self entropy should decrease when descriptors moved closer "
                   "to each other "
                << val2 << std::endl;

      REQUIRE(val2 < val1);

      auto analy_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
      std::cout << "Analytical self entropy grad of first descriptor "
                << analy_grad.at(0) << std::endl;
      auto analy_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
      std::cout << "Analytical self entropy grad of second descriptor "
                << analy_grad2.at(0) << std::endl;

      entropy_term = std::make_unique<NumericalGrad>(std::move(entropy_term));

      auto numer_grad = entropy_term->compute_grad(dwrapper_init, 0, settings);
      std::cout << "Numerical self entropy grad of first descriptor "
                << numer_grad.at(0) << std::endl;
      auto numer_grad2 = entropy_term->compute_grad(dwrapper_init, 1, settings);
      std::cout << "Numerical self entropy grad of second descriptor "
                << numer_grad2.at(0) << std::endl;

      REQUIRE(numer_grad.at(0) == Approx(analy_grad.at(0)));
      REQUIRE(numer_grad.at(0) < 0.0);
    }
    WHEN("Testing Numerical Decorator with Weighting Decorator") {

      double val1 = entropy_term->compute(dwrapper_init, settings);
      auto grad1 = entropy_term->compute_grad(dwrapper_init, 0, settings);

      // Just flip the sign
      entropy_term = std::make_unique<Weight>(std::move(entropy_term), -1.0);
      double val2 = entropy_term->compute(dwrapper_init, settings);
      auto grad2 = entropy_term->compute_grad(dwrapper_init, 0, settings);

      REQUIRE(std::fabs(val1) == Approx(std::fabs(val2)));
      REQUIRE(val1 != Approx(val2));

      REQUIRE(std::fabs(grad1.at(0)) == Approx(std::fabs(grad2.at(0))));
      REQUIRE(grad1.at(0) != Approx(grad2.at(0)));
    }
  }
}

TEST_CASE("Testing:self entropy shell + initialize", "[integration,panacea]") {

  /**
   * The purpose of this tests is to check the errors are thrown if a shell
   * entropy term is used before being initialized. This test is also to
   * check that once a shell term has been initialized it has the same output
   * as an entropy term that was constructred and initialized during creation.
   **/

  // Create an entropy shell term
  KernelDistributionSettings kernel_settings;
  kernel_settings.dist_settings = std::move(KernelSpecification(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None, settings::KernelMemory::Own,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Strict, settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No, constants::automate));

  EntropySettings settings;
  settings.type = settings::EntropyType::Self;
  settings.setDistributionSettings(
      std::move(std::make_unique<KernelDistributionSettings>(kernel_settings)));

  EntropyFactory entropy_factory;

  auto entropy_term_shell = entropy_factory.create(settings);

  REQUIRE(entropy_term_shell->state() == EntropyTerm::State::Shell);

  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper(&data, 3, 2);

  REQUIRE_THROWS(entropy_term_shell->compute(dwrapper));
  REQUIRE_THROWS(entropy_term_shell->compute(dwrapper, 0));
  REQUIRE_THROWS(entropy_term_shell->compute_grad(dwrapper, 0));
  REQUIRE_THROWS(entropy_term_shell->update(dwrapper));

  // Now we are going to initialize the shell
  entropy_term_shell->initialize(dwrapper);
  REQUIRE(entropy_term_shell->state() == EntropyTerm::State::Initialized);
  const double shell_term_entropy_value = entropy_term_shell->compute(dwrapper);

  auto entropy_term_initialized = entropy_factory.create(dwrapper, settings);
  REQUIRE(entropy_term_initialized->state() == EntropyTerm::State::Initialized);
  const double initialized_term_entropy_value =
      entropy_term_initialized->compute(dwrapper);

  REQUIRE(shell_term_entropy_value == Approx(initialized_term_entropy_value));
}
