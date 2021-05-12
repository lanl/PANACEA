



// Public PANACEA includes
#include <panacea/base_descriptor_wrapper.hpp>
#include <panacea/entropy_term.hpp>
#include <panacea/panacea.hpp>
#include <panacea/settings.hpp>

// Standard includes
#include <algorithm>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

/**
 * Create a 1 dimensional expontial distribution
 **/

std::vector<std::vector<double>> createDistribution(
    const int samples,
    const int dims) {

  std::default_random_engine generator;
  // Create a different distribution for each dimension
  std::vector<std::exponential_distribution<double>> distributions;
  for ( int dim = 0; dim < dims; ++dim ) {
    distributions.emplace_back(1.0 + static_cast<double>(dim+1)/10.0);
  }

  std::vector<std::vector<double>> data;
  for(int sample=0; sample<samples; ++sample) {
    std::vector<double> point;
    point.reserve(dims);
    for( int dim = 0; dim < dims; ++dim ) {
      point.push_back(distributions.at(dim)(generator));
    }
    data.push_back(point);
  }
  return data;
}

using namespace panacea;
using namespace panacea::settings;

int main()
{

  const int samples = 10000;
  const int dims = 2;
  auto data = createDistribution(samples,dims);

  PANACEA panacea_pi;

  std::unique_ptr<BaseDescriptorWrapper> dwrapper = panacea_pi.wrap(data, samples, dims);
  auto desc_io = panacea_pi.create(settings::FileType::TXTDescriptors);
  desc_io->write(dwrapper.get(),"descriptors0.txt");

  PANACEASettings settings_cross_mean = PANACEASettings::make()
                                      .set(EntropyType::Cross)
                                      .set(PANACEAAlgorithm::Flexible)
                                      .distributionType(kernel)
                                          .set(KernelPrimitive::Gaussian)
                                          .set(KernelCount::Single)
                                          .set(KernelCorrelation::Correlated)
                                          .set(KernelCenterCalculation::Mean)
                                          .set(KernelNormalization::None);

  PANACEASettings settings_cross_median = PANACEASettings::make()
                                      .set(EntropyType::Cross)
                                      .set(PANACEAAlgorithm::Flexible)
                                      .distributionType(kernel)
                                          .set(KernelPrimitive::Gaussian)
                                          .set(KernelCount::Single)
                                          .set(KernelCorrelation::Correlated)
                                          .set(KernelCenterCalculation::Median)
                                          .set(KernelNormalization::None);


  auto cross_mean = panacea_pi.create(dwrapper.get(), settings_cross_mean);
  auto cross_median = panacea_pi.create(dwrapper.get(), settings_cross_median);

  auto kern_dist_io = panacea_pi.create(settings::FileType::TXTKernelDistribution);
  kern_dist_io->write(cross_mean.get(), "cross_mean0.txt");
  kern_dist_io->write(cross_median.get(), "cross_median0.txt");
  return 0;
}
