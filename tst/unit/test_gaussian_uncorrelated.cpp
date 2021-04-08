
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
      settings::KernelNormalization::None,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None);

  MemoryManager mem_manager;

  PrimitiveFactory prim_factory;
  
  prim_factory.registerPrimitive<
    GaussUncorrelated,
    settings::KernelPrimitive::Gaussian,
    settings::KernelCorrelation::Uncorrelated>();

  auto gauss_uncorrelated_prim_grp = prim_factory.create(
      dwrapper.get(),
      mem_manager,
      specification);

  // There should be two primitives sense it is a one to one ratio and there 
  // are two descriptors
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.size() == 2);
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->getId() == 0);
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->getId() == 1);
  // Get the density located at the center of both primitives
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->compute(dwrapper.get(),0) == Approx(0.282095));
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->compute(dwrapper.get(),1) == Approx(0.282095));
  
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->compute(dwrapper.get(),1) == Approx(0.103777));
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->compute(dwrapper.get(),0) == Approx(0.103777));

  std::vector<settings::EquationSetting> eq_settings = {settings::EquationSetting::None}; 
  // Calculate the density at this location
  const int descriptor_ind = 0;
  // Calculate the gradient with respect to this point
  auto grad = gauss_uncorrelated_prim_grp.primitives.at(0)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, settings::GradSetting::WRTKernel );

  for ( auto val : grad) {
    std::cout << "val " << val << std::endl;
  }
//  std::cout << "density prim 2" << std::endl;
//  std::cout << gauss_uncorrelated_prims.at(1)->compute(dwrapper.get(),1) << std::endl;
}
