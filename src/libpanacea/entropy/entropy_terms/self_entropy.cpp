
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "self_entropy.hpp"

#include "distribution/distributions/distribution.hpp"
#include "distribution/distributions/kernel_distribution.hpp"
#include "distribution/distribution_factory.hpp"
#include "distribution/distribution_settings/distribution_settings.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"
#include "error.hpp"

// Standard includes
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace panacea {

  settings::EntropyType SelfEntropy::type() const noexcept {
    return settings::EntropyType::Self;
  }

  EntropyTerm::ReadFunction SelfEntropy::getReadFunction(const PassKey<EntropyTerm> &) {
    return SelfEntropy::read;
  }

  EntropyTerm::WriteFunction SelfEntropy::getWriteFunction(const PassKey<EntropyTerm> &) const {
    return SelfEntropy::write;
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const EntropySettings & entropy_settings
      ) {

    double self_entropy = 0.0;
    for( int desc_pt = 0; desc_pt < descriptor_wrapper.getNumberPoints(); ++desc_pt ){
      self_entropy += -1.0 * log(distribution_->compute(
            descriptor_wrapper,
            desc_pt,
            entropy_settings.getDistributionSettings(Method::Compute)));
      std::cout << "desc_pt " << desc_pt << " self " << self_entropy << std::endl;
    }
    return self_entropy;
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings
      ) {

    return -1.0 * log(distribution_->compute(
          descriptor_wrapper,
          desc_ind,
          entropy_settings.getDistributionSettings(Method::Compute)
          ));
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const PANACEASettings & panacea_settings
      ) {
    return compute(descriptor_wrapper, EntropySettings(panacea_settings));
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const PANACEASettings & panacea_settings
      ) {
    return compute(descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const EntropySettings & entropy_settings
          ) {

    std::cout << "Descriptor wrapper called from compute_grad in self entropy" << std::endl;
      for( int i = 0; i < descriptor_wrapper.getNumberPoints(); ++i){
        for( int j = 0; j< descriptor_wrapper.getNumberDimensions(); ++j) {
          std::cout << descriptor_wrapper(i,j) << " ";
        }
        std::cout << "\n";
      }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;


      std::vector<double> inv_distribution;
      inv_distribution.reserve(descriptor_wrapper.getNumberPoints());
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper.getNumberPoints(); ++desc_ind2 ){
        inv_distribution.push_back(-1.0/distribution_->compute(
              descriptor_wrapper,
              desc_ind2,
              entropy_settings.getDistributionSettings(Method::ComputeGradiant)));
      }
      // Compute the gradiant with respect to the Descriptors
      std::vector<double> grad(descriptor_wrapper.getNumberDimensions(),0.0);

      // Compute the gradiant with respect to each of the Kernels
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper.getNumberPoints(); ++desc_ind2 ){
        std::vector<double> grad_temp = distribution_->compute_grad(
            descriptor_wrapper,
            desc_ind2, // desc_ind
            desc_ind,  // where we are taking gradiant wrt
            entropy_settings.getDistributionSettings(Method::ComputeGradiant));

        std::transform(grad_temp.begin(), grad_temp.end(), grad_temp.begin(),
          std::bind(std::multiplies<double>(),
            std::placeholders::_1,
            inv_distribution.at(desc_ind2)));

        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      }

        std::cout << "grad values " << std::endl;
      for( auto & val : grad) {
        std::cout << val << " ";
      }
      std::cout << std::endl;
      return grad;
  }

  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const PANACEASettings & panacea_settings
          ) {
    return compute_grad(descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  std::unique_ptr<EntropyTerm> SelfEntropy::create(
      const PassKey<EntropyFactory> & key,
      const BaseDescriptorWrapper & descriptor_wrapper,
      const EntropySettings & settings) {

    DistributionFactory dist_factory;

    auto dist = dist_factory.create(descriptor_wrapper, settings.getDistributionSettings(Method::Create));
    return std::make_unique<SelfEntropy>(key, std::move(dist));
  }

  std::unique_ptr<EntropyTerm> SelfEntropy::create(
      const PassKey<EntropyFactory> & key,
      const EntropySettings & settings) {

    DistributionFactory dist_factory;


    auto dist = dist_factory.create(settings.getDistributionSettings(Method::Create));
    return std::make_unique<SelfEntropy>(key, std::move(dist));
  }

  void SelfEntropy::set(const settings::EntropyOption option, std::any val) {
    std::string error_msg = "SelfEntropy does not contain any options that can be set";
    PANACEA_FAIL(error_msg);
  }

  const std::vector<int> & SelfEntropy::getDimensions() const noexcept {
    return distribution_->getDimensions();
  }

  void SelfEntropy::update(const BaseDescriptorWrapper & descriptor_wrapper) {
    distribution_->update(descriptor_wrapper);
  }

  void SelfEntropy::initialize(const BaseDescriptorWrapper & descriptor_wrapper) {
    distribution_->initialize(descriptor_wrapper);
  }

  std::vector<std::any> SelfEntropy::write(
      const settings::FileType file_type,
      std::ostream & os,
      const EntropyTerm & entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      if( entropy_term_instance.type() == settings::EntropyType::Self){
        const auto & self_ent = dynamic_cast<const SelfEntropy &>(
            entropy_term_instance);
        nested_values.push_back(self_ent.distribution_.get());
      } else {
        PANACEA_FAIL("Unsupported entropy term encountered.");
      }
    }
    return nested_values;
  }

   io::ReadInstantiateVector SelfEntropy::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm & entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      if( entropy_term_instance.type() == settings::EntropyType::Self){
        auto & self_ent = dynamic_cast<SelfEntropy &>(entropy_term_instance);
        nested_values.emplace_back(self_ent.distribution_.get(), std::nullopt);
      } else {
        PANACEA_FAIL("Unsupported entropy term encountered.");
      }
    }
    return nested_values;
   }



}
