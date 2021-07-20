#ifndef PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
#define PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
#pragma once

// Local private PANACEA includes
#include "private_settings.hpp"

#include "constants.hpp"
#include "error.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>

namespace panacea {

namespace defaults {
const ::panacea::settings::KernelCorrelation kern_correlation_default =
    ::panacea::settings::KernelCorrelation::Correlated;
const ::panacea::settings::KernelCount kern_count_default =
    ::panacea::settings::KernelCount::Single;
const ::panacea::settings::KernelPrimitive kern_primitive_default =
    ::panacea::settings::KernelPrimitive::Gaussian;
const ::panacea::settings::KernelNormalization kern_normalization_default =
    ::panacea::settings::KernelNormalization::Variance;
const ::panacea::settings::KernelMemory kern_memory_default =
    ::panacea::settings::KernelMemory::Own;
const ::panacea::settings::KernelCenterCalculation kern_center_default =
    ::panacea::settings::KernelCenterCalculation::Mean;
const ::panacea::settings::KernelAlgorithm kern_algorithm_default =
    ::panacea::settings::KernelAlgorithm::Strict;
const ::panacea::settings::RandomizeDimensions randomize_dims_default =
    ::panacea::settings::RandomizeDimensions::No;
const ::panacea::settings::RandomizeNumberDimensions
    randomize_num_dims_default =
        ::panacea::settings::RandomizeNumberDimensions::No;
} // namespace defaults

class KernelSpecification {
  settings::KernelCorrelation kern_correlation_ =
      defaults::kern_correlation_default;
  settings::KernelCount kern_count_ = defaults::kern_count_default;
  settings::KernelPrimitive kern_prim_ = defaults::kern_primitive_default;
  settings::KernelNormalization kern_normalization_ =
      defaults::kern_normalization_default;
  settings::KernelMemory kern_memory_ = defaults::kern_memory_default;
  settings::KernelCenterCalculation kern_center_ =
      defaults::kern_center_default;
  settings::KernelAlgorithm kern_algorithm_ = defaults::kern_algorithm_default;

  settings::RandomizeDimensions randomize_dims_ =
      defaults::randomize_dims_default;
  settings::RandomizeNumberDimensions randomize_num_dims_ =
      defaults::randomize_num_dims_default;
  int max_number_dimensions_ = constants::automate;

public:
  KernelSpecification() = default;

  KernelSpecification(
      const settings::KernelCorrelation kern_correlation,
      const settings::KernelCount kern_count,
      const settings::KernelPrimitive kern_prim,
      const settings::KernelNormalization kern_normalization,
      const settings::KernelMemory kern_memory,
      const settings::KernelCenterCalculation kern_center,
      const settings::KernelAlgorithm kern_algorithm,
      const settings::RandomizeDimensions randomize_dims,
      const settings::RandomizeNumberDimensions randomize_num_dims,
      const int max_num_dims = constants::automate)
      : kern_correlation_(kern_correlation), kern_count_(kern_count),
        kern_prim_(kern_prim), kern_normalization_(kern_normalization),
        kern_memory_(kern_memory), kern_center_(kern_center),
        kern_algorithm_(kern_algorithm), randomize_dims_(randomize_dims),
        randomize_num_dims_(randomize_num_dims),
        max_number_dimensions_(max_num_dims) {
    if (max_num_dims < -1 or max_num_dims == 0) {
      std::string error_msg =
          "Problematic kernel specifications the max number of ";
      error_msg +=
          "allowed dimensions must be a positive number or it must be -1 to ";
      error_msg +=
          "indicate that the dimensions will automatically be determined. ";
      error_msg +=
          "\nYou have provided a value of: " + std::to_string(max_num_dims);
      PANACEA_FAIL(error_msg);
    }
  };

  friend bool operator==(const KernelSpecification &spec1,
                         const KernelSpecification &spec2);
  friend bool operator!=(const KernelSpecification &spec1,
                         const KernelSpecification &spec2);

  template <class T> inline T get() const {
    if constexpr (std::is_same<T, settings::KernelCorrelation>::value) {
      return kern_correlation_;
    } else if constexpr (std::is_same<T, settings::KernelCount>::value) {
      return kern_count_;
    } else if constexpr (std::is_same<T, settings::KernelPrimitive>::value) {
      return kern_prim_;
    } else if constexpr (std::is_same<T,
                                      settings::KernelNormalization>::value) {
      return kern_normalization_;
    } else if constexpr (std::is_same<T, settings::KernelMemory>::value) {
      return kern_memory_;
    } else if constexpr (std::is_same<
                             T, settings::KernelCenterCalculation>::value) {
      return kern_center_;
    } else if constexpr (std::is_same<T, settings::KernelAlgorithm>::value) {
      return kern_algorithm_;
    } else if constexpr (std::is_same<T,
                                      settings::RandomizeDimensions>::value) {
      return randomize_dims_;
    } else if constexpr (std::is_same<
                             T, settings::RandomizeNumberDimensions>::value) {
      return randomize_num_dims_;
    } else if constexpr (std::is_same<T, std::string>::value) {
      std::stringstream string_spec("Kernel ");
      string_spec << settings::toString(kern_correlation_);
      string_spec << ", " << settings::toString(kern_count_);
      string_spec << ", " << settings::toString(kern_prim_);
      string_spec << ", " << settings::toString(kern_normalization_);
      string_spec << ", " << settings::toString(kern_memory_);
      string_spec << ", " << settings::toString(kern_center_);
      string_spec << ", " << settings::toString(kern_algorithm_);
      string_spec << ", " << settings::toString(randomize_dims_);
      string_spec << ", " << settings::toString(randomize_num_dims_);
      string_spec << ", " << max_number_dimensions_;
      return string_spec.str();
    }
  }

  int getMaxNumberDimensions() const noexcept { return max_number_dimensions_; }
  void setMaxNumberDimensions(const int dims) noexcept {
    max_number_dimensions_ = dims;
  }

  inline bool is(const settings::KernelCorrelation correlation) const noexcept {
    if (correlation == kern_correlation_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelCount kern_count) const noexcept {
    if (kern_count == kern_count_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelPrimitive kern_prim) const noexcept {
    if (kern_prim == kern_prim_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelNormalization kern_normalization) const
      noexcept {
    if (kern_normalization == kern_normalization_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelMemory kern_memory) const noexcept {
    if (kern_memory == kern_memory_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelCenterCalculation kern_center) const
      noexcept {
    if (kern_center == kern_center_)
      return true;
    return false;
  }

  inline bool is(const settings::KernelAlgorithm kern_algorithm) const
      noexcept {
    if (kern_algorithm == kern_algorithm_)
      return true;
    return false;
  }

  inline bool is(const settings::RandomizeDimensions rand_dims) const noexcept {
    if (rand_dims == randomize_dims_)
      return true;
    return false;
  }

  inline bool is(const settings::RandomizeNumberDimensions rand_num_dims) const
      noexcept {
    if (rand_num_dims == randomize_num_dims_)
      return true;
    return false;
  }

  /**
   * Define setters
   **/
  inline void set(const settings::KernelCorrelation correlation) noexcept {
    kern_correlation_ = correlation;
  }

  inline void set(const settings::KernelCount kern_count) noexcept {
    kern_count_ = kern_count;
  }

  inline void set(const settings::KernelPrimitive kern_prim) noexcept {
    kern_prim_ = kern_prim;
  }

  inline void
  set(const settings::KernelNormalization kern_normalization) noexcept {
    kern_normalization_ = kern_normalization;
  }

  inline void set(const settings::KernelMemory kern_memory) noexcept {
    kern_memory_ = kern_memory;
  }

  inline void
  set(const settings::KernelCenterCalculation kern_center) noexcept {
    kern_center_ = kern_center;
  }

  inline void set(const settings::KernelAlgorithm kern_algorithm) noexcept {
    kern_algorithm_ = kern_algorithm;
  }

  inline void set(const settings::RandomizeDimensions rand_dims) noexcept {
    randomize_dims_ = rand_dims;
  }

  inline void
  set(const settings::RandomizeNumberDimensions rand_num_dims) noexcept {
    randomize_num_dims_ = rand_num_dims;
  }

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &,
                                     std::any kern_spec_instance);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &,
                                        std::any kern_spec_instance);
};

bool operator==(const KernelSpecification &spec1,
                const KernelSpecification &spec2);
bool operator!=(const KernelSpecification &spec1,
                const KernelSpecification &spec2);
bool operator==(const std::unique_ptr<KernelSpecification> &spec1,
                const std::unique_ptr<KernelSpecification> &spec2);
bool operator!=(const std::unique_ptr<KernelSpecification> &spec1,
                const std::unique_ptr<KernelSpecification> &spec2);
} // namespace panacea
#endif // PANACEA_PRIVATE_KERNEL_SPECIFICATIONS
