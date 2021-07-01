
#ifndef PANACEA_PRIVATE_NORMALIZER_H
#define PANACEA_PRIVATE_NORMALIZER_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"

#include "normalization_methods/normalization_method_factory.hpp"
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <optional>
#include <vector>

namespace panacea {

  namespace settings {
    enum class KernelNormalization;
  }
  /**
   * If 0.0 are encountered:
   * Flexible - will set normalization constant to 1.0
   * Strict - will set to 0.0
   **/
  enum class NormalizerOption {
    Flexible,
    Strict
  };

  class BaseDescriptorWrapper;

  /*
   * The only thing that gets normalized and unnormalized is
   * the covariance matrix.
   *
   * It would be unnormalized before the covariance matrix could
   * be updated and normalized afterwards.
   **/
  class Normalizer {
      std::vector<double> normalization_coeffs_;
      NormalizationMethodFactory::NormalizationMethod norm_method_ = nullptr;
      NormalizerOption norm_option_ = NormalizerOption::Strict;
    public:
      Normalizer() = default;
      Normalizer(const std::vector<double> & normalization_coeffs,
          const NormalizerOption opt = NormalizerOption::Strict);

      /**
       * Appropriate for loading values from a restart file
       **/
      Normalizer(
          const settings::KernelNormalization & norm_method,
          const NormalizerOption opt = NormalizerOption::Strict);

      Normalizer(const BaseDescriptorWrapper & descriptor_wrapper,
          const settings::KernelNormalization & norm_method,
          const NormalizerOption opt = NormalizerOption::Strict);

      const std::vector<double> getNormalizationCoeffs() const;

      void update(
          const BaseDescriptorWrapper & descriptor_wrapper,
          std::any extra_args = settings::None::None);

      template<class T>
      T get() const noexcept;

      void normalize(Covariance & cov) const;
      void unnormalize(Covariance & cov) const;

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          std::any norm_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          std::any norm_instance);


  };

  template<class T>
  inline T Normalizer::get() const noexcept{
    return norm_option_;
  }

  std::ostream& operator<<(std::ostream& os, const NormalizerOption &);
  std::istream& operator>>(std::istream& is, NormalizerOption & );
}
#endif // PANACEA_PRIVATE_NORMALIZER_H
