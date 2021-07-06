
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/entropy_term.hpp"
#include "panacea/panacea.hpp"

// Local private includes
#include "helper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;
using namespace panacea::settings;

TEST_CASE("Testing:panacea self and cross entropy restart fileio","[end-to-end,panacea]"){

  // Creating settings for generating a self entropy term where the
  // underlying distribution is using an kernel estimator
  // that is a guassian kernel.
  PANACEASettings panacea_settings_self = PANACEASettings::make()
    .set(EntropyType::Self)
    .set(PANACEAAlgorithm::Flexible)
    .distributionType(kernel)
    .set(KernelPrimitive::Gaussian)
    .set(KernelCount::OneToOne)
    .set(KernelCorrelation::Uncorrelated)
    .set(KernelCenterCalculation::None)
    .set(KernelNormalization::None);

  PANACEASettings panacea_settings_cross = PANACEASettings::make()
    .set(EntropyType::Cross)
    .set(PANACEAAlgorithm::Flexible)
    .distributionType(kernel)
    .set(KernelPrimitive::Gaussian)
    .set(KernelCount::Single)
    .set(KernelCorrelation::Correlated)
    .set(KernelCenterCalculation::Mean)
    .set(KernelNormalization::None);


  // pi - public interface
  PANACEA panacea_pi;

  // Data has the following form, where it is stacked
  //
  //         col1   col2   col3
  // row1    1.0     2.0    3.0  Point 1
  // row2    1.0     2.0    3.0  Point 2
  test::ArrayData array_data;
  const int rows = 2;
  const int cols = 3;
  auto dwrapper = panacea_pi.wrap(&(array_data.data), rows, cols);

  std::vector<std::unique_ptr<EntropyTerm>> entropy_terms;

  entropy_terms.emplace_back(panacea_pi.create(*dwrapper, panacea_settings_self));
  entropy_terms.emplace_back(panacea_pi.create(*dwrapper, panacea_settings_cross));

    double total_entropy = 0.0;
  for( auto & ent_term : entropy_terms ) {
    total_entropy += ent_term->compute(*dwrapper);
  }

}

