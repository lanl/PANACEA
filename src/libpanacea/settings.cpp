
// Private PANACEA includes
#include "private_settings.hpp"

// Standard includes
#include <type_traits>

/*
 * Simple class in charge of the settings used with panacea
 * this class should only store enums and or bool types
 */
namespace panacea {

  /************************************************************************
   * PANACEA Settings
   ************************************************************************/
  PANACEASettingsBuilder PANACEASettings::make() {
    return PANACEASettingsBuilder();
  }

  /************************************************************************
   * PANACEA Settings Builder methods
   ************************************************************************/
  PANACEASettingsBuilder & PANACEASettingsBuilder::distributionType(const settings::Kernel &) {
    ent_settings_.dist_type_ = settings::DistributionType::Kernel;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(
      const settings::EntropyType & ent_type) {

    ent_settings_.ent_type_ = ent_type;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(
      const settings::PANACEAAlgorithm & algorithm_setting) {

    ent_settings_.algorithm_setting_ = algorithm_setting;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::weightEntropTermBy(const double & weight) {
    ent_settings_.weight_ = weight;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::setIncrementRatioTo(const double & inc_ratio) {
    ent_settings_.inc_ratio_ = inc_ratio;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::setNumericalGradTo(const bool & on_or_off) {
    ent_settings_.numerical_grad_ = on_or_off;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(const settings::KernelPrimitive & primitive) {
    ent_settings_.primitive_ = primitive;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(const settings::KernelCount & kern_count) {
    ent_settings_.kern_count_ = kern_count;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(const settings::KernelCorrelation & kern_correlation) {
    ent_settings_.kern_correlation_ = kern_correlation;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(const settings::KernelCenterCalculation & kern_center) {
    ent_settings_.kern_center_ = kern_center;
    return *this;
  }

  PANACEASettingsBuilder & PANACEASettingsBuilder::set(const settings::KernelNormalization & kern_norm) {
    ent_settings_.kern_norm_ = kern_norm;
    return *this;
  }


}
