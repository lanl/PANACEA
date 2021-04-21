
// Local private PANACEA includes
#include "self_entropy.hpp"

// Standard includes
#include <cassert>
#include <cmath>

namespace panacea {

  settings::EntropyType SelfEntropy::type() const noexcept {
    return settings::EntropyType::Self;
  } 

  double SelfEntropy::compute(const BaseDescriptorWrapper * descriptor_wrapper) {

    assert(descriptor_wrapper != nullptr);

    double self_entropy = 0.0;
    for( int desc_pt = 0; desc_pt < descriptor_wrapper->getNumberPoints(); ++desc_pt ){
      self_entropy += = -1.0 * log(distribution_->compute(descriptor_wrapper, desc_pt));
    }
    return self_entropy;
  }

  std::vector<double> SelfEntropy::compute_grad(
          const BaseDescriptorWrapper * descriptor_wrapper,
          const int desc_ind,
          ) {

      std::vector<double> inv_distribution();
      inv_distribution.reserve(descriptor_wrapper->getNumberPoints());
      
      for( int desc_ind1 = 0; desc_ind1 < descriptor_wrapper->getNumberPoints(); ++desc_ind1 ){
        inv_distribution.push_back( 1.0/distribution_.compute(descriptor_wrapper, desc_ind1));
      }
     

      // Compute the gradiant with respect to the Descriptors 
      std::vector<double> grad_temp_wrt_desc = 
        distribution_.compute_grad(descriptor_wrapper, desc_ind);

      // Compute the gradiant with respect to each of the Kernels 
      std::vector<double> grad_temp_wrt_kern = distribution_.compute_grad(descriptor_wrapper, desc_ind);
/*      for( int desc_ind = 0; desc_ind < descriptor_wrapper->getNumberPoints(); ++desc_ind ){
            std::transform(grad_temp.begin(), grad_temp.end(), grad_temp.begin(),
                std::bind(std::multiplies<double>(), 
                  std::placeholders::_1, 
                  inv_p.at(sample_ii)));
            std::transform(grad.begin(), grad.end(), grad_temp.begin(), grad.begin(), std::plus<double>());
   
      }*/
  }


}
