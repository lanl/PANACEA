
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

  double before_total_entropy = 0.0;
  for( auto & ent_term : entropy_terms ) {
    before_total_entropy += ent_term->compute(*dwrapper);
  }

   auto restart_file = panacea_pi.create(settings::FileType::TXTRestart);
   ofstream restart_out;
   restart_out.open("self_and_cross_entropy_restart.txt");
   for( auto & ent_term : entropy_terms ) {
     restart_file->write(ent_term.get(), restart_out);
   }
   restart_out.close();

   std::vector<std::unique_ptr<EntropyTerm>> entropy_terms_shell;
   entropy_terms_shell.emplace_back(panacea_pi.create(panacea_settings_self));
   entropy_terms_shell.emplace_back(panacea_pi.create(panacea_settings_cross));

   ifstream restart_in;
   restart_in.open("self_and_cross_entropy_restart.txt");
   for( auto & ent_term : entropy_terms_shell ) {
     restart_file->read(ent_term.get(), restart_in);
   }
   restart_in.close();

   double after_total_entropy = 0.0;
   for( auto & ent_term : entropy_terms_shell ) {
     after_total_entropy += ent_term->compute(*dwrapper);
   }

   REQUIRE(before_total_entropy == Approx(after_total_entropy));
}

