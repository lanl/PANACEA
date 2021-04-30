
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

  /******************************************************
   * Ostream enums
   ******************************************************/
  std::ostream& operator<<(std::ostream& os, const settings::DistributionType & dist_type) {
    if( dist_type == settings::DistributionType::Histogram ) {
      os << "Histogram";
    } else if (dist_type == settings::DistributionType::Kernel ) {
      os << "Kernel";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::FileType & file_type) {
    if( file_type == settings::FileType::TXTRestart ) {
      os << "TXTRestart";
    } else if( file_type == settings::FileType::TXTDescriptors) {
      os << "TXTDescriptors";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::EntropyOption & ent_opt) {
    if( ent_opt == settings::EntropyOption::Weight ) {
      os << "Weight";
    }else if(ent_opt == settings::EntropyOption::IncrementRatio ) {
      os << "IncrementRatio";
    }else if(ent_opt == settings::EntropyOption::NumericalGrad ){
      os << "NumericalGrad";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::EntropyType & ent_type) {
    if( ent_type == settings::EntropyType::Self){
      os << "Self";
    }else if( ent_type == settings::EntropyType::Cross){
      os << "Cross";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::PANACEAAlgorithm & alg) {
    if( alg == settings::PANACEAAlgorithm::Strict){
      os << "Strict";
    }else if( alg== settings::PANACEAAlgorithm::Flexible){
      os << "Flexible";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelPrimitive & kern_prim) {
    if( kern_prim == settings::KernelPrimitive::Gaussian ){
      os << "Gaussian";
    }else if(kern_prim == settings::KernelPrimitive::Exponential) {
      os << "Exponential";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelCount & count) {
    if( count == settings::KernelCount::OneToOne ) {
      os << "OneToOne";
    }else if( count == settings::KernelCount::Single ) {
      os << "Single";
    }else if(count == settings::KernelCount::Fixed){
      os << "Fixed";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelCorrelation & corr) {
    if( corr == settings::KernelCorrelation::Correlated){
      os << "Correlated";
    }else if(corr == settings::KernelCorrelation::Uncorrelated) {
      os << "Uncorrelated";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelNormalization & norm) {
    if( norm == settings::KernelNormalization::None){
      os << "None";
    }else if(norm == settings::KernelNormalization::Variance) {
      os << "Variance";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelCenterCalculation & cent) {
    if( cent == settings::KernelCenterCalculation::None ){
      os << "None";
    }else if(cent == settings::KernelCenterCalculation::Mean) {
      os << "Mean";
    }else if(cent == settings::KernelCenterCalculation::Median) {
      os << "Median";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::CalculationType & calc) {
    if(calc == settings::CalculationType::Numerical) {
      os << "Numerical";
    }else if(calc == settings::CalculationType::Analytical) {
      os << "Analytical";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::EquationSetting & eq_set) {
    if( eq_set == settings::EquationSetting::None) {
      os << "None";
    }else if(eq_set == settings::EquationSetting::IgnoreExp) {
      os << "IgnoreExp";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::None & none) {
    os << "None";
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::GradSetting & grad_set) {
    if(grad_set == settings::GradSetting::WRTDescriptor) {
      os << "WRTDescriptor";
    }else if(grad_set == settings::GradSetting::WRTKernel) {
      os << "WRTKernel";
    }else if(grad_set == settings::GradSetting::WRTBoth) {
      os << "WRTBoth";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelAlgorithm & kern_alg) {
    if( kern_alg == settings::KernelAlgorithm::Strict){
      os << "Strict";
    }else if( kern_alg == settings::KernelAlgorithm::Flexible){
      os << "Flexible";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const settings::KernelMemory & kern_mem) {
    if(kern_mem == settings::KernelMemory::Default) {
      os << "Default";
    }else if(kern_mem == settings::KernelMemory::Own) {
      os << "Own";
    }else if(kern_mem == settings::KernelMemory::Share) {
      os << "Share";
    } 
    return os;
  }

}
