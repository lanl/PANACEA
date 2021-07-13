// Local private PANACEA includes
#include "weight.hpp"

#include "numerical_grad.hpp"
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
    return EntropyDecorator::compute(descriptor_wrapper, entropy_settings) * weight_; }

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

  std::any Weight::get(const settings::EntropyOption option) const {
    if( option == settings::EntropyOption::Weight ) {
      return weight_;
    } else {
      return EntropyDecorator::get(option);
    }
  }

  bool Weight::set(const settings::EntropyOption option, std::any val) {
    if( option == settings::EntropyOption::Weight ) {
      if( std::type_index(val.type()) == std::type_index(typeid(const double)) ){
        weight_ = std::any_cast<const double>(val);
      } else if(std::type_index(val.type()) == std::type_index(typeid(double)) ) {
        weight_ = std::any_cast<double>(val);
      } else {
        std::string error_msg = "Unable to set " + std::string(toString(option));
        error_msg += " value type is inapprorpiate. Supported types include:";
        error_msg += "\nconst double\ndouble";
        PANACEA_FAIL(error_msg);
      }
    } else {
      return EntropyDecorator::set(option, val);
    }
    return true;
  }

  std::vector<EntropyTerm::ReadElement> Weight::getReadFunction(const PassKey<EntropyTerm> &) {
    std::vector<EntropyTerm::ReadElement> read_functions = EntropyDecorator::getReadFunction(key);
    read_functions.emplace_back(read, *this);
    return read_functions;
  }

  std::vector<EntropyTerm::WriteElement> Weight::getWriteFunction(const PassKey<EntropyTerm> &) const {
    std::vector<EntropyTerm::WriteElement> write_functions = EntropyDecorator::getWriteFunction(key);
    write_functions.emplace_back(write, *this);
    return write_functions;
  }

  std::vector<std::any> Weight::write(
      const settings::FileType file_type,
      std::ostream & os,
      const EntropyTerm & entropy_term_instance) {

    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      os << "[Weight]\n";
      os << std::any_cast<double>(
          entropy_term_instance.get(
            settings::EntropyOption::Weight)) << "\n";
    }
    return nested_values;
  }

  io::ReadInstantiateVector Weight::read(
      const settings::FileType file_type,
      std::istream & is,
      EntropyTerm & entropy_term_instance) {

    io::ReadInstantiateVector nested_values;
    if( file_type == settings::FileType::TXTRestart ) {

      std::string line = "";
      while(line.find("[Weight]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "While reading weight decorator section of entropy term ";
          error_msg += "from restart file, file does not contain the [Weight] tag.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }

      double weight;
      is >> weight;
      entropy_term_instance.set(settings::EntropyOption::Weight, weight);

    }

    return nested_values;
  }

}


