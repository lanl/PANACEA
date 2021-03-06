
// Local private PANACEA includes
#include "primitives/primitive_factory.hpp"

#include "constants.hpp"

// Public PANACEA includes
#include "descriptors/descriptor_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:primitive_factory create primitive group",
          "[integration,panacea]") {
  // Assumes we are dealing with two dimensions and two points
  // The dimensions are linearly dependent so only one will ultimately be used
  std::vector<std::vector<double>> raw_desc_data{{0.0, 3.0}, {2.0, 5.0}};

  auto dwrapper =
      std::make_unique<DescriptorWrapper<std::vector<std::vector<double>> *>>(
          &raw_desc_data, 2, 2);

  WHEN("Looking at Correlated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(*dwrapper, specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
  WHEN("Looking at Uncorrelated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(*dwrapper, specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
  WHEN("Looking at Correlated GaussianLog") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::GaussianLog,
        settings::KernelNormalization::None, settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(*dwrapper, specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
}

TEST_CASE("Testing:primitive_factory create primitive group shell",
          "[integration,panacea]") {
  WHEN("Looking at Correlated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 0);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance == nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance == nullptr);
  }
  WHEN("Looking at Uncorrelated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 0);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance == nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance == nullptr);
  }
  WHEN("Looking at Correlated GaussianLog") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::GaussianLog,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    auto prim_grp = prim_factory.createGroup(specification);

    // Check that the specifications are equivalent
    REQUIRE(prim_grp.getSpecification() == specification);
    REQUIRE(prim_grp.primitives.size() == 0);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance == nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance == nullptr);
  }
}

TEST_CASE("Testing:primitive_factory initialize", "[intigration,panacea]") {
  // Assumes we are dealing with two dimensions and two points
  // The dimensions are linearly dependent so only one will ultimately be used
  std::vector<std::vector<double>> raw_desc_data{{0.0, 3.0}, {2.0, 5.0}};

  auto dwrapper =
      std::make_unique<DescriptorWrapper<std::vector<std::vector<double>> *>>(
          &raw_desc_data, 2, 2);

  WHEN("Looking at Correlated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    // Create shell
    auto prim_grp = prim_factory.createGroup(specification);

    // Fill in shell
    prim_grp.initialize(*dwrapper);

    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
  WHEN("Looking at Uncorrelated Gaussian") {
    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    // Create shell
    auto prim_grp = prim_factory.createGroup(specification);

    // Fill in shell
    prim_grp.initialize(*dwrapper);

    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
  WHEN("Looking at Correlated GaussianLog") {
    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne, settings::KernelPrimitive::GaussianLog,
        settings::KernelNormalization::None, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate);

    PrimitiveFactory prim_factory;

    // Create shell
    auto prim_grp = prim_factory.createGroup(specification);

    // Fill in shell
    prim_grp.initialize(*dwrapper);

    REQUIRE(prim_grp.primitives.size() == 2);
    REQUIRE(prim_grp.kernel_wrapper != nullptr);
    REQUIRE(prim_grp.covariance != nullptr);
    REQUIRE(prim_grp.reduced_covariance != nullptr);
    REQUIRE(prim_grp.reduced_inv_covariance != nullptr);
    // Cycle through primitives and ensure that they are all of the same type as
    // the spec
    for (const auto &prim : prim_grp.primitives) {
      REQUIRE(prim->type() == specification.get<settings::KernelPrimitive>());
      REQUIRE(prim->correlation() ==
              specification.get<settings::KernelCorrelation>());
    }
  }
}
