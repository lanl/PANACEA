
// Local private includes
#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distribution_factory.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "helper.hpp"
#include "io/file_io_factory.hpp"
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:panacea non trivial distribution", "[integration,panacea]") {

  SECTION("read & write with fileio with shell plus initialize") {
    test::ArrayDataNonTrivial array_data;

    DescriptorWrapper<double ***> dwrapper(&(array_data.data), array_data.rows,
                                           array_data.cols);

    KernelDistributionSettings kernel_settings;

    kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::Single, settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::Variance, settings::KernelMemory::Own,
        settings::KernelCenterCalculation::Mean,
        settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No, constants::automate));

    DistributionSettings *distribution_settings = &kernel_settings;

    DistributionFactory dist_factory;

    auto dist1 = dist_factory.create(kernel_settings);

    dist1->initialize(dwrapper);

    double dist_val1 = dist1->compute(dwrapper, 0, *distribution_settings);

    io::FileIOFactory file_io_factory;
    auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

    restart_file->write(dist1.get(), "distribution_nontrivial.restart");

    auto dist2 = dist_factory.create(kernel_settings);
    restart_file->read(dist2.get(), "distribution_nontrivial.restart");
    double dist_val2 = dist2->compute(dwrapper, 0, *distribution_settings);

    REQUIRE(dist_val1 == Approx(dist_val2));
  }
}

TEST_CASE("Testing:distributions write & read with fileio",
          "[integration,panacea]") {

  // 3 points 2 dimensions
  std::vector<std::vector<double>> data1{{1.0, 4.0}, {1.0, 4.0}, {1.0, 4.0}};

  size_t num_pts = data1.size();
  size_t dims = 2;

  DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper1(
      &data1, num_pts, dims);

  KernelDistributionSettings kernel_settings;

  kernel_settings.dist_settings = std::move(KernelSpecification(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne, settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None, settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible, settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No, constants::automate));

  DistributionSettings *distribution_settings = &kernel_settings;

  DistributionFactory dist_factory;

  auto dist1 = dist_factory.create(dwrapper1, kernel_settings);

  double value1 = dist1->compute(dwrapper1, 0, *distribution_settings);
  auto dims1 = dist1->getDimensions();
  auto grad1 = dist1->compute_grad(dwrapper1, 0, 0, *distribution_settings);

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);
  restart_file->write(dist1.get(), "distribution_restart.txt");

  // Need to switch kernel settings to own
  kernel_settings.dist_settings.set(settings::KernelMemory::Own);
  auto dist2 = dist_factory.create(kernel_settings);

  restart_file->read(dist2.get(), "distribution_restart.txt");

  double value2 = dist2->compute(dwrapper1, 0, *distribution_settings);
  auto dims2 = dist2->getDimensions();
  auto grad2 = dist2->compute_grad(dwrapper1, 0, 0, *distribution_settings);

  REQUIRE(value1 == Approx(value2));
  REQUIRE(dims1.size() == dims2.size());
  for (int i = 0; i < dims1.size(); ++i) {
    REQUIRE(dims1.at(i) == dims2.at(i));
  }

  REQUIRE(grad1.size() == grad2.size());
  for (int i = 0; i < grad1.size(); ++i) {
    REQUIRE(grad1.at(i) == grad2.at(i));
  }
}
