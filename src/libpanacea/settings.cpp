
// Private PANACEA includes
#include "private_settings.hpp"

#include "error.hpp"

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
PANACEASettingsBuilder &
PANACEASettingsBuilder::distributionType(const settings::Kernel &) {
  ent_settings_.dist_type_ = settings::DistributionType::Kernel;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::set(const settings::EntropyType &ent_type) {

  ent_settings_.ent_type_ = ent_type;
  return *this;
}

PANACEASettingsBuilder &PANACEASettingsBuilder::set(
    const settings::PANACEAAlgorithm &algorithm_setting) {

  ent_settings_.algorithm_setting_ = algorithm_setting;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::weightEntropTermBy(const double &weight) {
  ent_settings_.weight_ = weight;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::setIncrementRatioTo(const double &inc_ratio) {
  ent_settings_.inc_ratio_ = inc_ratio;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::setNumericalGradTo(const bool &on_or_off) {
  ent_settings_.numerical_grad_ = on_or_off;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::set(const settings::KernelPrimitive &primitive) {
  ent_settings_.primitive_ = primitive;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::set(const settings::KernelCount &kern_count) {
  ent_settings_.kern_count_ = kern_count;
  return *this;
}

PANACEASettingsBuilder &PANACEASettingsBuilder::set(
    const settings::KernelCorrelation &kern_correlation) {
  ent_settings_.kern_correlation_ = kern_correlation;
  return *this;
}

PANACEASettingsBuilder &PANACEASettingsBuilder::set(
    const settings::KernelCenterCalculation &kern_center) {
  ent_settings_.kern_center_ = kern_center;
  return *this;
}

PANACEASettingsBuilder &
PANACEASettingsBuilder::set(const settings::KernelNormalization &kern_norm) {
  ent_settings_.kern_norm_ = kern_norm;
  return *this;
}

/******************************************************
 * Ostream enums
 ******************************************************/
std::ostream &operator<<(std::ostream &os,
                         const settings::DistributionType &dist_type) {
  if (dist_type == settings::DistributionType::Histogram) {
    os << "Histogram";
  } else if (dist_type == settings::DistributionType::Kernel) {
    os << "Kernel";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::FileType &file_type) {
  if (file_type == settings::FileType::TXTRestart) {
    os << "TXTRestart";
  } else if (file_type == settings::FileType::TXTDescriptors) {
    os << "TXTDescriptors";
  } else if (file_type == settings::FileType::TXTKernelDistribution) {
    os << "TXTKernelDistribution";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::EntropyOption &ent_opt) {
  if (ent_opt == settings::EntropyOption::Weight) {
    os << "Weight";
  } else if (ent_opt == settings::EntropyOption::IncrementRatio) {
    os << "IncrementRatio";
  } else if (ent_opt == settings::EntropyOption::NumericalGrad) {
    os << "NumericalGrad";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::EntropyType &ent_type) {
  if (ent_type == settings::EntropyType::Self) {
    os << "Self";
  } else if (ent_type == settings::EntropyType::Cross) {
    os << "Cross";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::PANACEAAlgorithm &alg) {
  if (alg == settings::PANACEAAlgorithm::Strict) {
    os << "Strict";
  } else if (alg == settings::PANACEAAlgorithm::Flexible) {
    os << "Flexible";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelPrimitive &kern_prim) {
  if (kern_prim == settings::KernelPrimitive::Gaussian) {
    os << "Gaussian";
  } else if (kern_prim == settings::KernelPrimitive::Exponential) {
    os << "Exponential";
  } else if (kern_prim == settings::KernelPrimitive::GaussianLog) {
    os << "GaussianLog";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const settings::KernelCount &count) {
  if (count == settings::KernelCount::OneToOne) {
    os << "OneToOne";
  } else if (count == settings::KernelCount::Single) {
    os << "Single";
  } else if (count == settings::KernelCount::Fixed) {
    os << "Fixed";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelCorrelation &corr) {
  if (corr == settings::KernelCorrelation::Correlated) {
    os << "Correlated";
  } else if (corr == settings::KernelCorrelation::Uncorrelated) {
    os << "Uncorrelated";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelNormalization &norm) {
  if (norm == settings::KernelNormalization::None) {
    os << "None";
  } else if (norm == settings::KernelNormalization::Variance) {
    os << "Variance";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelCenterCalculation &cent) {
  if (cent == settings::KernelCenterCalculation::None) {
    os << "None";
  } else if (cent == settings::KernelCenterCalculation::Mean) {
    os << "Mean";
  } else if (cent == settings::KernelCenterCalculation::Median) {
    os << "Median";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::CalculationType &calc) {
  if (calc == settings::CalculationType::Numerical) {
    os << "Numerical";
  } else if (calc == settings::CalculationType::Analytical) {
    os << "Analytical";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::EquationSetting &eq_set) {
  if (eq_set == settings::EquationSetting::None) {
    os << "None";
  } else if (eq_set == settings::EquationSetting::IgnoreExp) {
    os << "IgnoreExp";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os, const settings::None &none) {
  os << "None";
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::GradSetting &grad_set) {
  if (grad_set == settings::GradSetting::WRTDescriptor) {
    os << "WRTDescriptor";
  } else if (grad_set == settings::GradSetting::WRTKernel) {
    os << "WRTKernel";
  } else if (grad_set == settings::GradSetting::WRTBoth) {
    os << "WRTBoth";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelAlgorithm &kern_alg) {
  if (kern_alg == settings::KernelAlgorithm::Strict) {
    os << "Strict";
  } else if (kern_alg == settings::KernelAlgorithm::Flexible) {
    os << "Flexible";
  }
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         const settings::KernelMemory &kern_mem) {
  if (kern_mem == settings::KernelMemory::Default) {
    os << "Default";
  } else if (kern_mem == settings::KernelMemory::Own) {
    os << "Own";
  } else if (kern_mem == settings::KernelMemory::OwnIfRestart) {
    os << "OwnIfRestart";
  } else if (kern_mem == settings::KernelMemory::Share) {
    os << "Share";
  }
  return os;
}

/***************************************************
 * Istream enums
 ***************************************************/

std::istream &operator>>(std::istream &is,
                         settings::DistributionType &dist_type) {
  std::string line;
  std::getline(is, line);
  if (line.find("Histogram", 0) != std::string::npos) {
    dist_type = settings::DistributionType::Histogram;
  } else if (line.find("Kernel", 0) != std::string::npos) {
    dist_type = settings::DistributionType::Kernel;
  } else {
    std::string error_msg =
        "Unrecognized distribution type while reading istream.\n";
    error_msg += "Accepted distribution types are:\n";
    error_msg += "Histogram\nKernel\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::FileType &file_type) {
  std::string line;
  std::getline(is, line);
  if (line.find("TXTRestart", 0) != std::string::npos) {
    file_type = settings::FileType::TXTRestart;
  } else if (line.find("TXTDescriptors", 0) != std::string::npos) {
    file_type = settings::FileType::TXTDescriptors;
  } else if (line.find("TXTKernelDistribution", 0) != std::string::npos) {
    file_type = settings::FileType::TXTKernelDistribution;
  } else {
    std::string error_msg = "Unrecognized file type while reading istream.\n";
    error_msg += "Accepted file types are:\n";
    error_msg += "TXTRestart\nTXTDescriptors\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::EntropyOption &ent_opt) {
  std::string line;
  std::getline(is, line);
  if (line.find("Weight", 0) != std::string::npos) {
    ent_opt = settings::EntropyOption::Weight;
  } else if (line.find("IncrementRatio", 0) != std::string::npos) {
    ent_opt = settings::EntropyOption::IncrementRatio;
  } else if (line.find("NumericalGrad", 0) != std::string::npos) {
    ent_opt = settings::EntropyOption::NumericalGrad;
  } else {
    std::string error_msg =
        "Unrecognized entropy option while reading istream.\n";
    error_msg += "Accepted entropy options are:\n";
    error_msg += "Weight\nIncrementRatio\nNumericalGrad\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::EntropyType &ent_type) {
  std::string line;
  std::getline(is, line);
  if (line.find("Self", 0) != std::string::npos) {
    ent_type = settings::EntropyType::Self;
  } else if (line.find("Cross", 0) != std::string::npos) {
    ent_type = settings::EntropyType::Cross;
  } else {
    std::string error_msg =
        "Unrecognized entropy type while reading istream.\n";
    error_msg += "Accepted entropy types are:\n";
    error_msg += "Self\nCross\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::PANACEAAlgorithm &alg) {
  std::string line;
  std::getline(is, line);
  if (line.find("Strict", 0) != std::string::npos) {
    alg = settings::PANACEAAlgorithm::Strict;
  } else if (line.find("Flexible", 0) != std::string::npos) {
    alg = settings::PANACEAAlgorithm::Flexible;
  } else {
    std::string error_msg =
        "Unrecognized algorithm setting while reading istream.\n";
    error_msg += "Accepted algorithm settings are:\n";
    error_msg += "Strict\nFlexible\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is,
                         settings::KernelPrimitive &kern_prim) {
  std::string line;
  std::getline(is, line);
  if (line.find("Gaussian", 0) != std::string::npos) {
    kern_prim = settings::KernelPrimitive::Gaussian;
  } else if (line.find("Exponential", 0) != std::string::npos) {
    kern_prim = settings::KernelPrimitive::Exponential;
  } else if (line.find("GaussianLog", 0) != std::string::npos) {
    kern_prim = settings::KernelPrimitive::GaussianLog;
  } else {
    std::string error_msg =
        "Unrecognized kernel primitive while reading istream.\n";
    error_msg += "Accepted kernel primitive types are:\n";
    error_msg += "Gaussian\nExponential\nGaussianLog";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::KernelCount &count) {
  std::string line;
  std::getline(is, line);
  if (line.find("OneToOne", 0) != std::string::npos) {
    count = settings::KernelCount::OneToOne;
  } else if (line.find("Single", 0) != std::string::npos) {
    count = settings::KernelCount::Single;
  } else if (line.find("Fixed", 0) != std::string::npos) {
    count = settings::KernelCount::Fixed;
  } else {
    std::string error_msg =
        "Unrecognized kernel count setting while reading istream.\n";
    error_msg += "Accepted kernel types are:\n";
    error_msg += "OneToOne\nSingle\nFixed\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::KernelCorrelation &corr) {
  std::string line;
  std::getline(is, line);
  if (line.find("Correlated", 0) != std::string::npos) {
    corr = settings::KernelCorrelation::Correlated;
  } else if (line.find("Uncorrelated", 0) != std::string::npos) {
    corr = settings::KernelCorrelation::Uncorrelated;
  } else {
    std::string error_msg =
        "Unrecognized kernel correlation while reading istream.\n";
    error_msg += "Accepted kernel correlation types are:\n";
    error_msg += "Correlated\nUncorrelated\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is,
                         settings::KernelNormalization &norm) {
  std::string line;
  std::getline(is, line);
  if (line.find("None", 0) != std::string::npos) {
    norm = settings::KernelNormalization::None;
  } else if (line.find("Variance", 0) != std::string::npos) {
    norm = settings::KernelNormalization::Variance;
  } else {
    std::string error_msg = "Unrecognized kernel normalization method while ";
    error_msg += "reading istream.\n";
    error_msg += "Accepted kernel normalization method types are:\n";
    error_msg += "None\nVariance\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is,
                         settings::KernelCenterCalculation &cent) {
  std::string line;
  std::getline(is, line);
  if (line.find("None", 0) != std::string::npos) {
    cent = settings::KernelCenterCalculation::None;
  } else if (line.find("Mean", 0) != std::string::npos) {
    cent = settings::KernelCenterCalculation::Mean;
  } else if (line.find("Median", 0) != std::string::npos) {
    cent = settings::KernelCenterCalculation::Median;
  } else {
    std::string error_msg =
        "Unrecognized kernel center calculation type while ";
    error_msg += "reading istream.\n";
    error_msg += "Accepted kernel center calculation types are:\n";
    error_msg += "None\nMean\nMedian\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::CalculationType &calc) {
  std::string line;
  std::getline(is, line);
  if (line.find("Numerical", 0) != std::string::npos) {
    calc = settings::CalculationType::Numerical;
  } else if (line.find("Analytical", 0) != std::string::npos) {
    calc = settings::CalculationType::Analytical;
  } else {
    std::string error_msg =
        "Unrecognized calculation type while reading istream.\n";
    error_msg += "Accepted calculation types are:\n";
    error_msg += "Numerical\nAnalytical\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::EquationSetting &eq_set) {
  std::string line;
  std::getline(is, line);
  if (line.find("None", 0) != std::string::npos) {
    eq_set = settings::EquationSetting::None;
  } else if (line.find("IgnoreExp", 0) != std::string::npos) {
    eq_set = settings::EquationSetting::IgnoreExp;
  } else {
    std::string error_msg =
        "Unrecognized equation setting while reading istream.\n";
    error_msg += "Accepted equation settings are:\n";
    error_msg += "None\nIgnoreExp\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::None &none) {
  std::string line;
  std::getline(is, line);
  if (line.find("None", 0) == std::string::npos) {
    std::string error_msg =
        "Unrecognized none setting while reading istream.\n";
    error_msg += "Accepted settings are:\n";
    error_msg += "None\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }
  none = settings::None::None;

  return is;
}

std::istream &operator>>(std::istream &is, settings::GradSetting &grad_set) {
  std::string line;
  std::getline(is, line);
  if (line.find("WRTDescriptor", 0) != std::string::npos) {
    grad_set = settings::GradSetting::WRTDescriptor;
  } else if (line.find("WRTKernel", 0) != std::string::npos) {
    grad_set = settings::GradSetting::WRTKernel;
  } else if (line.find("WRTBoth", 0) != std::string::npos) {
    grad_set = settings::GradSetting::WRTBoth;
  } else {
    std::string error_msg =
        "Unrecognized gradiant setting while reading istream.\n";
    error_msg += "Accepted gradiant settings are:\n";
    error_msg += "WRTBoth\nWRTDescriptor\nWRTKernel\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is,
                         settings::KernelAlgorithm &kern_alg) {
  std::string line;
  std::getline(is, line);
  if (line.find("Strict", 0) != std::string::npos) {
    kern_alg = settings::KernelAlgorithm::Strict;
  } else if (line.find("Flexible", 0) != std::string::npos) {
    kern_alg = settings::KernelAlgorithm::Flexible;
  } else {
    std::string error_msg =
        "Unrecognized kernel algorithm setting while reading istream.\n";
    error_msg += "Accepted kernel algorithm settings are:\n";
    error_msg += "Strict\nFlexible\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

std::istream &operator>>(std::istream &is, settings::KernelMemory &kern_mem) {
  std::string line;
  std::getline(is, line);
  if (line.find("Default", 0) != std::string::npos) {
    kern_mem = settings::KernelMemory::Default;
  } else if (line.find("OwnIfRestart", 0) != std::string::npos) {
    kern_mem = settings::KernelMemory::OwnIfRestart;
  } else if (line.find("Own", 0) != std::string::npos) {
    kern_mem = settings::KernelMemory::Own;
  } else if (line.find("Share", 0) != std::string::npos) {
    kern_mem = settings::KernelMemory::Share;
  } else {
    std::string error_msg =
        "Unrecognized kernek memory option while reading istream.\n";
    error_msg += "Accepted kernel memory options are:\n";
    error_msg += "Default\nOwnIfRestart\nOwn\nShare\n";
    error_msg += "Line is: " + line + "\n";
    PANACEA_FAIL(error_msg);
  }

  return is;
}

} // namespace panacea
