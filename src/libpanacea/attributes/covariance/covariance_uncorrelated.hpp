
#ifndef PANACEA_PRIVATE_COVARIANCE_UNCORRELATED_H
#define PANACEA_PRIVATE_COVARIANCE_UNCORRELATED_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"

#include "data_settings.hpp"
#include "matrix/matrix.hpp"
#include "private_settings.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <memory>

namespace panacea {

class CovarianceUncorrelated : public Covariance {
private:
  /// The covariance matrix
  std::unique_ptr<Matrix> matrix_ = nullptr;

  /// Mean of the values used to create the covariance matrix
  std::unique_ptr<Vector> mean_ = nullptr;

  /// Total number of data points used in the creation of the covariance matrix
  /// and in updating it
  int total_number_data_pts_ = 0;

  NormalizationState normalized_ = NormalizationState::Unnormalized;

public:
  CovarianceUncorrelated() = default;

  explicit CovarianceUncorrelated(const CovarianceBuild);

  CovarianceUncorrelated(const BaseDescriptorWrapper &desc_wrap,
                         const CovarianceOption opt = CovarianceOption::Strict);

  CovarianceUncorrelated(std::unique_ptr<Matrix> matrix,
                         std::unique_ptr<Vector> mean, int total_num_pts,
                         const CovarianceOption opt = CovarianceOption::Strict);

  inline virtual settings::KernelCorrelation correlation() const
      noexcept final {
    return settings::KernelCorrelation::Uncorrelated;
  }

  /// Designed to update the covariance matrix
  virtual void update(const BaseDescriptorWrapper &desc_wrap) final;

  /// Don't want to allow for the matrix to be arbitrarily changed but do want
  /// to provide access to the actual covariance matrix elements
  virtual double operator()(const int row, const int col) const final;

  virtual int rows() const final;
  virtual int cols() const final;

  /**
   * Check to ensure that the covariance instance is fully defined, e.g.
   * there are no nullptr's in the internal members.
   **/
  virtual bool defined() const noexcept final;

  virtual double getDeterminant() const final;

  virtual double getMean(const int index) const final;

  virtual int getCummulativeDescPoints() const final;

  virtual bool is(const NormalizationState &state) const noexcept final;

  /**
   * Check if the covariance matrix is full of 0's, this can occur if all
   * data passed in is stacked on top of its self.
   **/
  virtual bool isZero(const double threshold = 1E-9) const noexcept final;

  virtual const NormalizationState &getNormalizationState() const
      noexcept final;

  // Specific to Reducer
  virtual const Matrix &matrix(PassKey<Reducer>) const final;

  // Specific to Normalizer class
  virtual void set(PassKey<Normalizer>, NormalizationState state) final;
  virtual double &operator()(PassKey<Normalizer>, const int row,
                             const int col) final;

  virtual void print() const final;

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &, std::any cov_instance);

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &, std::any cov_instance);
};
} // namespace panacea
#endif // PANACEA_PRIVATE_COVARIANCE_UNCORRELATED_H
