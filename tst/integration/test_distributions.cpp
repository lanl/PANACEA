
// Local private includes
#include "constants.hpp"
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

TEST_CASE("Testing:distributions write & read with fileio","[integration,panacea]"){

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data1{
    {1.0, 4.0},
    {1.0, 4.0},
    {1.0, 4.0}};

  size_t num_pts = data1.size();
  size_t dims = 2;
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper1(&data1, num_pts, dims);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  KernelDistributionSettings kernel_settings;
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  kernel_settings.dist_settings = std::move(KernelSpecification(
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
      ));

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  DistributionSettings * distribution_settings = &kernel_settings;

  DistributionFactory dist_factory;

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  auto dist1 = dist_factory.create(dwrapper1, kernel_settings);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  double value1 = dist1->compute(dwrapper1,0, *distribution_settings);
  auto dims1 = dist1->getDimensions();
  auto grad1 = dist1->compute_grad(dwrapper1,0,0,*distribution_settings);

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  restart_file->write(dist1.get(),"distribution_restart.txt");

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  std::cout << "Creating Distribution shell" << std::endl;
  // Need to switch kernel settings to own
  kernel_settings.dist_settings.set(settings::KernelMemory::Own);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  auto dist2 = dist_factory.create(kernel_settings);
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  std::cout << "Beginning Read" << std::endl;

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
  restart_file->read(dist2.get(),"distribution_restart.txt");
  std::cout << __FILE__ <<":" << __LINE__ << std::endl;

  double value2 = dist2->compute(dwrapper1,0, *distribution_settings);
  auto dims2 = dist2->getDimensions();
  auto grad2 = dist2->compute_grad(dwrapper1,0,0,*distribution_settings);

  std::cout << __FILE__ <<":" << __LINE__ << std::endl;
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



