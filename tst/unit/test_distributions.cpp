
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_factory.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "memory.hpp"
#include "settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

/**
 * Jupyter notebooks depecting the logic behind the update calls can be found
 * in PANACEA/jupyter_notebooks/Covariance
 **/
TEST_CASE("Testing:distributions trivial registration","[unit,panacea]"){

  DistributionFactory dist_factory;
  dist_factory.registerDistribution<
    KernelDistribution,
    settings::DistributionType::Kernel>();

}


TEST_CASE("Testing:distributions","[unit,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data, 3, 2);

  MemoryManager mem_manager;

  KernelDistributionSettings kernel_settings;

  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None
      ));

  DistributionFactory dist_factory;
  dist_factory.registerDistribution<
    KernelDistribution,
    settings::DistributionType::Kernel>();

  auto dist = dist_factory.create(&dwrapper, mem_manager, &kernel_settings);

  REQUIRE(dist->compute(&dwrapper,0) == Approx(dist->compute(&dwrapper,2)));
  REQUIRE(dist->compute(&dwrapper,1) > dist->compute(&dwrapper,0));

  const DistributionSettings * dist_settings = &kernel_settings;

  auto grad0 = dist->compute_grad(&dwrapper,0, *dist_settings);
  auto grad1 = dist->compute_grad(&dwrapper,1, *dist_settings);
  auto grad2 = dist->compute_grad(&dwrapper,2, *dist_settings);

  REQUIRE(grad0.at(0) == Approx(-1.0 * grad2.at(0)));
  // Should be positive if the gradient was taken with respect to Descriptors,
  // magnitude of density would be increasing from left to right near point 0,
  // but the default behavior is to take the gradiant with respect to the kernel
  // which has the opposite sign
  REQUIRE(grad0.at(0) < 0.0);
  // Should be negative if the gradient was taken with respect to Descriptors,
  // magnitude of density would be increasing from left to right near point 0,
  // but the default behavior is to take the gradiant with respect to the kernel
  // which has the opposite sign
  REQUIRE(grad2.at(0) > 0.0);
  REQUIRE(grad1.at(0) == Approx(0.0));
}

