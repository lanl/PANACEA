
// Local private includes
#include "primitives/gaussian_uncorrelated.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "attribute_manipulators/inverter.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "helper.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "memory.hpp"
#include "primitives/primitive_factory.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:creation of gaussian uncorrelated primitive","[unit,panacea]"){
  GaussUncorrelated gauss(test::Test::key(), 0);
}

TEST_CASE("Testing:compute of gaussian uncorrelated primitive","[unit,panacea]"){

  // Assumes we are dealing with two dimensions and two points
  // The dimensions are linearly dependent so only one will ultimately be used
  std::vector<std::vector<double>> raw_desc_data{
    {0.0, 3.0},
    {2.0, 5.0}};

  auto dwrapper = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

  KernelSpecification specification(
      settings::KernelCorrelation::Uncorrelated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::Variance,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None);

  MemoryManager mem_manager;

  PrimitiveFactory prim_factory;
  
  prim_factory.registerPrimitive<
    GaussUncorrelated,
    settings::KernelPrimitive::Gaussian,
    settings::KernelCorrelation::Uncorrelated>();

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  auto gauss_uncorrelated_prims = prim_factory.create(
      dwrapper.get(),
      mem_manager,
      specification);

    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  // There should be two primitives sense it is a one to one ratio and there 
  // are two descriptors
  REQUIRE(gauss_uncorrelated_prims.size() == 2);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  REQUIRE(gauss_uncorrelated_prims.at(0)->getId() == 0);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  REQUIRE(gauss_uncorrelated_prims.at(1)->getId() == 1);
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
  // Get the density located at the center of both primitives
  std::cout << "density prim 1" << std::endl;
  std::cout << gauss_uncorrelated_prims.at(0)->compute(dwrapper.get(),0) << std::endl;
//  std::cout << "density prim 2" << std::endl;
//  std::cout << gauss_uncorrelated_prims.at(1)->compute(dwrapper.get(),1) << std::endl;
}
