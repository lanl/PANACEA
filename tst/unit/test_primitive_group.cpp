
// Local private PANACEA includes
#include "primitives/primitive_group.hpp"

#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "io/file_io_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "primitives/primitive_factory.hpp"
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:primitive group write & read", "[unit,panacea]") {
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper_init(&data, 3,
                                                                      2);

  KernelSpecification kernel_settings(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None, settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No, constants::automate);

  PrimitiveFactory prim_factory;
  auto prim_grp = prim_factory.createGroup(dwrapper_init, kernel_settings,
                                           "test_prim_group");

  std::fstream fs;
  fs.open("test_prim_group.restart", std::fstream::out);
  PrimitiveGroup::write(settings::FileType::TXTRestart, fs, &prim_grp);
  fs.close();

  PrimitiveGroup prim_grp2;
  std::fstream fs2;
  fs2.open("test_prim_group.restart", std::fstream::in);
  PrimitiveGroup::read(settings::FileType::TXTRestart, fs2, &prim_grp2);
  fs2.close();

  REQUIRE(prim_grp2.name == "test_prim_group");
}
