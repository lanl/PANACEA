// Local private PANACEA includes
#include "weight.hpp"

#include "entropy/entropy_settings/entropy_settings.hpp"
#include "error.hpp"

// Standard includes
#include <algorithm>
#include <any>
#include <iostream>
#include <typeindex>
#include <vector>

namespace panacea {

  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper) {
    return EntropyDecorator::compute(descriptor_wrapper) * weight_;
  }

  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind) {
    return EntropyDecorator::compute(descriptor_wrapper, desc_ind) * weight_;
  }

  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const EntropySettings & entropy_settings) {
    return EntropyDecorator::compute(descriptor_wrapper, entropy_settings) * weight_;
  }

  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings) {
    return EntropyDecorator::compute(descriptor_wrapper, desc_ind, entropy_settings) * weight_;
  }

  /**
   * Note we do not multiply by the weight here because we are passing that baton
   * to another Decorator method where the weight is accounted for.
   **/
  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const PANACEASettings & panacea_settings) {
    return Weight::compute(descriptor_wrapper, EntropySettings(panacea_settings));
  }

  /**
   * Note we do not multiply by the weight here because we are passing that baton
   * to another Decorator method where the weight is accounted for.
   **/
  double Weight::compute(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const PANACEASettings & panacea_settings) {
    return Weight::compute(descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  std::vector<double> Weight::compute_grad(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind) {

    auto vec = EntropyDecorator::compute_grad(descriptor_wrapper, desc_ind);
    std::transform(vec.begin(), vec.end(), vec.begin(), std::bind(std::multiplies<double>(), std::placeholders::_1, weight_) );
    return vec;
  }

  std::vector<double> Weight::compute_grad(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings) {

    auto vec = EntropyDecorator::compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
    std::transform(vec.begin(), vec.end(), vec.begin(), std::bind(std::multiplies<double>(), std::placeholders::_1, weight_) );
    return vec;
  }

  std::vector<double> Weight::compute_grad(
      const BaseDescriptorWrapper & descriptor_wrapper,
      const int desc_ind,
      const PANACEASettings & panacea_settings) {
    return Weight::compute_grad(descriptor_wrapper, desc_ind, EntropySettings(panacea_settings));
  }

  void Weight::set(const settings::EntropyOption option, std::any val) {
    if( option == settings::EntropyOption::Weight ) {
      if( std::type_index(val.type()) == std::type_index(typeid(double))){
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
        weight_ = std::any_cast<double>(val);
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      } else {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
        weight_ = std::any_cast<const double>(val);
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      }
    } else {
      EntropyDecorator::set(option, val);
    }
  }

  EntropyTerm::ReadFunction Weight::getReadFunction(const PassKey<EntropyTerm> &) {
    return read;
  }

  EntropyTerm::WriteFunction Weight::getWriteFunction(const PassKey<EntropyTerm> &) const {
    return write;
  }

  std::vector<std::any> Weight::write(
      const settings::FileType file_type,
      std::ostream & os,
      const EntropyTerm & entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      std::cout << "Casting from const EntropyTerm & to const Weight &" << std::endl;
      // Use a pointer here because it won't throw and so we can check if it was valid
      // or not
      const Weight * weight_ptr = dynamic_cast<const Weight *>(&entropy_term_instance);
      if( weight_ptr == nullptr) {
        weight_ptr = dynamic_cast<const Weight *>(&entropy_term_instance);
        std::cout << "Problem casting" << std::endl;
      }
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      os << "[Weight]\n";
      os << weight_ptr->weight_ << "\n";
      nested_values = weight_ptr->entropy_term_->getWriteFunction(weight_ptr->key)(file_type, os, *weight_ptr->entropy_term_);
    }
    return nested_values;
  }

  io::ReadInstantiateVector Weight::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm & entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      std::cout << "Casting from EntropyTerm & to Weight &" << std::endl;
      auto & weight = dynamic_cast<Weight &>(entropy_term_instance);
        std::cout << __FILE__ << ":" << __LINE__ << std::endl;

      std::string line = "";
      while(line.find("[Weight]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading weight decorator section of entropy term ";
          error_msg += "from restart file, file does not contain the [Weight] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      is >> weight.weight_;

      nested_values = weight.entropy_term_->getReadFunction(weight.key)(file_type, is, *weight.entropy_term_);
    }

    return nested_values;
  }


}


