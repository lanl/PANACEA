
// Local private includes
#include "primitives/gaussian_correlated.hpp"

#include "attributes/covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "attribute_manipulators/inverter.hpp"
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

TEST_CASE("Testing:creation of gaussian correlated primitive","[unit,panacea]"){
  GaussCorrelated gauss(test::Test::key(), 0);
}

TEST_CASE("Testing:compute of gaussian correlated primitive","[unit,panacea]"){

  // Assumes we are dealing with two dimensions and two points
  // The dimensions are linearly dependent so only one will ultimately be used
  std::vector<std::vector<double>> raw_desc_data{
    {0.0, 3.0},
    {2.0, 5.0}};

  auto dwrapper = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,2, 2);

  KernelSpecification specification(
      settings::KernelCorrelation::Correlated,
      settings::KernelCount::OneToOne,
      settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None,
      settings::KernelMemory::Share,
      settings::KernelCenterCalculation::None,
      settings::KernelAlgorithm::Flexible);

  PrimitiveFactory prim_factory;
  
  auto gauss_correlated_prim_grp = prim_factory.create(
      dwrapper.get(),
      specification);

  // There should be two primitives sense it is a one to one ratio and there 
  // are two descriptors
  REQUIRE(gauss_correlated_prim_grp.primitives.size() == 2);
  REQUIRE(gauss_correlated_prim_grp.primitives.at(0)->getId() == 0);
  REQUIRE(gauss_correlated_prim_grp.primitives.at(1)->getId() == 1);
  // Get the density located at the center of both primitives
  REQUIRE(gauss_correlated_prim_grp.primitives.at(0)->compute(dwrapper.get(),0) == Approx(0.282095));
  REQUIRE(gauss_correlated_prim_grp.primitives.at(1)->compute(dwrapper.get(),1) == Approx(0.282095));
  
  REQUIRE(gauss_correlated_prim_grp.primitives.at(0)->compute(dwrapper.get(),1) == Approx(0.103777));
  REQUIRE(gauss_correlated_prim_grp.primitives.at(1)->compute(dwrapper.get(),0) == Approx(0.103777));

  settings::EquationSetting eq_settings = settings::EquationSetting::None; 

  {
    const int descriptor_ind = 0;
    const int kernel_ind = 0;
    WHEN("Taking graident with respect to kernel"){
      const settings::GradSetting setting = settings::GradSetting::WRTKernel;
      const auto grad = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting );
      // Because the descriptor and kernel are on top of one another, they are both at the peak of a gaussian kernel
      // the gradient at this point should be 0
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
    WHEN("Taking graident with respect to descriptor"){
      const settings::GradSetting setting = settings::GradSetting::WRTDescriptor;
      const auto grad = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting );
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
      const auto grad = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting );
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
    WHEN("Taking graident with respect to descriptor"){
      const settings::GradSetting setting = settings::GradSetting::WRTDescriptor;
      const auto grad = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting );
      REQUIRE(grad.at(0) == Approx(0.0));
      REQUIRE(grad.at(1) == Approx(0.0));
    }
  }
}

TEST_CASE("Testing:compute of gaussian correlated primitive single mean kernel","[unit,panacea]"){

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  // Assumes we are dealing with two dimensions and three points
  std::vector<std::vector<double>> raw_desc_data{
    {0.0, 3.0},
    {2.0, 5.0},
    {7.0, 7.0}};

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  auto dwrapper = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,3, 2);

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  KernelSpecification specification(
      settings::KernelCorrelation::Correlated,
      settings::KernelCount::Single,
      settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None,
      settings::KernelMemory::Own,
      settings::KernelCenterCalculation::Mean,
      settings::KernelAlgorithm::Flexible);

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  PrimitiveFactory prim_factory;
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  
  auto gauss_correlated_prim_grp = prim_factory.create(
      dwrapper.get(),
      specification);

  /*
  // There should be two primitives sense it is a one to one ratio and there 
  // are two descriptors
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  REQUIRE(gauss_correlated_prim_grp.primitives.size() == 1);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  REQUIRE(gauss_correlated_prim_grp.primitives.at(0)->getId() == 0);
    std::cout << __FILE__ <<":" << __LINE__ << std::endl;*/
/*
  // Get the density located at the center of the primitives and compare it with the densities
  // at the sides, density at center should be greater
  //
  // Center will be 
  // 
  // col1  col2
  //  3.0   5.0
  //
  std::vector<std::vector<double>> raw_desc_data_sample{
    {3.0, 5.0},
    {2.9, 4.9},
    {3.1, 5.1}};

  auto dwrapper_sample = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data_sample,3, 2);

  const int single_kernel_index = 0;
  const double sample_center = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),0);
  const double sample_side1 = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),1);
  const double sample_side2 = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),2);

  REQUIRE( sample_center > sample_side1);
  REQUIRE( sample_center > sample_side2);
  REQUIRE( sample_side1 == Approx(sample_side2));
*/ 
}

TEST_CASE("Testing:compute of gaussian correlated primitive single median kernel","[unit,panacea]"){

  // Assumes we are dealing with two dimensions and three points
  std::vector<std::vector<double>> raw_desc_data{
    {0.0, 3.0},
    {2.0, 5.0},
    {8.0, 7.0}};

  auto dwrapper = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data,3, 2);

  KernelSpecification specification(
      settings::KernelCorrelation::Correlated,
      settings::KernelCount::Single,
      settings::KernelPrimitive::Gaussian,
      settings::KernelNormalization::None,
      settings::KernelMemory::Own,
      settings::KernelCenterCalculation::Median,
      settings::KernelAlgorithm::Flexible);

  PrimitiveFactory prim_factory;
  
  auto gauss_correlated_prim_grp = prim_factory.create(
      dwrapper.get(),
      specification);

  // There should be two primitives sense it is a one to one ratio and there 
  // are two descriptors
  REQUIRE(gauss_correlated_prim_grp.primitives.size() == 1);
  REQUIRE(gauss_correlated_prim_grp.primitives.at(0)->getId() == 0);

  // Get the density located at the center of the primitives and compare it with the densities
  // at the sides, density at center should be greater
  //
  // Center will be 
  // 
  // col1  col2
  //  2.0   5.0
  //
  std::vector<std::vector<double>> raw_desc_data_sample{
    {2.0, 5.0},
    {1.9, 4.9},
    {2.1, 5.1}};

  auto dwrapper_sample = std::make_unique<
    DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data_sample,3, 2);

  const int single_kernel_index = 0;
  const double sample_center = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),0);
  const double sample_side1 = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),1);
  const double sample_side2 = gauss_correlated_prim_grp.primitives.at(single_kernel_index)->compute(dwrapper_sample.get(),2);

  REQUIRE( sample_center > sample_side1);
  REQUIRE( sample_center > sample_side2);
  REQUIRE( sample_side1 == Approx(sample_side2));
}

TEST_CASE("Testing:compute of gaussian correlated primitive grad","[unit,panacea]"){

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
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible);

    PrimitiveFactory prim_factory;

    auto gauss_correlated_prim_grp = prim_factory.create(
        dwrapper.get(),
        specification);

    std::vector<double> grad_wrt_desc;
    const settings::GradSetting setting1 = settings::GradSetting::WRTDescriptor;
    grad_wrt_desc = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting1 );

    // Compare with numerical result
    std::vector<std::vector<double>> raw_desc_data2{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) + 0.000001, numbers3.at(index)}};
    auto dwrapper2 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,2, 2);

    const double val2 = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute(dwrapper2.get(),descriptor_ind);

    std::vector<std::vector<double>> raw_desc_data3{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) - 0.000001, numbers3.at(index)}};

    auto dwrapper3 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data3,2, 2);

    const double val3 = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute(dwrapper3.get(),descriptor_ind);

    const double grad_val = (val2-val3)/(raw_desc_data2.at(1).at(0) - raw_desc_data3.at(1).at(0));

    REQUIRE( grad_val == Approx(grad_wrt_desc.at(0))); 

    const settings::GradSetting setting2 = settings::GradSetting::WRTKernel;
    const auto grad_wrt_kern = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting2 );
    
    REQUIRE( (grad_wrt_desc.at(0)*-1.0) == Approx(grad_wrt_kern.at(0))); 
  } // End of for loop
}

TEST_CASE("Testing:compute of gaussian correlated primitive grad with normalization","[unit,panacea]"){

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

    std::cout << "Descriptor Wrapper " << std::endl; 
    dwrapper->print();

    KernelSpecification specification(
        settings::KernelCorrelation::Correlated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::Variance,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible);

    PrimitiveFactory prim_factory;

    auto gauss_correlated_prim_grp = prim_factory.create(
        dwrapper.get(),
        specification);

    const settings::GradSetting setting1 = settings::GradSetting::WRTDescriptor;
    std::vector<double> grad_wrt_desc_analy = gauss_correlated_prim_grp.primitives.at(
        kernel_ind)->compute_grad(
          dwrapper.get(), descriptor_ind, eq_settings, setting1 );

    // Compare with numerical result
    std::vector<std::vector<double>> raw_desc_data2{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) + 0.000001, numbers3.at(index)}};
    auto dwrapper2 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data2,2, 2);

    const double val2 = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute(dwrapper2.get(),descriptor_ind);

    std::vector<std::vector<double>> raw_desc_data3{
      {numbers1.at(index)           , numbers2.at(index)},
      {numbers3.at(index) - 0.000001, numbers3.at(index)}};

    auto dwrapper3 = std::make_unique<
      DescriptorWrapper<std::vector<std::vector<double>>*>>(&raw_desc_data3,2, 2);

    const double val3 = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute(dwrapper3.get(),descriptor_ind);

    const std::vector<double> norm_coeffs =  gauss_correlated_prim_grp.normalizer.getNormalizationCoeffs();
    const double grad_val_numer = (val2-val3)/(raw_desc_data2.at(1).at(0) - raw_desc_data3.at(1).at(0));

    std::cout << "Norm Coefficients" << std::endl;
    for( const auto & norm_coef : norm_coeffs ) {
      std::cout << norm_coef << " ";
    }
    std::cout << std::endl;

    REQUIRE( grad_val_numer == Approx(grad_wrt_desc_analy.at(0))); 
    const settings::GradSetting setting2 = settings::GradSetting::WRTKernel;
    const auto grad_wrt_kern = gauss_correlated_prim_grp.primitives.at(kernel_ind)->compute_grad(dwrapper.get(), descriptor_ind, eq_settings, setting2 );
    
    REQUIRE( (grad_wrt_desc_analy.at(0)*-1.0) == Approx(grad_wrt_kern.at(0)));
  } // End of for loop
}
