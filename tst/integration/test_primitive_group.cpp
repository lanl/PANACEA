
// Local private PANACEA includes
#include "primitives/primitive_group.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "io/file_io_factory.hpp"
#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"
#include "primitives/primitive_factory.hpp"

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:primitive group write & read using fileio","[integration,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

  KernelSpecification kernel_settings(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None,
        settings::KernelAlgorithm::Flexible,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::No,
        -1
      );


  PrimitiveFactory prim_factory;
  auto prim_grp = prim_factory.createGroup(
      dwrapper_init,
      kernel_settings,
      "test_prim_group");

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);
  restart_file->write(&prim_grp, "test_prim_group_full.restart");

  PrimitiveGroup prim_grp2;
  restart_file->read(&prim_grp2, "test_prim_group_full.restart");

  REQUIRE(prim_grp2.name == "test_prim_group");


  REQUIRE(prim_grp2.kernel_wrapper.get() != nullptr);
  REQUIRE(prim_grp2.kernel_wrapper->rows() == 3);
  REQUIRE(prim_grp2.kernel_wrapper->cols() == 2);

  REQUIRE(prim_grp2.kernel_wrapper->at(0,0) == Approx(1.0));
  REQUIRE(prim_grp2.kernel_wrapper->at(0,1) == Approx(4.0));
  REQUIRE(prim_grp2.kernel_wrapper->at(1,0) == Approx(2.0));
  REQUIRE(prim_grp2.kernel_wrapper->at(1,1) == Approx(5.0));
  REQUIRE(prim_grp2.kernel_wrapper->at(2,0) == Approx(3.0));
  REQUIRE(prim_grp2.kernel_wrapper->at(2,1) == Approx(6.0));

  REQUIRE(prim_grp2.covariance.get() != nullptr);
  REQUIRE(prim_grp.covariance->getMean(0) == Approx(prim_grp2.covariance->getMean(0)));
  REQUIRE(prim_grp.covariance->getMean(1) == Approx(prim_grp2.covariance->getMean(1)));
  REQUIRE(prim_grp.covariance->getCummulativeDescPoints() ==
      prim_grp2.covariance->getCummulativeDescPoints());
  REQUIRE(prim_grp.covariance->getNormalizationState() ==
      prim_grp2.covariance->getNormalizationState());
  REQUIRE(prim_grp.covariance->getDeterminant() == Approx(prim_grp2.covariance->getDeterminant()));

  REQUIRE(prim_grp2.reduced_covariance.get() != nullptr);
  REQUIRE(prim_grp.reduced_covariance->operator()(0,0) == Approx(prim_grp2.reduced_covariance->operator()(0,0)));
  REQUIRE(prim_grp.reduced_covariance->getNormalizationState() == prim_grp2.reduced_covariance->getNormalizationState());
  REQUIRE(prim_grp.reduced_covariance->getDeterminant() == Approx(prim_grp2.reduced_covariance->getDeterminant()));

  REQUIRE(prim_grp2.reduced_inv_covariance.get() != nullptr);
  REQUIRE(prim_grp2.reduced_inv_covariance->getNumberDimensions() == prim_grp.reduced_inv_covariance->getNumberDimensions());
  REQUIRE(prim_grp2.reduced_inv_covariance->operator()(0,0) == Approx(prim_grp2.reduced_inv_covariance->operator()(0,0)));

  REQUIRE(prim_grp2.primitives.size() == 3);
  REQUIRE(prim_grp.primitives.at(0)->compute(dwrapper_init,0) == Approx(prim_grp2.primitives.at(0)->compute(dwrapper_init,0)));
  REQUIRE(prim_grp.primitives.at(0)->compute(dwrapper_init,1) == Approx(prim_grp2.primitives.at(0)->compute(dwrapper_init,1)));
  REQUIRE(prim_grp.primitives.at(0)->compute(dwrapper_init,2) == Approx(prim_grp2.primitives.at(0)->compute(dwrapper_init,2)));

}
