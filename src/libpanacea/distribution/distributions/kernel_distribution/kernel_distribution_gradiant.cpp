
// Local private PANACEA includes
#include "kernel_distribution_gradiant.hpp"

#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "primitives/primitive.hpp"
#include "primitives/primitive_group.hpp"
#include "settings.hpp"

// Standard includes
#include <iostream>
#include <vector>

namespace panacea {

  namespace {

    /**
     * Will calculate the gradiant at the location of the desc_index, 
     * assumes we are taking the gradiant with respect to the decriptor
     * thus each kernel will add a contribution.
     **/
    std::vector<double> gradiant_one_to_one_wrt_desc_only(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const int & grad_index, // Not really needed
        const PrimitiveGroup & prim_grp,
        const KernelDistributionSettings & distribution_settings,
        const double pre_factor
        ) {

      // We want the gradiant at the location of the sample
      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      assert(grad_index == descriptor_index && "It doesn't make sense to have the gradiant with respect to a different index");
      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
      for( auto & prim_ptr : prim_grp.primitives ) {

        std::vector<double> grad_temp = prim_ptr->compute_grad(
            descriptor_wrapper,
            descriptor_index,
            distribution_settings.equation_settings,
            settings::GradSetting::WRTDescriptor);

        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      }

      std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(), std::placeholders::_1, pre_factor));

      return grad;
    }

    /**
     * Will calculate the gradiant at the location of the desc_index, 
     * assumes we are taking the gradiant with respect to the decriptor
     * with a single kernel, thus the gradient will not need to effect
     * any changes to the kernel. 
     **/
    std::vector<double> gradiant_single_wrt_desc_only(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const int & grad_index, // Not really needed
        const PrimitiveGroup & prim_grp,
        const KernelDistributionSettings & distribution_settings,
        const double pre_factor
        ) {

      // We want the gradiant at the location of the sample
      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      assert(grad_index == descriptor_index && "It doesn't make sense to have the gradiant with respect to a different index");

      std::vector<double> grad = prim_grp.primitives.at(0)->compute_grad(
          descriptor_wrapper,
          descriptor_index,
          distribution_settings.equation_settings,
          settings::GradSetting::WRTDescriptor);

      std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(), std::placeholders::_1, pre_factor));

      return grad;
    }

    /**
     * One to one mapping between the descriptors and the kernels, this
     * means that the kernels and descriptors are the same. We are interested
     * in the gradiant at the location of the descriptor index if we were to 
     * change the kernel center.
     *
     * Hence there will only be one kernel that is relevant, all the other kernels
     * will not add a contribution. 
     **/
    std::vector<double> gradiant_one_to_one_wrt_kern_only(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const int & grad_index,
        const PrimitiveGroup & prim_grp,
        const KernelDistributionSettings & distribution_settings,
        const double pre_factor
        ) {

      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );

      auto grad = prim_grp.primitives.at(grad_index)->compute_grad(
            descriptor_wrapper,
            descriptor_index,
            distribution_settings.equation_settings,
            settings::GradSetting::WRTKernel);

      std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(), std::placeholders::_1, pre_factor));
      return grad;
    }

    std::vector<double> gradiant_one_to_one_wrt_both(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const int & grad_index, 
        const PrimitiveGroup & prim_grp,
        const KernelDistributionSettings & distribution_settings,
        const double pre_factor
        ) {

      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      assert(descriptor_index == grad_index);

      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
      for( auto & prim_ptr : prim_grp.primitives ) {
        // Ignore the gradiant of the kernel with the same index because the gradiants will cancel
        if(prim_ptr->getId() != descriptor_index ) {
          std::vector<double> grad_temp = prim_ptr->compute_grad(
              descriptor_wrapper,
              descriptor_index,
              distribution_settings.equation_settings,
              settings::GradSetting::WRTDescriptor);

          std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());
        }
      }

      std::transform(grad.begin(), grad.end(), grad.begin(),
          std::bind(std::multiplies<double>(), std::placeholders::_1, pre_factor));
      return grad;
    }

  } // namespace
  /***************************************************************
   * Declaring public Member function maps
   **************************************************************/

  std::unordered_map<settings::GradSetting,
        std::unordered_map<settings::EquationSetting,
        std::unordered_map<settings::KernelCount,
        KernelDistributionGradiant::GradiantMethod>>> 
          KernelDistributionGradiant::grad_method;

  KernelDistributionGradiant::KernelDistributionGradiant() {
    // Register gradiant methods
    grad_method
      [settings::GradSetting::WRTDescriptor]
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne] = gradiant_one_to_one_wrt_desc_only;

    grad_method
      [settings::GradSetting::WRTKernel]
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne] = gradiant_one_to_one_wrt_kern_only;

    grad_method
      [settings::GradSetting::WRTBoth]
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne] = gradiant_one_to_one_wrt_both;

    grad_method
      [settings::GradSetting::WRTDescriptor]
      [settings::EquationSetting::None]
      [settings::KernelCount::Single] = gradiant_single_wrt_desc_only;
  }

}
