
// Local private includes
#include "primitives/gaussian_uncorrelated.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "attribute_manipulators/inverter.hpp"
#include "constants.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "helper.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "kernels/kernel_specifications.hpp"
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
  REQUIRE(gauss.getPreFactor() == Approx(0.0));
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
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible,
      settings::RandomizeDimensions::No,
      settings::RandomizeNumberDimensions::No,
      constants::automate
      );

  PrimitiveFactory prim_factory;

  auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
      *dwrapper,
      specification);

  // There should be two primitives sense it is a one to one ratio and there
  // are two descriptors
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.size() == 2);
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->getId() == 0);
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->getId() == 1);
  // Get the density located at the center of both primitives
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->compute(*dwrapper,0) == Approx(0.2820947918));
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->compute(*dwrapper,1) == Approx(0.2820947918));

  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(0)->compute(*dwrapper,1) == Approx(0.1037768744));
  REQUIRE(gauss_uncorrelated_prim_grp.primitives.at(1)->compute(*dwrapper,0) == Approx(0.1037768744));

  settings::EquationSetting eq_settings = settings::EquationSetting::None;

  {
    const int descriptor_ind = 0;
    const int kernel_ind = 0;
    WHEN("Taking graident with respect to kernel"){
      const settings::GradSetting setting = settings::GradSetting::WRTKernel;
      const auto grad = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting );
      // Because the descriptor and kernel are on top of one another, they are both at the peak of a gaussian kernel
      // the gradient at this point should be 0
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
    WHEN("Taking graident with respect to descriptor"){
      const settings::GradSetting setting = settings::GradSetting::WRTDescriptor;
      const auto grad = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting );
      // Because the descriptor and kernel are on top of one another, they are both at the peak of a gaussian kernel
      // the gradient at this point should be 0
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
  }

  {
    // Because the descriptor and kernel are on top of one another, they are both at the peak of a gaussian kernel
    // the gradient at this point should be 0
    const int descriptor_ind = 1;
    const int kernel_ind = 1;
    WHEN("Taking graident with respect to kernel"){
      const settings::GradSetting setting = settings::GradSetting::WRTKernel;
      const auto grad = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting );
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
    WHEN("Taking graident with respect to descriptor"){
      const settings::GradSetting setting = settings::GradSetting::WRTDescriptor;
      const auto grad = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting );
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
  }

}

TEST_CASE("Testing:compute of gaussian uncorrelated primitive grad","[unit,panacea]"){

  // Set of numbers to test the gradient at
  std::vector<double> numbers1 = { 1.0, 2.0, 5.5, -3.0, 12.0 };
  std::vector<double> numbers2 = { 1.5, -2.0, 0.0, -6.0, 0.0 };
  std::vector<double> numbers3 = { 0.0, 0.0, 0.0, 0.0, 12.0 };
  std::vector<double> numbers4 = { -1.0, -1.0, -1.0, 5.0, 5.0 };

  const int descriptor_ind = 1;
  const int kernel_ind = 0;

  settings::EquationSetting eq_settings = settings::EquationSetting::None;

  for( int index = 0; index < numbers1.size(); ++index ) {
    // Assumes we are dealing with two dimensions and two points
    // The dimensions are linearly dependent so only one will ultimately be used
    std::vector<std::vector<double>> raw_desc_data{
      {numbers1.at(index), numbers2.at(index)},
      {numbers3.at(index), numbers3.at(index)}};

    auto dwrapper = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        constants::automate
        );

    PrimitiveFactory prim_factory;

    auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
        *dwrapper,
        specification);

    std::vector<double> grad_wrt_desc;
    const settings::GradSetting setting1 = settings::GradSetting::WRTDescriptor;
    grad_wrt_desc = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting1 );

    // Compare with numerical result
    std::vector<std::vector<double>> raw_desc_data2{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) + 0.000001, numbers3.at(index)}};
    auto dwrapper2 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,2, 2);

    const double val2 = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute(*dwrapper2,descriptor_ind);

    std::vector<std::vector<double>> raw_desc_data3{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) - 0.000001, numbers3.at(index)}};

    auto dwrapper3 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data3,2, 2);

    const double val3 = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute(*dwrapper3,descriptor_ind);

    const double grad_val_numer = (val2-val3)/(raw_desc_data2.at(1).at(0) - raw_desc_data3.at(1).at(0));

    REQUIRE( grad_val_numer == Approx(grad_wrt_desc.at(0)));

    const settings::GradSetting setting2 = settings::GradSetting::WRTKernel;
    const auto grad_wrt_kern = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting2 );

    REQUIRE( (grad_wrt_desc.at(0)*-1.0) == Approx(grad_wrt_kern.at(0)));
  } // End of for loop
}

TEST_CASE("Testing:compute of gaussian uncorrelated primitive grad with normalization","[unit,panacea]"){

  // Set of numbers to test the gradient at
  std::vector<double> numbers1 = {  1.0,  2.0,  5.5, -3.0, 12.0 };
  std::vector<double> numbers2 = {  1.5, -2.0,  0.0, -6.0,  0.0 };
  std::vector<double> numbers3 = {  0.0,  0.0,  0.0,  0.0, 12.0 };
  std::vector<double> numbers4 = { -1.0, -1.0, -1.0,  5.0,  5.0 };

  const int descriptor_ind = 1;
  const int kernel_ind = 0;

  settings::EquationSetting eq_settings = settings::EquationSetting::None;
  for( int index = 0; index < numbers1.size(); ++index ) {
    // Assumes we are dealing with two dimensions and two points
    // The dimensions are linearly dependent so only one will ultimately be used
    std::vector<std::vector<double>> raw_desc_data{
      {numbers1.at(index), numbers2.at(index)},
      {numbers3.at(index), numbers4.at(index)}};

    auto dwrapper = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::Variance,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        constants::automate
        );

    PrimitiveFactory prim_factory;

    auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
        *dwrapper,
        specification);

    std::vector<double> grad_wrt_desc;
    const settings::GradSetting setting1 = settings::GradSetting::WRTDescriptor;
    grad_wrt_desc = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting1 );

    // Compare with numerical resul
    std::vector<std::vector<double>> raw_desc_data2{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) + 0.000001, numbers4.at(index)}};
    auto dwrapper2 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,2, 2);

    const double val2 = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute(*dwrapper2,descriptor_ind);

    std::vector<std::vector<double>> raw_desc_data3{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) - 0.000001, numbers4.at(index)}};

    auto dwrapper3 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data3,2, 2);

    const double val3 = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute(*dwrapper3,descriptor_ind);

    const std::vector<double> norm_coeffs =  gauss_uncorrelated_prim_grp.normalizer->getNormalizationCoeffs();
    const double grad_val_numer = (val2-val3)/(raw_desc_data2.at(1).at(0) - raw_desc_data3.at(1).at(0));// * norm_coeffs.at(0);

    std::cout << "Grad val diff top " << (val2-val3) << std::endl;
    std::cout << "Grad val diff bottom " << (raw_desc_data2.at(1).at(0) - raw_desc_data3.at(1).at(0)) << std::endl;
    REQUIRE( grad_val_numer == Approx(grad_wrt_desc.at(0)));
    const settings::GradSetting setting2 = settings::GradSetting::WRTKernel;
    const auto grad_wrt_kern = gauss_uncorrelated_prim_grp.primitives.at(kernel_ind)->compute_grad(*dwrapper, descriptor_ind, eq_settings, setting2 );

    REQUIRE( (grad_wrt_desc.at(0)*-1.0) == Approx(grad_wrt_kern.at(0)));
  } // End of for loop
}

TEST_CASE("Testing:gaussian uncorrelated update","[integration,panacea]"){

/*
 * This test is not valid if we are using the full covariance matrix to calculate the determinant
  GIVEN("raw data consisting of two dimensions and two points."){
    std::vector<std::vector<double>> raw_desc_data{
      {0.0, -1.0},
      {2.0, 5.0}};

    // Variance of first set of points
    //
    // Dim 1 Mean (0.0 + 2.0) / 2.0 = 1.0
    // Dim 2 mean (-1.0 + 5.0) / 2.0 = 2.0
    //
    // Sample variance
    // (1 + 1)/1 = 2.0
    // (9.0 + 9.0)/1 = 18.0
    //
    // Determinant 2.0 * 18.0 = 36.0

    auto dwrapper = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::Single,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Own,
        settings::KernelCenterCalculation::Mean,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        constants::automate
        );

    PrimitiveFactory prim_factory;

    auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
        *dwrapper,
        specification);

    // There should be only a single primitive because the count is "Single"
    const int primitive_index = 0;
    Primitive & primitive = *gauss_uncorrelated_prim_grp.primitives.at(primitive_index);

    const double initial_pre_factor = primitive.getPreFactor();
    // The pre factor should be positive
    REQUIRE(initial_pre_factor > 0.0);

    // If we add another kernel point without expanding the dimensions we should see a
    // decrease in the pre factor simply because our covarince matrix will change,

    THEN("add a third point."){
      std::vector<std::vector<double>> raw_desc_data2{
        {4.0, 2.0}};

      // After adding the covariance matrix will be calculated using these points
      //
      //   0.0, -1.0
      //   2.0,  5.0
      //   4.0,  2.0

      // Variance of first set of points
      //
      // Dim 1 Mean (0.0 + 2.0 + 4.0) / 3.0 = 2.0
      // Dim 2 mean (-1.0 + 5.0 + 2.0) / 3.0 = 2.0
      //
      // Sample variance
      // (4.0 + 0.0 + 4.0)/2 = 4.0
      // (9.0 + 9.0 + 0.0)/2 = 9.0

      // Determinant 4.0 * 9.0 = 36.0

      auto dwrapper2 = std::make_unique<
        DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,1, 2);

      // now we are going to update the primitive with the new set of points however, because
      // the number of dimensions did not change the variance fo the two dimensions
      // balances out the determinant the pre factor should remain the same
      gauss_uncorrelated_prim_grp.update(*dwrapper2);

      const double pre_factor_after_adding_point = primitive.getPreFactor();
      REQUIRE(initial_pre_factor == Approx(pre_factor_after_adding_point));
    }
  }*/

  GIVEN("raw data consisting of one dimension and three points."){
    std::vector<std::vector<double>> raw_desc_data{
      {0.0},
      {-3.0},
      {3.0}};

    auto dwrapper = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,3, 1);

    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::Single,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Own,
        settings::KernelCenterCalculation::Mean,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        constants::automate
        );

    PrimitiveFactory prim_factory;

    auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
        *dwrapper,
        specification);

    // There should be only a single primitive because the count is "Single"
    const int primitive_index = 0;
    Primitive & primitive = *gauss_uncorrelated_prim_grp.primitives.at(primitive_index);

    const double initial_pre_factor = primitive.getPreFactor();
    // The pre factor should be positive
    REQUIRE(initial_pre_factor > 0.0);

    THEN("a second set of data with three points and two dimensions."){
      std::vector<std::vector<double>> raw_desc_data2{
      {0.0, 1.0},
      {-3.0, 0.0},
      {3.0, -1.0}};

      auto dwrapper2 = std::make_unique<
        DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,3, 2);

      // Should throw dimensions of descriptor wrapper should be consistent
      REQUIRE_THROWS(gauss_uncorrelated_prim_grp.update(*dwrapper2));
    }
  }

  GIVEN("raw data consisting of two dimension and three points."){
    std::vector<std::vector<double>> raw_desc_data{
      {0.0, 1.0},
      {-3.0, 0.0},
      {3.0, -1.0}};

    auto dwrapper = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,3, 2);

    PrimitiveFactory prim_factory;

    KernelSpecification specification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::Single,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Own,
        settings::KernelCenterCalculation::Mean,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        1 // number of dimensions to use
        );

    auto gauss_uncorrelated_prim_grp = prim_factory.createGroup(
        *dwrapper,
        specification);

    KernelSpecification specification2(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::Single,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Own,
        settings::KernelCenterCalculation::Mean,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        2 // number of dimensions to use
        );

    auto gauss_uncorrelated_prim_grp2 = prim_factory.createGroup(
        *dwrapper,
        specification2);

    // There should be only a single primitive because the count is "Single"
    const int primitive_index = 0;
    Primitive & primitive1 = *gauss_uncorrelated_prim_grp.primitives.at(primitive_index);
    const double pre_factor1 = primitive1.getPreFactor();
    Primitive & primitive2 = *gauss_uncorrelated_prim_grp2.primitives.at(primitive_index);
    const double pre_factor2 = primitive2.getPreFactor();
    // The pre factor 1 should be greater than prefactor 2 because it has fewer dimensions
    // and the determinant of the covariance matrix will be the same
    REQUIRE(pre_factor1 > 0.0);
    REQUIRE(pre_factor2 > 0.0);
    REQUIRE(pre_factor1 > pre_factor2);
  }
}
