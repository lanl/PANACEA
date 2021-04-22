
// Local private includes
#include "entropy/entropy_terms/self_entropy.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "entropy/entropy_factory.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "kernels/kernel_specifications.hpp"
#include "memory.hpp"
#include "settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <memory>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:entropy creation trivial","[unit,panacea]"){
  EntropyFactory entropy_factory;
  entropy_factory.registerEntropyTerm<SelfEntropy,settings::EntropyType::Self>();
}

TEST_CASE("Testing:self entropy","[unit,panacea]"){
  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

//  typedef DescriptorWrapper<std::vector<std::vector<double>>*> DWrap;
  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper_init(&data, 3, 2);

  KernelDistributionSettings kernel_settings;
  kernel_settings.dist_settings = std::move(KernelSpecification(
        settings::KernelCorrelation::Uncorrelated,
        settings::KernelCount::OneToOne,
        settings::KernelPrimitive::Gaussian,
        settings::KernelNormalization::None,
        settings::KernelMemory::Share,
        settings::KernelCenterCalculation::None
      ));

  EntropySettings settings;
  settings.type = settings::EntropyType::Self;
  settings.dist_settings = std::make_unique<KernelDistributionSettings>(kernel_settings);

  MemoryManager mem_manager;

  EntropyFactory entropy_factory;
  entropy_factory.registerEntropyTerm<SelfEntropy,settings::EntropyType::Self>();

  auto entropy_term = entropy_factory.create(
      &dwrapper_init,
      mem_manager,
      &settings);

  double val1 = entropy_term->compute(&dwrapper_init);
  std::cout << "Self entropy is " << val1 << " at initial positions." << std::endl;

  data.at(0).at(0) = 1.5;
  double val2 = entropy_term->compute(&dwrapper_init);
  std::cout << "Self entropy should decrease when descriptors moved closer to each other " << val2 << std::endl;

  REQUIRE( val2 < val1 );

  auto grad = entropy_term->compute_grad(&dwrapper_init,0, settings);
  std::cout << "Self entropy grad of first descriptor " << grad.at(0) << std::endl;


}
