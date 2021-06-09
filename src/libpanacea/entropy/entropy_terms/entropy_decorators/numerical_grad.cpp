
// Local private PANACEA includes
#include "numerical_grad.hpp"

#include "descriptors/descriptor_wrapper.hpp"

// Standard includes
#include <cassert>
#include <vector>

namespace panacea {

std::vector<double> NumericalGrad::compute_grad(
    const BaseDescriptorWrapper *const_descriptor_wrapper, const int wrt_pt,
    const EntropySettings &entropy_settings) {

  if (numerical_grad_) {
    /**
     * Why are we doing this?
     *
     * The easiest and most performant way to numerically calculate the gradiant
     * is to simply adjust values in the descriptor wrapper compute the entropy
     * and then return it to it's original value, this avoids an unneccessary
     *copy of the descriptor wrapper which could be quite complex and costly to
     *create.
     *
     * Though we are making changes to the descriptor wrapper within this
     * method, it is being returned to it's original state before returning.
     **/
    auto descriptor_wrapper =
        const_cast<BaseDescriptorWrapper *>(const_descriptor_wrapper);

    const int ndim = getDimensions().size();
    std::vector<double> grad(ndim, 0.0);

    // Cycle through dimensions
    for (const int &dim : getDimensions()) {
      assert(dim < ndim);
      const double orig_x_val = descriptor_wrapper->operator()(wrt_pt, dim);
      const double diff = orig_x_val * inc_ratio_;
      // Change in this dimention
      const std::vector<double> new_x_vals = {orig_x_val - diff,
                                              orig_x_val + diff};

      // Temporarily alter the input value
      std::vector<double> new_y_vals(2, 0.0);
      descriptor_wrapper->operator()(wrt_pt, dim) = new_x_vals.at(0);
      new_y_vals.at(0) += EntropyDecorator::compute(descriptor_wrapper);
      descriptor_wrapper->operator()(wrt_pt, dim) = new_x_vals.at(1);
      new_y_vals.at(1) += EntropyDecorator::compute(descriptor_wrapper);
      grad.at(dim) = (new_y_vals.at(1) - new_y_vals.at(0)) / (2.0 * diff);
      // Reset to the original value
      descriptor_wrapper->operator()(wrt_pt, dim) = orig_x_val;
    }
    return grad;
  } else {
    return EntropyDecorator::compute_grad(const_descriptor_wrapper, wrt_pt,
                                          entropy_settings);
  }
}

void NumericalGrad::set(const settings::EntropyOption option, std::any val) {
  if (option == settings::EntropyOption::IncrementRatio) {
    inc_ratio_ = std::any_cast<double>(val);
  } else if (option == settings::EntropyOption::NumericalGrad) {
    numerical_grad_ = std::any_cast<bool>(val);
  } else {
    EntropyDecorator::set(option, val);
  }
}
} // namespace panacea
