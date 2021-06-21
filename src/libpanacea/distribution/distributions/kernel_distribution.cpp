
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "kernel_distribution.hpp"

#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "primitives/primitive.hpp"
#include "primitives/gaussian_uncorrelated.hpp"
#include "primitives/gaussian_correlated.hpp"
#include "primitives/primitive_factory.hpp"
#include "primitives/primitive_group.hpp"
#include "private_settings.hpp"

// Standard includes
#include <iostream>
#include <string>
#include <vector>

namespace panacea {

  KernelDistribution::KernelDistribution(const PassKey<DistributionFactory> &,
          const BaseDescriptorWrapper * descriptor_wrapper,
          const KernelSpecification & settings) {

    PrimitiveFactory prim_factory;
    prim_grp_ = prim_factory.createGroup(
        descriptor_wrapper,
        settings);

    pre_factor_ = 1.0/static_cast<double>(prim_grp_.primitives.size());

  }

  KernelDistribution::KernelDistribution(const PassKey<DistributionFactory> &,
          const KernelSpecification & settings) {

    PrimitiveFactory prim_factory;
    prim_grp_ = prim_factory.createGroup(settings);

    pre_factor_ = 1.0/static_cast<double>(prim_grp_.primitives.size());

  }

  Distribution::ReadFunction KernelDistribution::getReadFunction_() {
    return KernelDistribution::read;
  }

  Distribution::WriteFunction KernelDistribution::getWriteFunction_() {
    return KernelDistribution::write;
  }

  settings::DistributionType KernelDistribution::type() const noexcept {
    return settings::DistributionType::Kernel;
  }

  double KernelDistribution::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const DistributionSettings & distribution_settings_
      ) {
    assert(distribution_settings_.type() == settings::DistributionType::Kernel );

    auto distribution_settings =
      dynamic_cast<const KernelDistributionSettings &>(distribution_settings_);

    std::cout << "Kernel count" << std::endl;
    std::cout << distribution_settings.dist_settings.get<settings::KernelCount>() << std::endl;
    std::cout << "Equation settings" << std::endl;


    std::cout << "Descriptor wrapper called from compute in kernel distribution" << std::endl;
      for( int i = 0; i < descriptor_wrapper->getNumberPoints(); ++i){
        for( int j = 0; j< descriptor_wrapper->getNumberDimensions(); ++j) {
          std::cout << descriptor_wrapper->operator()(i,j) << " ";
        }
        std::cout << "\n";
      }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;




    double density = 0.0;
    for( auto & prim_ptr : prim_grp_.primitives ) {
      density += prim_ptr->compute(
          descriptor_wrapper,
          desc_ind,
          distribution_settings.eq_settings);
    }

    double result = pre_factor_ * density;
    if( result == 0.0 ) {
      return std::numeric_limits<double>::min();
    }

    std::cout << "pre_factor_ " << pre_factor_ << " density " << density << std::endl;
    return result;
  }

  std::vector<double> KernelDistribution::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          const int grad_ind,
          const DistributionSettings & distribution_settings_,
          std::any option
          ) {

    assert(descriptor_wrapper->getNumberDimensions() == prim_grp_.kernel_wrapper->getNumberDimensions());
    assert(distribution_settings_.type() == settings::DistributionType::Kernel );

    settings::GradSetting grad_setting;
    if( option.type() != typeid(settings::None) ){
      grad_setting = std::any_cast<settings::GradSetting>(option);
    }
    auto distribution_settings =
      dynamic_cast<const KernelDistributionSettings &>(distribution_settings_);

#ifndef NDEBUG
    std::string error_msg = "Unsupported gradiant method encountered";
    if( option.type() == typeid(settings::None) ) {
      if(kern_dist_grad.grad_method
          .count(settings::GradSetting::WRTBoth) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          .count(settings::GradSetting::WRTKernel) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [settings::GradSetting::WRTBoth]
          .count(distribution_settings.eq_settings) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [settings::GradSetting::WRTKernel]
          .count(distribution_settings.eq_settings) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [settings::GradSetting::WRTBoth]
          [distribution_settings.eq_settings]
          .count(distribution_settings.dist_settings.get<settings::KernelCount>()) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [settings::GradSetting::WRTKernel]
          [distribution_settings.eq_settings]
          .count(distribution_settings.dist_settings.get<settings::KernelCount>()) == 0) {
        PANACEA_FAIL(error_msg);
      }
    } else {
      if(kern_dist_grad.grad_method
          .count(grad_setting) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [grad_setting]
          .count(distribution_settings.eq_settings) == 0) {
        PANACEA_FAIL(error_msg);
      }
      if(kern_dist_grad.grad_method
          [grad_setting]
          [distribution_settings.eq_settings]
          .count(distribution_settings.dist_settings.get<settings::KernelCount>()) == 0) {
        PANACEA_FAIL(error_msg);
      }
    }
#endif // NDEBUG

    if( option.type() == typeid(settings::None) ) {
      if( desc_ind == grad_ind ) {

        return kern_dist_grad.grad_method
          [settings::GradSetting::WRTBoth]
          [distribution_settings.eq_settings]
            [distribution_settings.dist_settings.get<settings::KernelCount>()](
                descriptor_wrapper,
                desc_ind,
                grad_ind,
                prim_grp_,
                distribution_settings,
                pre_factor_);
      } else {
        // Remaining cases should occur WRTKernel only
        return kern_dist_grad.grad_method
          [settings::GradSetting::WRTKernel]
          [distribution_settings.eq_settings]
            [distribution_settings.dist_settings.get<settings::KernelCount>()](
                descriptor_wrapper,
                desc_ind,
                grad_ind,
                prim_grp_,
                distribution_settings,
                pre_factor_);
      }
    } else {

      return kern_dist_grad.grad_method
          [grad_setting]
          [distribution_settings.eq_settings]
            [distribution_settings.dist_settings.get<settings::KernelCount>()](
                descriptor_wrapper,
                desc_ind,
                grad_ind,
                prim_grp_,
                distribution_settings,
                pre_factor_);

    }

  }

  const std::vector<int> & KernelDistribution::getDimensions() const noexcept {
    assert(prim_grp_.reduced_covariance!=nullptr);
    return prim_grp_.reduced_covariance->getReducedDimensions();
  }

  void KernelDistribution::update(const BaseDescriptorWrapper * descriptor_wrapper) {
    prim_grp_.update(descriptor_wrapper);
    pre_factor_ = 1.0/static_cast<double>(prim_grp_.primitives.size());
  }

  void KernelDistribution::initialize(const BaseDescriptorWrapper * descriptor_wrapper) {
    prim_grp_.initialize(descriptor_wrapper);
    pre_factor_ = 1.0/static_cast<double>(prim_grp_.primitives.size());
  }

  std::vector<std::any> KernelDistribution::write(
          const settings::FileType file_type,
          std::ostream & os,
          Distribution * dist) {

    KernelDistribution * kern_dist = dynamic_cast<KernelDistribution *>(dist);

    std::vector<std::any> nested_objs;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution) {
      os << "[Prefactor]\n";
      os << kern_dist->pre_factor_ << "\n";
      nested_objs.push_back(&(kern_dist->prim_grp_));
    }
    return nested_objs;
  }

  io::ReadInstantiateVector KernelDistribution::read(
          const settings::FileType file_type,
          std::istream & is,
          Distribution * dist) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ||
        file_type == settings::FileType::TXTKernelDistribution ) {

      KernelDistribution * kern_dist = dynamic_cast<KernelDistribution *>(dist);
      std::string line = "";
      while(line.find("[Prefactor]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading kernel distribution section of restart file";
          error_msg += ", header does not contain the [Prefactor] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }
      is >> kern_dist->pre_factor_;
      nested_values.emplace_back(&(kern_dist->prim_grp_),std::nullopt);
    }
    return nested_values;
  }


}
