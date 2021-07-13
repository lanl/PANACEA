
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
#include "settings.hpp"

// Standard includes
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>

namespace panacea {

  settings::EntropyType SelfEntropy::type() const noexcept {
    return settings::EntropyType::Self;
  }

  std::vector<EntropyTerm::ReadElement> SelfEntropy::getReadElements(const PassKey<EntropyTerm> &) {
    return std::vector<EntropyTerm::ReadElement> { EntropyTerm::ReadElement{SelfEntropy::read, *this} };
  }

  std::vector<EntropyTerm::WriteElement> SelfEntropy::getWriteElements(const PassKey<EntropyTerm> &) const {
    return std::vector<EntropyTerm::WriteElement> {EntropyTerm::WriteElement{SelfEntropy::write, *this}};
  }

  bool SelfEntropy::set(const settings::EntropyOption opt, std::any value){
    return false;
  }

  std::any SelfEntropy::get(const settings::EntropyOption opt) const {
    std::string error_msg = "Unsupported option " + std::string(toString(opt));
    error_msg += " for this entropy term. Perhaps your entropy term was ";
    error_msg += "not initialized the way you expected it to be.";
    PANACEA_FAIL(error_msg);
    return false;
  }


  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper
      ) {

    if( state_ != EntropyTerm::State::Initialized) {
      std::string error_msg = "Trying to call compute on an entropy term before it has ";
      error_msg += "been initialized, please either initialize the entropy term first";
      error_msg += " or when creating the entropy term provide the descriptors.";
      PANACEA_FAIL(error_msg);
    }
    double self_entropy = 0.0;
    for( int desc_pt = 0; desc_pt < descriptor_wrapper.getNumberPoints(); ++desc_pt ){
      self_entropy += -1.0 * log(distribution_->compute(
            descriptor_wrapper,
            desc_pt,
            entropy_settings_.getDistributionSettings(Method::Compute)));
    }
    return self_entropy;
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind
      ) {

    if( state_ != EntropyTerm::State::Initialized) {
      std::string error_msg = "Trying to call compute on an entropy term before it has ";
      error_msg += "been initialized, please either initialize the entropy term first";
      error_msg += " or when creating the entropy term provide the descriptors.";
      PANACEA_FAIL(error_msg);
    }
    return -1.0 * log(distribution_->compute(
          descriptor_wrapper,
          desc_ind,
          entropy_settings_.getDistributionSettings(Method::Compute)
          ));
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const EntropySettings & entropy_settings
      ) {

    entropy_settings_ = entropy_settings;
    return compute(descriptor_wrapper);
  }

  double SelfEntropy::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings
      ) {

    entropy_settings_ = entropy_settings;
    return compute(descriptor_wrapper, desc_ind);
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
          const int desc_ind // Where the gradiant is being calculated at
          ) {

    if( state_ != EntropyTerm::State::Initialized) {
      std::string error_msg = "Trying to call compute_grad on an entropy term before it has ";
      error_msg += "been initialized, please either initialize the entropy term first";
      error_msg += " or when creating the entropy term provide the descriptors.";
      PANACEA_FAIL(error_msg);
    }
      std::vector<double> inv_distribution;
      inv_distribution.reserve(descriptor_wrapper.getNumberPoints());
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper.getNumberPoints(); ++desc_ind2 ){
        inv_distribution.push_back(-1.0/distribution_->compute(
              descriptor_wrapper,
              desc_ind2,
              entropy_settings_.getDistributionSettings(Method::ComputeGradiant)));
      }
      // Compute the gradiant with respect to the Descriptors
      std::vector<double> grad(descriptor_wrapper.getNumberDimensions(),0.0);

      // Compute the gradiant with respect to each of the Kernels
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper.getNumberPoints(); ++desc_ind2 ){
        std::vector<double> grad_temp = distribution_->compute_grad(
            descriptor_wrapper,
            desc_ind2, // desc_ind
            desc_ind,  // where we are taking gradiant wrt
            entropy_settings_.getDistributionSettings(Method::ComputeGradiant));

        std::transform(grad_temp.begin(), grad_temp.end(), grad_temp.begin(),
          std::bind(std::multiplies<double>(),
            std::placeholders::_1,
            inv_distribution.at(desc_ind2)));

        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      }

      return grad;
  }


  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper & descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const EntropySettings & entropy_settings
          ) {

    entropy_settings_ = entropy_settings;
    return compute_grad(descriptor_wrapper, desc_ind);;
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
    return std::make_unique<SelfEntropy>(key, std::move(dist), settings, EntropyTerm::State::Initialized);
  }

  std::unique_ptr<EntropyTerm> SelfEntropy::create(
      const PassKey<EntropyFactory> & key,
      const EntropySettings & settings) {

    DistributionFactory dist_factory;

    auto dist = dist_factory.create(settings.getDistributionSettings(Method::Create));
    return std::make_unique<SelfEntropy>(key, std::move(dist), settings, EntropyTerm::State::Shell);
  }

  const std::vector<int> SelfEntropy::getDimensions() const noexcept {
    return distribution_->getDimensions().convert();
  }

  void SelfEntropy::update(const BaseDescriptorWrapper & descriptor_wrapper) {
    if( state_ != EntropyTerm::State::Initialized) {
      std::string error_msg = "Trying to call update on an entropy term before it has ";
      error_msg += "been initialized, please either initialize the entropy term first";
      error_msg += " or when creating the entropy term provide the descriptors.";
      PANACEA_FAIL(error_msg);
    }
    distribution_->update(descriptor_wrapper);
  }

  void SelfEntropy::initialize(const BaseDescriptorWrapper & descriptor_wrapper) {
    distribution_->initialize(descriptor_wrapper);
    state_ = EntropyTerm::State::Initialized;
  }

  std::vector<std::any> SelfEntropy::write(
      const settings::FileType file_type,
      std::ostream & os,
      const EntropyTerm & entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      if( entropy_term_instance.type() == settings::EntropyType::Self){

        if( file_type == settings::FileType::TXTRestart) {
          if(entropy_term_instance.state() != EntropyTerm::State::Initialized){
            std::string error_msg = "Only entropy terms that have been initialized can";
            error_msg += " be written to a restart file. If you want to still output ";
            error_msg += "partial content of the term consider using a different file ";
            error_msg += "type other than restart.";
            PANACEA_FAIL(error_msg);
          }
        }

      try {
        const SelfEntropy & self_ent =
          dynamic_cast<const SelfEntropy &>(entropy_term_instance);
        nested_values.push_back(self_ent.distribution_.get());
      } catch (...) {
        std::string error_msg = "Bad cast when trying to cast from ";
        error_msg += "const EntropyTerm & to const SelfEntropy &.";
        PANACEA_FAIL(error_msg);
      }

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
        try {
          SelfEntropy & self_ent =
            dynamic_cast<SelfEntropy &>(entropy_term_instance);
          nested_values.emplace_back(self_ent.distribution_.get(), std::nullopt);

          // Set the file type to initialized if reading a restart file
          // This means that only entropy terms that have been initialized should be
          // written to restart files
          if( file_type == settings::FileType::TXTRestart) {
            self_ent.state_ = EntropyTerm::State::Initialized;
          }
        } catch (...) {
          std::string error_msg = "Bad cast when attempting to convert ";
          error_msg += "EntropyTerm & to SelfEntropy &.";
          PANACEA_FAIL(error_msg);
        }
      } else {
        PANACEA_FAIL("Unsupported entropy term encountered.");
      }
    }
    return nested_values;
   }

}
