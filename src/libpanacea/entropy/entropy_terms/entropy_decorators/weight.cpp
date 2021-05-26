// Local private PANACEA includes
#include "weight.hpp"

#include "error.hpp"

// Standard includes
#include <algorithm>
#include <any>
#include <typeindex>
#include <vector>

namespace panacea {

  double Weight::compute(
      const BaseDescriptorWrapper * descriptor_wrapper) {
    return EntropyDecorator::compute(descriptor_wrapper) * weight_;
  }

  double Weight::compute(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind) {
    return EntropyDecorator::compute(descriptor_wrapper, desc_ind) * weight_;
  }

  std::vector<double> Weight::compute_grad(
      const BaseDescriptorWrapper * descriptor_wrapper,
      const int desc_ind,
      const EntropySettings & entropy_settings) {

    auto vec = EntropyDecorator::compute_grad(descriptor_wrapper, desc_ind, entropy_settings);
    std::transform(vec.begin(), vec.end(), vec.begin(), std::bind(std::multiplies<double>(), std::placeholders::_1, weight_) );
    return vec;
  }

  void Weight::set(const settings::EntropyOption option, std::any val) {
    if( option == settings::EntropyOption::Weight ) {
      if( std::type_index(val.type()) == std::type_index(typeid(double))){
        weight_ = std::any_cast<double>(val);
      } else {
        weight_ = std::any_cast<const double>(val);
      }
    } else {
      EntropyDecorator::set(option, val);
    }
  }

  EntropyTerm::ReadFunction Weight::getReadFunction(const PassKey<EntropyTerm> &) {
    return read;
  }

  EntropyTerm::WriteFunction Weight::getWriteFunction(const PassKey<EntropyTerm> &) {
    return write;
  }

  std::vector<std::any> Weight::write(
      const settings::FileType file_type,
      std::ostream & os,
      EntropyTerm * entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      auto weight = dynamic_cast<Weight *>(entropy_term_instance);
      os << "[Weight]\n";
      os << weight->weight_ << "\n";
      nested_values = entropy_term_instance->getWriteFunction(weight->key)(file_type, os, entropy_term_instance);
    }

    return nested_values;
  }

  io::ReadInstantiateVector Weight::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm * entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      auto weight = dynamic_cast<Weight *>(entropy_term_instance);

      std::string line = "";
      while(line.find("[Weight]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading weight decorator section of entropy term ";
          error_msg += "from restart file, file does not contain the [Weight] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      is >> weight->weight_;

      nested_values = entropy_term_instance->getReadFunction(weight->key)(file_type, is, entropy_term_instance);
    }

    return nested_values;
  }


}


