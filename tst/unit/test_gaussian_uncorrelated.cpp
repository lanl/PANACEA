
// Local private includes
#include "primitives/gaussian_uncorrelated.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "attribute_manipulators/inverter.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "memory.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:creation of gaussian uncorrelated primitive","[unit,panacea]"){
  GaussUncorrelated gauss(0);
}

TEST_CASE("Testing:compute of gaussian uncorrelated primitive","[unit,panacea]"){

  // Assumes we are dealing with two dimensions and two points
  std::vector<std::vector<double>> raw_desc_data{
    {0.0, 3.0},
    {2.0, 5.0}};

  auto dwrapper = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

  Covariance cov(dwrapper.get());

  Reducer reducer;
  ReducedCovariance reduced_cov = reducer.reduce(cov, std::vector<int> {});

  Inverter inverter;
  ReducedInvCovariance reduced_inv_cov = inverter.invert(reduced_cov);

  KernelSpecification specification(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::Variance,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None);

  MemoryManager mem_manager;
  KernelWrapperFactory kfactory;
  auto kwrapper = kfactory.create(dwrapper.get(), specification, mem_manager);

  PrimitiveAttributes attr;
  attr.kernel_wrapper = kwrapper.get();
  attr.reduced_covariance = &reduced_cov;
  attr.reduced_inv_covariance = &reduced_inv_cov;

      

}
