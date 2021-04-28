#ifndef PANACEA_SETTINGS_H
#define PANACEA_SETTINGS_H
#pragma once

// Standard includes
#include <type_traits>

/*
 * Simple class in charge of the settings used with panacea
 * this class should only store enums and or bool types
 */
namespace panacea {

  namespace settings {
  
    class Kernel {};

    enum class EntropyOption {
      Weight,
      IncrementRatio, // Used in numerical gradiant calculations
      NumericalGrad   // Turn numerical gradiant on or off 
    };

    enum class EntropyType {
      Self,
      Cross
    };

    /** 
     * Fail if the specified kernel specifications are not satisfied
     * the algorithm flexibility in determining best options if the 
     * ones specefied lead to non ideal conditions, i.e. if defining
     * a covariance matrix for a kernel in which the data is stacked
     * at a single point then the covariance matrix will become 0
     * everywhere which will lead to numerical problems.
     **/
    enum class PANACEAAlgorithm {
      Strict,   
      Flexible  
    };

    enum class KernelPrimitive {
      Gaussian,
      Exponential    
    };

    /**
     * How the descriptors are represented by the kernel
     * Single - a single kernel is used to represent all the descriptors
     * OneToOne - each descriptor has it's own kernel
     * Fixed - there are a fixed number of kernels
     **/
    enum class KernelCount {
      Single,
      OneToOne,
      Fixed
    };

    enum class KernelCorrelation {
      Correlated,
      Uncorrelated
    };

    enum class KernelNormalization {
      None,
      Variance
    };

    enum class KernelCenterCalculation {
      None,
      Mean,
      Median,
      Custom
    };

  }

  class PANACEASettingsBuilder;

  class PANACEASettings {
 
      friend class KernelBuilder; 
      friend class PANACEASettingsBuilder;  

    public:
      static PANACEASettingsBuilder make();
    private:
      PANACEASettings() = default;
      settings::EntropyType ent_type = settings::EntropyType::Self;
      settings::EntropyType ent_type_ = settings::EntropyType::Self; 
      settings::PANACEAAlgorithm algorithm_setting_ = settings::PANACEAAlgorithm::Strict;
      settings::KernelPrimitive primitive_ = settings::KernelPrimitive::Gaussian;
      settings::KernelCount kern_count_ = settings::KernelCount::OneToOne;
      settings::KernelCorrelation kern_correlation_ = settings::KernelCorrelation::Uncorrelated;
      settings::KernelNormalization kern_norm_ = settings::KernelNormalization::None;
      settings::KernelCenterCalculation kern_center_ = settings::KernelCenterCalculation::None;
      double weight_ = 1.0;
      double inc_ratio_ = 1E-4;
      bool numerical_grad_ = false;
    };

  class KernelBuilder;

  class PANACEASettingsBuilder {
    public:
      KernelBuilder distributionType(const settings::Kernel &);

      PANACEASettingsBuilder & set(const settings::EntropyType &);
      PANACEASettingsBuilder & set(const settings::PANACEAAlgorithm &);
    private:
      PANACEASettings ent_settings_;
  };

  class KernelBuilder : public PANACEASettingsBuilder {
    public:
      explicit KernelBuilder(PANACEASettings & settings) : ent_settings_(settings) {};
      KernelBuilder & weightEntropTermBy(const double & weight);
      KernelBuilder & setIncrementRatioTo(const double & inc_ratio);
      KernelBuilder & setNumericalGradTo(const bool & on_or_off);
      KernelBuilder & set(const settings::KernelPrimitive &);
      KernelBuilder & set(const settings::KernelCount &);
      KernelBuilder & set(const settings::KernelCorrelation &);
      KernelBuilder & set(const settings::KernelCenterCalculation &);
      KernelBuilder & set(const settings::KernelNormalization &);

      operator PANACEASettings&&() {
           return std::move(ent_settings_); // notice the move
       }

    private:
      PANACEASettings ent_settings_;
  };

}
#endif // PANACEA_SETTINGS_H
