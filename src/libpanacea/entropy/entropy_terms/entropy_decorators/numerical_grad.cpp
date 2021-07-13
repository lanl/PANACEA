
// Local private PANACEA includes
#include "numerical_grad.hpp"

#include "descriptors/descriptor_wrapper.hpp"
#include "entropy/entropy_settings/entropy_settings.hpp"

// Standard includes
#include <cassert>
#include <vector>

namespace panacea {

      std::vector<EntropyTerm::ReadElement> NumericalGrad::getReadFunction(const PassKey<EntropyTerm> & key) {
        std::vector<EntropyTerm::ReadElement> read_functions = EntropyDecorator::getReadFunction(key);
        read_functions.emplace_back(read, *this);
        return read_functions;
      }

      std::vector<EntropyTerm::WriteElement> NumericalGrad::getWriteFunction(const PassKey<EntropyTerm> &) const {
        std::vector<EntropyTerm::WriteElement> write_functions = EntropyDecorator::getWriteFunction(key);
        write_functions.emplace_back(write, *this);
        return write_functions;
      }

  /**
   * WARNING the following two methods look almost identical but they
   * are not.
   **/
  std::vector<double> NumericalGrad::compute_grad(
      const BaseDescriptorWrapper & const_descriptor_wrapper,
      const int wrt_pt) {

    if( numerical_grad_ ) {
      /**
       * Why are we doing this?
       *
       * The easiest and most performant way to numerically calculate the gradiant
       * is to simply adjust values in the descriptor wrapper compute the entropy
       * and then return it to it's original value, this avoids an unneccessary copy of the
       * descriptor wrapper which could be quite complex and costly to create.
       *
       * Though we are making changes to the descriptor wrapper within this
       * method, it is being returned to it's original state before returning.
       **/
      BaseDescriptorWrapper & descriptor_wrapper = const_cast<BaseDescriptorWrapper &>(const_descriptor_wrapper);

      const int ndim = getDimensions().size();
      std::vector<double> grad(ndim,0.0);

      // Cycle through dimensions
      for( const int & dim : getDimensions() ){
        assert(dim < ndim);
        const double orig_x_val = descriptor_wrapper(wrt_pt,dim);
        const double diff = orig_x_val*inc_ratio_;
        // Change in this dimention
        const std::vector<double> new_x_vals = {orig_x_val-diff, orig_x_val+diff};

        // Temporarily alter the input value
        std::vector<double> new_y_vals(2,0.0);
        descriptor_wrapper(wrt_pt, dim) = new_x_vals.at(0);
        new_y_vals.at(0) += EntropyDecorator::compute(descriptor_wrapper);
        descriptor_wrapper(wrt_pt, dim) = new_x_vals.at(1);
        new_y_vals.at(1) += EntropyDecorator::compute(descriptor_wrapper);
        grad.at(dim) = (new_y_vals.at(1) - new_y_vals.at(0))/(2.0*diff);
        // Reset to the original value
        descriptor_wrapper(wrt_pt,dim) = orig_x_val;
      }
      return grad;
    } else {
      return EntropyDecorator::compute_grad(const_descriptor_wrapper, wrt_pt);
    }
  }

  std::vector<double> NumericalGrad::compute_grad(
      const BaseDescriptorWrapper & const_descriptor_wrapper,
      const int wrt_pt,
      const EntropySettings & entropy_settings) {

    if( numerical_grad_ ) {
      /**
       * Why are we doing this?
       *
       * The easiest and most performant way to numerically calculate the gradiant
       * is to simply adjust values in the descriptor wrapper compute the entropy
       * and then return it to it's original value, this avoids an unneccessary copy of the
       * descriptor wrapper which could be quite complex and costly to create.
       *
       * Though we are making changes to the descriptor wrapper within this
       * method, it is being returned to it's original state before returning.
       **/
      BaseDescriptorWrapper & descriptor_wrapper = const_cast<BaseDescriptorWrapper &>(const_descriptor_wrapper);

      const int ndim = getDimensions().size();
      std::vector<double> grad(ndim,0.0);

      // Cycle through dimensions
      for( const int & dim : getDimensions() ){
        assert(dim < ndim);
        const double orig_x_val = descriptor_wrapper(wrt_pt,dim);
        const double diff = orig_x_val*inc_ratio_;
        // Change in this dimention
        const std::vector<double> new_x_vals = {orig_x_val-diff, orig_x_val+diff};

        // Temporarily alter the input value
        std::vector<double> new_y_vals(2,0.0);
        descriptor_wrapper(wrt_pt, dim) = new_x_vals.at(0);
        new_y_vals.at(0) += EntropyDecorator::compute(descriptor_wrapper,entropy_settings);
        descriptor_wrapper(wrt_pt, dim) = new_x_vals.at(1);
        new_y_vals.at(1) += EntropyDecorator::compute(descriptor_wrapper,entropy_settings);
        grad.at(dim) = (new_y_vals.at(1) - new_y_vals.at(0))/(2.0*diff);
        // Reset to the original value
        descriptor_wrapper(wrt_pt,dim) = orig_x_val;
      }
      return grad;
    } else {
      return EntropyDecorator::compute_grad(const_descriptor_wrapper, wrt_pt, entropy_settings);
    }
  }

  std::vector<double> NumericalGrad::compute_grad(
      const BaseDescriptorWrapper & const_descriptor_wrapper,
      const int wrt_pt,
      const PANACEASettings & panacea_settings) {
    return NumericalGrad::compute_grad(
        const_descriptor_wrapper,
        wrt_pt,
        EntropySettings(panacea_settings));
  }

  std::any NumericalGrad::get(const settings::EntropyOption option) const {
    if( option == settings::EntropyOption::IncrementRatio ) {
      return inc_ratio_;
    } else if(option == settings::EntropyOption::NumericalGrad ) {
      return numerical_grad_;
    } else {
      return EntropyDecorator::get(option);
    }
  }

  bool NumericalGrad::set(const settings::EntropyOption option, std::any val) {
    if( option == settings::EntropyOption::IncrementRatio ) {
      if( std::type_index(val.type()) == std::type_index(typeid(double)) ) {
        inc_ratio_ = std::any_cast<double>(val);
      } else if( std::type_index(val.type()) == std::type_index(typeid(const double)) ) {
        inc_ratio_ = std::any_cast<const double>(val);
      } else {
        std::string error_msg = "Unsupported type encountered while attempting ";
        error_msg += "to set " + std::string(toString(option)) + ", supported types include.";
        error_msg += "\ndouble\nconst double";
        PANACEA_FAIL(error_msg);
      }
    } else if(option == settings::EntropyOption::NumericalGrad ) {
      if( std::type_index(val.type()) == std::type_index(typeid(bool)) ) {
        numerical_grad_ = std::any_cast<bool>(val);
      } else if( std::type_index(val.type()) == std::type_index(typeid(const bool)) ) {
        numerical_grad_ = std::any_cast<const bool>(val);
      } else {
        std::string error_msg = "Unsupported type encountered while attempting ";
        error_msg += "to set " + std::string(toString(option)) + ", supported types include.";
        error_msg += "\ndouble\nconst double";
        PANACEA_FAIL(error_msg);
      }
    } else {
      return EntropyDecorator::set(option,val);
    }
    return true;
  }

  std::vector<std::any> NumericalGrad::write(
      const settings::FileType file_type,
      std::ostream & os,
      const EntropyTerm & entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      os << "[Increment Ratio]\n";
      os << std::any_cast<double>(
          entropy_term_instance.get(
            settings::EntropyOption::IncrementRatio)) << "\n";
      os << "[Numerical Grad]\n";
      os << std::any_cast<bool>(
          entropy_term_instance.get(
            settings::EntropyOption::NumericalGrad)) << "\n";

    }

    return nested_values;

  }

  io::ReadInstantiateVector NumericalGrad::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm & entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ) {

      std::string line = "";
      while(line.find("[Increment Ratio]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading numerical grad decorator section of entropy term ";
          error_msg += "from restart file, file does not contain the [Increment Ratio] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      double inc_ratio;
      is >> inc_ratio;
      entropy_term_instance.set(settings::EntropyOption::IncrementRatio, inc_ratio);
      while(line.find("[Numerical Grad]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading numerical grad decorator section of entropy term ";
          error_msg += "from restart file, file does not contain the [Increment Ratio] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      bool numerical_grad;
      is >> numerical_grad;
      entropy_term_instance.set(settings::EntropyOption::NumericalGrad, numerical_grad);

    }

    return nested_values;
  }



}
