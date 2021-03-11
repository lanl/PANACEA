
#ifndef PANACEA_PRIVATE_NUMERICAL_ENGINE_H
#define PANACEA_PRIVATE_NUMERICAL_ENGINE_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  /*
   * Takes an object with a compute method and a way to alter a parameter
   * and calculates the gradient numerically. 
   *
   * This class is designed to work with interfaces such as the Base Kernel Wrapper and 
   * the base descriptor wrapper as the input
   *
   * And either primitives, distributions, or entropy terms as the computer
   */
  class NumericalEngine {
      // How much the input should be varied via ratio
      // to compute the gradient
      double inc_ratio_ = 0.001;
      // Which dimensions are active
      const std::vector<int> active_dimensions_;
    public:
      NumericalEngine(const std::vector<int> & active_dimensions) : active_dimensions_(active_dimensions) {};
      NumericalEngine(const std::vector<int> & active_dimensions, double inc_ratio) : 
        inc_ratio_(inc_ratio),
        active_dimensions_(active_dimensions) {};
      template<class T, class U> 
      std::vector<double> compute_grad(T & input, U & computer, const int ind1, const int ind2);
  };

  /*
   * Method will calculate the gradient at the point index specified.
   */
  template<class T, class U>
    inline std::vector<double> compute_grad(
        T & input,
        U & computer,
        const int at_pt_ind) {
     
      const int ndim = input.getNumberDimensions(); 
      const int num_pts = input.getNumberPoints();
      assert(at_pt < num_pts && at_pt >= 0);

      std::vector<double> grad(ndim,0.0);

      // Cycle through dimensions
      for( int dim : active_dimensions_ ){
        assert(dim < ndim);
        double orig_x_val = input(at_pt_ind,at_dim);
        double diff = orig_x_val*inc_ratio_;
        // Change in this dimention
        std::vector<double> new_x_vals = {val-diff, val+diff};
        // Temporarily alter the input value
        std::vector<double> new_y_vals(2,0.0);
        for( int pt = 0; pt < num_pts; ++pt){
          input(at_pt_ind, dim) = new_x_vals.at(0);
          new_y_vals.at(0) += computer.compute(input,pt);
          input(at_pt_ind, dim) = new_x_vals.at(1);
          new_y_vals.at(1) += computer.compute(input,pt);
        }
        grad.at(dim) += (new_y_vals.at(1) - new_y_vals.at(0))/(2.0*diff);
      }

      // Reset to the original value
      input(at_pt,at_dim) = orig_val;
      return grad;
    }
}


#endif // PANACEA_PRIVATE_NUMERICAL_ENGINE_H
