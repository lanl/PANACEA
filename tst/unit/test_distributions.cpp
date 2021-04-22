
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

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

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

  auto dist = dist_factory.create(&dwrapper_init, mem_manager, &kernel_settings);

  REQUIRE(dist->compute(&dwrapper_init,0) == Approx(dist->compute(&dwrapper_init,2)));
  REQUIRE(dist->compute(&dwrapper_init,1) > dist->compute(&dwrapper_init,0));

  const DistributionSettings * dist_settings = &kernel_settings;

  std::vector<std::vector<double>> data_sample{
    {1.25, 4.5}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_sample(&data_sample, 3, 2);

  auto grad_setting = settings::GradSetting::WRTKernel;
  auto grad0_wrt_kern = dist->compute_grad(&dwrapper_sample,0,0, *dist_settings, grad_setting);
  auto grad1_wrt_kern = dist->compute_grad(&dwrapper_sample,0,1, *dist_settings, grad_setting);
  auto grad2_wrt_kern = dist->compute_grad(&dwrapper_sample,0,2, *dist_settings, grad_setting);
  std::cout << "grad0 WRT kernel Assuming we are sampling from a location removed from kernel " << grad0_wrt_kern.at(0) << std::endl;
  std::cout << "grad1 WRT kernel Assuming we are sampling from a location removed from kernel " << grad1_wrt_kern.at(0) << std::endl;
  std::cout << "grad2 WRT kernel Assuming we are sampling from a location removed from kernel " << grad2_wrt_kern.at(0) << std::endl;

  // Add all the gradiants together
  std::transform(grad0_wrt_kern.begin(), grad0_wrt_kern.end(), grad1_wrt_kern.begin(), grad0_wrt_kern.begin(), std::plus<double>());
  std::transform(grad0_wrt_kern.begin(), grad0_wrt_kern.end(), grad2_wrt_kern.begin(), grad0_wrt_kern.begin(), std::plus<double>());

  std::cout << "Sum of gradiants WRT kernel Assuming we are sampling from a location removed from kernel " << grad0_wrt_kern.at(0) << std::endl;
  
  grad_setting = settings::GradSetting::WRTDescriptor;
  auto grad0_wrt_desc = dist->compute_grad(&dwrapper_sample,0,0, *dist_settings, grad_setting);

  std::cout << "grad0 WRT descriptor assuming we are sampling from a location removed from kernel " << grad0_wrt_desc.at(0) << std::endl;
  REQUIRE(grad0_wrt_desc.at(0) == Approx(-1.0 * grad0_wrt_kern.at(0)));

  grad_setting = settings::GradSetting::WRTBoth;
  
  auto grad0 = dist->compute_grad(&dwrapper_init,0,0, *dist_settings, grad_setting);
  auto grad1 = dist->compute_grad(&dwrapper_init,1,1, *dist_settings, grad_setting);
  auto grad2 = dist->compute_grad(&dwrapper_init,2,2, *dist_settings, grad_setting);

  std::cout << "grad0 WRT both if probing at location of first descriptor/kernel " << grad0.at(0) << std::endl;
  std::cout << "grad1 WRT both if probing at location of second descriptor/kernel  " << grad1.at(0) << std::endl;
  std::cout << "grad2 WRT both if probing at location of third descriptor/kernel  " << grad2.at(0) << std::endl;
  REQUIRE(grad0.at(0) > 0.0);
  REQUIRE(grad2.at(0) < 0.0);

  REQUIRE(grad0.at(0) == Approx(-1.0*grad2.at(0)));
  REQUIRE(grad1.at(0) == Approx(0.0));
  
}

