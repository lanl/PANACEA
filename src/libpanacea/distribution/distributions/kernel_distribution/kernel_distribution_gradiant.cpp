
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
        const KernelDistributionSettings & distribution_settings
        ) {

      // We want the gradiant at the location of the sample
      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      assert(grad_index == descriptor_index && "It doesn't make sense to have the gradiant with respect to a different index");
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
      for( auto & prim_ptr : prim_grp.primitives ) {

        std::vector<double> grad_temp = prim_ptr->compute_grad(
            descriptor_wrapper,
            descriptor_index,
            distribution_settings.equation_settings,
            settings::GradSetting::WRTDescriptor);

        std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());

      }
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
        const KernelDistributionSettings & distribution_settings
        ) {

      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return prim_grp.primitives.at(grad_index)->compute_grad(
            descriptor_wrapper,
            descriptor_index,
            distribution_settings.equation_settings,
            settings::GradSetting::WRTKernel);
    }

    std::vector<double> gradiant_one_to_one_wrt_both(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const int & grad_index, 
        const PrimitiveGroup & prim_grp,
        const KernelDistributionSettings & distribution_settings
        ) {

      assert(descriptor_index < descriptor_wrapper->getNumberPoints() );
      assert(descriptor_index == grad_index);

      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
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
  }

}
