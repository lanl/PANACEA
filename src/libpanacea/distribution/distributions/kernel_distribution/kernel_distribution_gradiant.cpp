
// Local private PANACEA includes
#include "kernel_distribution_gradiant.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "primitives/primitive.hpp"
#include "primitives/primitive_group.hpp"
#include "settings.hpp"

// Standard includes
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
        const PrimitiveGroup & prim_grp
        ) {

      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
      for( auto & prim_ptr : prim_grp_.primitives ) {

        std::vector<double> grad_temp = prim_ptr->compute_grad(
            descriptor_wrapper,
            desc_ind,
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
        const PrimitiveGroup & prim_grp
        ) {

      return prim_grp_.primitives.at(descriptor_index)->compute_grad(
            descriptor_wrapper,
            desc_ind,
            distribution_settings.equation_settings,
            settings::GradSetting::WRTKernel);
    }

    std::vector<double> gradiant_one_to_one_wrt_both(
        const BaseDescriptorWrapper * descriptor_wrapper,
        const int & descriptor_index,
        const PrimitiveGroup & prim_grp
        ) {

      std::vector<double> grad(descriptor_wrapper->getNumberDimensions(),0.0);
      for( auto & prim_ptr : prim_grp_.primitives ) {
        // Ignore the gradiant of the kernel with the same index because the gradiants will cancel
        if(prim_ptr->getId() != descriptor_index ) {
          std::vector<double> grad_temp = prim_ptr->compute_grad(
              descriptor_wrapper,
              desc_ind,
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
        KernelDistributionGradiant::GradiantSetting>>> 
          KernelDistributionGradiant::grad_method;

  KernelDistributionGradiant::KernelDistributionGradiant() {
    // Register gradiant methods
    grad_method
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne]
      [settings::GradianSetting::WRTDescriptor] = gradiant_one_to_one_wrt_desc_only;

    grad_method
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne]
      [settings::GradianSetting::WRTKernel] = gradiant_one_to_one_wrt_kern_only;

    grad_method
      [settings::EquationSetting::None]
      [settings::KernelCount::OneToOne]
      [settings::GradianSetting::WRTBoth] = gradiant_one_to_one_wrt_both;
  }

}
