
// Local private PANACEA includes
#include "settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:settings printing with << ", "[unit,panacea]") {
  std::cout << settings::DistributionType::Histogram << std::endl;
  std::cout << settings::DistributionType::Kernel << std::endl;
  std::cout << settings::FileType::TXTRestart << std::endl;
  std::cout << settings::FileType::TXTDescriptors << std::endl;
  std::cout << settings::FileType::TXTKernelDistribution << std::endl;
  std::cout << settings::EntropyOption::Weight << std::endl;
  std::cout << settings::EntropyOption::IncrementRatio << std::endl;
  std::cout << settings::EntropyOption::NumericalGrad << std::endl;
  std::cout << settings::EntropyType::Self << std::endl;
  std::cout << settings::EntropyType::Cross << std::endl;
  std::cout << settings::PANACEAAlgorithm::Strict << std::endl;
  std::cout << settings::PANACEAAlgorithm::Flexible << std::endl;
  std::cout << settings::KernelPrimitive::GaussianLog << std::endl;
  std::cout << settings::KernelPrimitive::Gaussian << std::endl;
  std::cout << settings::KernelPrimitive::Exponential << std::endl;
  std::cout << settings::KernelCount::Single << std::endl;
  std::cout << settings::KernelCount::Fixed << std::endl;
  std::cout << settings::KernelCount::OneToOne << std::endl;
  std::cout << settings::KernelCorrelation::Correlated << std::endl;
  std::cout << settings::KernelCorrelation::Uncorrelated << std::endl;
  std::cout << settings::KernelNormalization::None << std::endl;
  std::cout << settings::KernelNormalization::Variance << std::endl;
  std::cout << settings::KernelCenterCalculation::None << std::endl;
  std::cout << settings::KernelCenterCalculation::Mean << std::endl;
  std::cout << settings::KernelCenterCalculation::Median << std::endl;
}
