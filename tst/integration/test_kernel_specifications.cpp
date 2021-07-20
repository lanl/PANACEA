
// Local private PANACEA includes
#include "kernels/kernel_specifications.hpp"

#include "io/file_io_factory.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:kernel specifications write & read using fileio",
          "[integration,panacea]") {

  // Use values that are not default so can be sure when read in it is not
  // simply from construction
  KernelSpecification kern_specs(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne, settings::KernelPrimitive::Exponential,
      settings::KernelNormalization::None, settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No, -1);

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

  restart_file->write(&kern_specs, "test_kern_specs_full.restart");

  KernelSpecification kern_specs2;
  restart_file->read(&kern_specs2, "test_kern_specs_full.restart");

  REQUIRE(kern_specs2.get<settings::KernelCorrelation>() ==
          settings::KernelCorrelation::Uncorrelated);
  REQUIRE(kern_specs2.get<settings::KernelCount>() ==
          settings::KernelCount::OneToOne);
  REQUIRE(kern_specs2.get<settings::KernelPrimitive>() ==
          settings::KernelPrimitive::Exponential);
  REQUIRE(kern_specs2.get<settings::KernelNormalization>() ==
          settings::KernelNormalization::None);
  REQUIRE(kern_specs2.get<settings::KernelMemory>() ==
          settings::KernelMemory::Share);
  REQUIRE(kern_specs2.get<settings::KernelCenterCalculation>() ==
          settings::KernelCenterCalculation::None);
  REQUIRE(kern_specs2.get<settings::KernelAlgorithm>() ==
          settings::KernelAlgorithm::Flexible);
}
