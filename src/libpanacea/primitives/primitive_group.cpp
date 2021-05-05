
// Local private includes
#include "primitive_group.hpp"

#include "attribute_manipulators/inverter.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "primitive_attributes.hpp"
#include "primitive_factory.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <memory>
#include <vector>

namespace panacea {

  PrimitiveAttributes PrimitiveGroup::createPrimitiveAttributes() noexcept {
    return PrimitiveAttributes {
    .normalizer = this->normalizer,
    .kernel_wrapper = this->kernel_wrapper.get(),
    .covariance = this->covariance.get(),
    .reduced_covariance = this->reduced_covariance.get(),
    .reduced_inv_covariance = this->reduced_inv_covariance.get()};
  }

  void PrimitiveGroup::update(
      const BaseDescriptorWrapper * dwrapper) {

    PrimitiveFactory prim_factory;
    prim_factory.update(PassKey<PrimitiveGroup>(),dwrapper, *this);
  }

  std::vector<std::any> PrimitiveGroup::write(
      const settings::FileType & file_type,
      std::ostream & os,
      std::any prim_grp_instance) {

    
    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      PrimitiveGroup * prim_grp;
      try {
        prim_grp = std::any_cast<PrimitiveGroup *>(prim_grp_instance);
      } catch (...) {
        std::string error_msg = "Unable to cast to PrimitiveGroup * while";
        error_msg += " trying to write primitive group to restart file.";
        PANACEA_FAIL(error_msg);
      }
      os << "[Primitive Group]\n";
      os << prim_grp->name << "\n";
      os << "\n";
      nested_values.push_back(&prim_grp->specification);
      nested_values.push_back(&prim_grp->normalizer);
      nested_values.push_back(prim_grp->kernel_wrapper.get());
      nested_values.push_back(prim_grp->covariance.get());
    }
    return nested_values;
  }

  std::vector<std::any> PrimitiveGroup::read(
      const settings::FileType & file_type,
      std::istream & is,
      std::any prim_grp_instance) {
    
    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTRestart ) {
      PrimitiveGroup * prim_grp;
      try {
        prim_grp = std::any_cast<PrimitiveGroup *>(prim_grp_instance);
      } catch (...) {
        std::string error_msg = "Unable to cast to PrimitiveGroup * while";
        error_msg += " trying to read primitive group from restart file.";
        PANACEA_FAIL(error_msg);
      }   

      std::string line = "";
      while(line.find("[Primitive Group]",0) == std::string::npos) {
        if( is.peek() == EOF ) {
          std::string error_msg = "Did not find [Primitive Group] header while trying ";
          error_msg += "to read in primitive group from restart file.";
          PANACEA_FAIL(error_msg);
        }
        std::getline(is, line);
      }
      is >> prim_grp->name;
      nested_values.push_back(&prim_grp->specification);
      nested_values.push_back(&prim_grp->normalizer);
      nested_values.push_back(prim_grp->kernel_wrapper.get());
      nested_values.push_back(prim_grp->covariance.get());
    }
    return nested_values;
  }

  void PrimitiveGroup::postReadInitialization(
      const settings::FileType & file_type,
      std::any prim_grp_instance) {
    
    if( file_type == settings::FileType::TXTRestart ) {
      PrimitiveGroup * prim_grp;
      try {
        prim_grp = std::any_cast<PrimitiveGroup *>(prim_grp_instance);
      } catch (...) {
        std::string error_msg = "Unable to cast to PrimitiveGroup * while";
        error_msg += " trying to run postReadInitialization on primitive ";
        error_msg += "group after reading restart file.";
        PANACEA_FAIL(error_msg);
      }   

      PrimitiveFactory prim_factory;
      prim_factory.reset(
          PassKey<PrimitiveGroup>(),
          *prim_grp);
    }
  }


}

