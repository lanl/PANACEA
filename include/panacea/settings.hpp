#ifndef PANACEA_SETTINGS_H
#define PANACEA_SETTINGS_H
#pragma once

// Standard includes
#include <optional>
#include <type_traits>

/*
 * Simple class in charge of the settings used with panacea
 * this class should only store enums and or bool types
 */
namespace panacea {

  namespace settings {
  
    enum class DistributionType {
      Histogram,
      Kernel
    };

    class Kernel { };
    const Kernel kernel = Kernel();

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

  /**
   * PANACEASettings designed to be used with the fluent builder pattern
   **/
  class PANACEASettings {
 
      friend class PANACEASettingsBuilder;  

    public:
      static PANACEASettingsBuilder make();
      std::optional<double> getWeight() const noexcept {
        return weight_;
      }

      std::optional<double> getIncrement() const noexcept {
        return inc_ratio_;
      }

      std::optional<bool> numericalGrad() const noexcept {
        return numerical_grad_;
      }

      template<class T>
      std::optional<T> get() const noexcept {
        if constexpr( std::is_same<settings::EntropyType,T>::value ) {
          return ent_type_;
        }else if constexpr(std::is_same<settings::PANACEAAlgorithm,T>::value) {
          return algorithm_setting_;
        }else if constexpr(std::is_same<settings::KernelPrimitive,T>::value) {
          return primitive_;
        }else if constexpr(std::is_same<settings::KernelCount,T>::value) {
          return kern_count_;
        }else if constexpr(std::is_same<settings::KernelCorrelation,T>::value) {
          return kern_correlation_;
        }else if constexpr(std::is_same<settings::KernelNormalization,T>::value) {
          return kern_norm_;
        }else if constexpr(std::is_same<settings::KernelCenterCalculation,T>::value) {
          return kern_center_;
        }else if constexpr(std::is_same<settings::DistributionType,T>::value) {
          return dist_type_;
        }
      }


    private:
      PANACEASettings() = default;
      std::optional<settings::EntropyType> ent_type_; 
      std::optional<settings::PANACEAAlgorithm> algorithm_setting_;
      std::optional<settings::KernelPrimitive> primitive_;
      std::optional<settings::KernelCount> kern_count_;
      std::optional<settings::KernelCorrelation> kern_correlation_;
      std::optional<settings::KernelNormalization> kern_norm_;
      std::optional<settings::KernelCenterCalculation> kern_center_;
      std::optional<settings::DistributionType> dist_type_;
      std::optional<double> weight_;
      std::optional<double> inc_ratio_;
      std::optional<bool> numerical_grad_;
    };

  class PANACEASettingsBuilder {
    public:
      PANACEASettingsBuilder & distributionType(const settings::Kernel &);

      PANACEASettingsBuilder & set(const settings::EntropyType &);
      PANACEASettingsBuilder & set(const settings::PANACEAAlgorithm &);

      PANACEASettingsBuilder & weightEntropTermBy(const double & weight);
      PANACEASettingsBuilder & setIncrementRatioTo(const double & inc_ratio);
      PANACEASettingsBuilder & setNumericalGradTo(const bool & on_or_off);
      PANACEASettingsBuilder & set(const settings::KernelPrimitive &);
      PANACEASettingsBuilder & set(const settings::KernelCount &);
      PANACEASettingsBuilder & set(const settings::KernelCorrelation &);
      PANACEASettingsBuilder & set(const settings::KernelCenterCalculation &);
      PANACEASettingsBuilder & set(const settings::KernelNormalization &);

      operator PANACEASettings&&() {
           return std::move(ent_settings_); // notice the move
       }

    protected:
      PANACEASettings ent_settings_;
  };
}
#endif // PANACEA_SETTINGS_H
