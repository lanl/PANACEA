
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_factory.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
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

/**
 * Jupyter notebooks depecting the logic behind the update calls can be found
 * in PANACEA/jupyter_notebooks/Covariance
 **/
TEST_CASE("Testing:distributions","[unit,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

  KernelDistributionSettings kernel_settings;

  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible
      ));

  DistributionFactory dist_factory;

  auto dist = dist_factory.create(&dwrapper_init, kernel_settings);

  REQUIRE(dist->compute(&dwrapper_init,0,kernel_settings) == Approx(dist->compute(&dwrapper_init,2,kernel_settings)));
  REQUIRE(dist->compute(&dwrapper_init,1,kernel_settings) > dist->compute(&dwrapper_init,0,kernel_settings));

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

TEST_CASE("Testing:distributions pre_factor","[unit,panacea]"){

  // 3 points 2 dimensions
  std::vector<std::vector<double>> data1{
    {1.0, 4.0},
    {1.0, 4.0},
    {1.0, 4.0}};

  size_t num_pts = data1.size();
  size_t dims = 2;

  std::vector<std::vector<double>> data2{{1.0, 4.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper1(&data1, num_pts, dims);
  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper2(&data2, 1, dims);

  KernelDistributionSettings kernel_settings;

  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible
      ));

  DistributionFactory dist_factory;

  std::cout << "Creating dist1" << std::endl;
  auto dist1 = dist_factory.create(&dwrapper1, kernel_settings);
  std::cout << "Creating dist2" << std::endl;
  auto dist2 = dist_factory.create(&dwrapper2, kernel_settings);

  std::cout << "Computing distribution 1" << std::endl;
  std::cout << dist1->compute(&dwrapper1,0,kernel_settings) << std::endl;
  std::cout << "Computing distribution 2" << std::endl;
  std::cout << dist2->compute(&dwrapper2,0,kernel_settings) << std::endl;
  // Check that the values are equivalent despite one being made up of three kernels and
  // the other composed of a single kernel
  REQUIRE(dist1->compute(&dwrapper1,0,kernel_settings) == Approx(dist2->compute(&dwrapper2,0,kernel_settings)));
}

TEST_CASE("Testing:distributions write & read with fileio","[integration,panacea]"){

  // 3 points 2 dimensions
  std::vector<std::vector<double>> data1{
    {1.0, 4.0},
    {1.0, 4.0},
    {1.0, 4.0}};

  size_t num_pts = data1.size();
  size_t dims = 2;

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper1(&data1, num_pts, dims);

  KernelDistributionSettings kernel_settings;

  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible
      ));


  DistributionSettings * distribution_settings = &kernel_settings;

  DistributionFactory dist_factory;

  auto dist1 = dist_factory.create(&dwrapper1, kernel_settings);

  double value1 = dist1->compute(&dwrapper1,0, *distribution_settings);
  auto dims1 = dist1->getDimensions();
  auto grad1 = dist1->compute_grad(&dwrapper1,0,0,*distribution_settings);

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);
  restart_file->write(dist1.get(),"distribution_restart.txt");

  std::cout << "Creating Distribution shell" << std::endl;
  // Need to switch kernel settings to own
  kernel_settings.dist_settings.set(settings::KernelMemory::Own);
  auto dist2 = dist_factory.create(kernel_settings);
  std::cout << "Beginning Read" << std::endl;

  restart_file->read(dist2.get(),"distribution_restart.txt");

  double value2 = dist2->compute(&dwrapper1,0, *distribution_settings);
  auto dims2 = dist2->getDimensions();
  auto grad2 = dist2->compute_grad(&dwrapper1,0,0,*distribution_settings);

  REQUIRE(value1 == Approx(value2));
  REQUIRE(dims1.size() == dims2.size());
  for(int i = 0; i < dims1.size(); ++i){
    REQUIRE(dims1.at(i) == dims2.at(i));
  }

  REQUIRE(grad1.size() == grad2.size());
  for(int i = 0; i < grad1.size(); ++i){
    REQUIRE(grad1.at(i) == grad2.at(i));
  }

}



