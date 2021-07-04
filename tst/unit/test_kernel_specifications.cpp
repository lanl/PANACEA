
// Local private PANACEA includes
#include "kernels/kernel_specifications.hpp"

#include "io/file_io_factory.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:kernel specifications write & read","[unit,panacea]"){

  // Use values that are not default so can be sure when read in it is not
  // simply from construction
  KernelSpecification kern_specs(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Exponential,
      settings::KernelNormalization::None,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible,
      settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No,
      -1
      );

  std::fstream fs;
  fs.open("test_kern_specs.restart", std::fstream::out);
  KernelSpecification::write(settings::FileType::TXTRestart, fs, &kern_specs);
  fs.close();

  KernelSpecification kern_specs2;
  std::fstream fs2;
  fs2.open("test_kern_specs.restart", std::fstream::in);
  KernelSpecification::read(settings::FileType::TXTRestart, fs2, &kern_specs2);
  fs2.close();

  REQUIRE(kern_specs2.get<settings::KernelCorrelation>()       == settings::KernelCorrelation::Uncorrelated);
  REQUIRE(kern_specs2.get<settings::KernelCount>()             == settings::KernelCount::OneToOne);
  REQUIRE(kern_specs2.get<settings::KernelPrimitive>()         == settings::KernelPrimitive::Exponential);
  REQUIRE(kern_specs2.get<settings::KernelNormalization>()     == settings::KernelNormalization::None);
  REQUIRE(kern_specs2.get<settings::KernelMemory>()            == settings::KernelMemory::Share);
  REQUIRE(kern_specs2.get<settings::KernelCenterCalculation>() == settings::KernelCenterCalculation::None);
  REQUIRE(kern_specs2.get<settings::KernelAlgorithm>()         == settings::KernelAlgorithm::Flexible);
}

TEST_CASE("Testing:kernel specifications write & read using fileio","[integration,panacea]"){

  // Use values that are not default so can be sure when read in it is not
  // simply from construction
  KernelSpecification kern_specs(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Exponential,
      settings::KernelNormalization::None,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible,
      settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No,
      -1
      );

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

  restart_file->write(&kern_specs, "test_kern_specs_full.restart");

  KernelSpecification kern_specs2;
  restart_file->read(&kern_specs2, "test_kern_specs_full.restart");

  REQUIRE(kern_specs2.get<settings::KernelCorrelation>()       == settings::KernelCorrelation::Uncorrelated);
  REQUIRE(kern_specs2.get<settings::KernelCount>()             == settings::KernelCount::OneToOne);
  REQUIRE(kern_specs2.get<settings::KernelPrimitive>()         == settings::KernelPrimitive::Exponential);
  REQUIRE(kern_specs2.get<settings::KernelNormalization>()     == settings::KernelNormalization::None);
  REQUIRE(kern_specs2.get<settings::KernelMemory>()            == settings::KernelMemory::Share);
  REQUIRE(kern_specs2.get<settings::KernelCenterCalculation>() == settings::KernelCenterCalculation::None);
  REQUIRE(kern_specs2.get<settings::KernelAlgorithm>()         == settings::KernelAlgorithm::Flexible);
}

TEST_CASE("Testing:kernel specifications equivalence operators == and !=","[unit,panacea]"){
  KernelSpecification kern_specs(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Exponential,
      settings::KernelNormalization::None,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible,
      settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No,
      -1
      );

  WHEN("Testing agains same instance") {
    REQUIRE(!(kern_specs!= kern_specs) );
    REQUIRE(kern_specs== kern_specs);
  }
  WHEN("Testing agains same configuration but difference instance") {
    KernelSpecification kern_specs2(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Exponential,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        -1
        );

    REQUIRE(!(kern_specs2 != kern_specs) );
    REQUIRE(kern_specs2 == kern_specs);
  }
  WHEN("Testing agains different configuration and different instance") {
    KernelSpecification kern_specs2(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Exponential,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        -1
        );

    REQUIRE(kern_specs2 != kern_specs);
    REQUIRE(!(kern_specs2 == kern_specs) );
  }
}
