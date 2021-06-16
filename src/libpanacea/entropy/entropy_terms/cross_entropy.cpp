
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "cross_entropy.hpp"

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

namespace panacea {

  settings::EntropyType CrossEntropy::type() const noexcept {
    return settings::EntropyType::Cross;
  }

  EntropyTerm::ReadFunction CrossEntropy::getReadFunction(const PassKey<EntropyTerm> &) {
    return CrossEntropy::read;
  }

  EntropyTerm::WriteFunction CrossEntropy::getWriteFunction(const PassKey<EntropyTerm> &) {
    return CrossEntropy::write;
  }

  double CrossEntropy::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const EntropySettings & entropy_settings
      ) {

    assert(descriptor_wrapper != nullptr);

    double cross_entropy = 0.0;
    for( int desc_pt = 0; desc_pt < descriptor_wrapper->getNumberPoints(); ++desc_pt ){
      cross_entropy += -1.0 * log(distribution_->compute(
            descriptor_wrapper,
            desc_pt,
            entropy_settings.getDistributionSettings(Method::Compute)));
    }
    return cross_entropy;
  }

  double CrossEntropy::compute(const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings
      ) {

    assert(descriptor_wrapper != nullptr);

    return -1.0 * log(distribution_->compute(
          descriptor_wrapper,
          desc_ind,
          entropy_settings.getDistributionSettings(Method::Compute)));
  }

  double CrossEntropy::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const PANACEASettings & panacea_settings
      ) {
    return compute( descriptor_wrapper, EntropySettings(panacea_settings));
  }

  double CrossEntropy::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const PANACEASettings & panacea_settings
      ) {
    return compute( descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  std::vector<double> CrossEntropy::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const EntropySettings & entropy_settings
          ) {


      std::vector<double> inv_distribution;
      inv_distribution.reserve(descriptor_wrapper->getNumberPoints());
      std::cout << std::endl;
      std::cout << "Calling inverse" << std::endl;
      std::cout << std::endl;
      std::cout << std::endl;
      for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper->getNumberPoints(); ++desc_ind2 ){

        /***
         * Here is the problem I'm calling compute from within the gradiant method
         * but have no way of indicating that.
         */
        inv_distribution.push_back(-1.0/distribution_->compute(
              descriptor_wrapper,
              desc_ind2,
              entropy_settings.getDistributionSettings(Method::ComputeGradiant)));

      }

      std::cout << "Number of points " << descriptor_wrapper->getNumberPoints() << std::endl;
      std::cout << "Inverse distribution should be 1" << std::endl;
      for( auto & inv_val : inv_distribution ) {
        std::cout << inv_val << " ";
      }
      std::cout << std::endl;
      // Compute the gradiant with respect to the Descriptors
      //std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);

      // Compute the gradiant with respect to each of the Kernels
      //for( int desc_ind2 = 0; desc_ind2 < descriptor_wrapper->getNumberPoints(); ++desc_ind2 ){

        // We assume the gradient is only ever calculated with changes in the
        // descriptors, the kernel center is not going to move in this case,
        // this is different from the self entropy term.
        //std::vector<double> grad_temp = distribution_->compute_grad(
        std::vector<double> grad = distribution_->compute_grad(
            descriptor_wrapper,
            desc_ind, // desc_ind
            desc_ind, // grad_ind
            entropy_settings.getDistributionSettings(Method::ComputeGradiant),
            settings::GradSetting::WRTDescriptor);

        std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(),
            std::placeholders::_1,
            inv_distribution.at(desc_ind)));

        //std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      //}
      return grad;
  }


  std::vector<double> CrossEntropy::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind, // Where the gradiant is being calculated at
          const PANACEASettings & panacea_settings
          ) {
    return compute_grad(descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  std::unique_ptr<EntropyTerm> CrossEntropy::create(
      const PassKey<EntropyFactory> & key,
      const BaseDescriptorWrapper * descriptor_wrapper,
      const EntropySettings & settings) {

    // Create distribution
    DistributionFactory dist_factory;

    auto dist = dist_factory.create(descriptor_wrapper, settings.getDistributionSettings(Method::Create));
    return std::make_unique<CrossEntropy>(key, std::move(dist));
  }

  std::unique_ptr<EntropyTerm> CrossEntropy::create(
      const PassKey<EntropyFactory> & key,
      const EntropySettings & settings) {

    // Create distribution
    DistributionFactory dist_factory;

    auto dist = dist_factory.create(settings.getDistributionSettings(Method::Create));
    return std::make_unique<CrossEntropy>(key, std::move(dist));
  }

  void CrossEntropy::set(const settings::EntropyOption option, std::any val) {
    std::string error_msg = "CrossEntropy does not contain any options that can be set";
    PANACEA_FAIL(error_msg);
  }


  const std::vector<int> & CrossEntropy::getDimensions() const noexcept {
    return distribution_->getDimensions();
  }

  void CrossEntropy::update(const BaseDescriptorWrapper * descriptor_wrapper) {
    distribution_->update(descriptor_wrapper);
  }

  void CrossEntropy::initialize(const BaseDescriptorWrapper * descriptor_wrapper) {
    distribution_->initialize(descriptor_wrapper);
  }

  std::vector<std::any> CrossEntropy::write(
      const settings::FileType file_type,
      std::ostream & os,
      EntropyTerm * entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      auto cross_ent = dynamic_cast<CrossEntropy *>(entropy_term_instance);
      nested_values.push_back(cross_ent->distribution_.get());
    }
    return nested_values;
  }

   io::ReadInstantiateVector CrossEntropy::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm * entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {
      auto cross_ent = dynamic_cast<CrossEntropy *>(entropy_term_instance);
      nested_values.emplace_back(cross_ent->distribution_.get(), std::nullopt);
    }
    return nested_values;
   }


}
