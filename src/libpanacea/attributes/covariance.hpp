
#ifndef PANACEA_PRIVATE_COVARIANCE_H
#define PANACEA_PRIVATE_COVARIANCE_H
#pragma once


// Local private includes
#include "data_settings.hpp"
#include "private_settings.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/passkey.hpp"

// Standard includes
#include <any>
#include <memory>

namespace panacea {

  /**
   * If 0.0 are encountered:
   * Flexible - will set normalization constant to 1.0
   * Strict - will set to 0.0
   **/
  enum class CovarianceOption {
    Flexible,
    Strict
  };

  // Whether to allocate memory for internal structures or not
  enum class CovarianceBuild {
    Allocate,
    NoAllocate
  };


  class BaseDescriptorWrapper;
  class Normalizer;

  /*
   * Class for storing the covariance matrix
   *
   * The covariance matrix can be generate using two techniques, the first is
   * to pass in a descriptor wrapper class. The data in the descriptor wrapper
   * is used to create the covariance matrix.
   *
   * Note, by default covariance matrix creation will adhere to a strict interpretation
   * of what a covariance matrix is. Thus if all of your data points are stacked on
   * top of one another than you will end up with a covariance matrix full of 0.0's which
   * does not make sense and is numerically instable for use with a kernel.
   *
   * To avoid this case the CovarianceOption = Flexible can be specified this will
   * allow the covariance matrix to assign 0's on problematic diagonal elements.
   */
  class Covariance {

    public:
      virtual ~Covariance();

      virtual settings::KernelCorrelation correlation() const noexcept = 0;

      /// Designed to update the covariance matrix
      virtual void update(const BaseDescriptorWrapper & desc_wrap) = 0;

      /// Don't want to allow for the matrix to be arbitrarily changed but do want to
      /// provide access to the actual covariance matrix elements
      virtual double operator()(const int row, const int col) const = 0;

      virtual int rows() const = 0;
      virtual int cols() const = 0;

      /**
       * Check to ensure that the covariance instance is fully defined, e.g.
       * there are no nullptr's in the internal members.
       **/
      virtual bool defined() const noexcept = 0;

      virtual double getDeterminant() const = 0;

      virtual double getMean(const int index) const = 0;

      /**
       * The total number of points used to build the covariance matrix.
       **/
      virtual int getCummulativeDescPoints() const = 0;

      virtual bool is(const NormalizationState & state) const noexcept = 0;

      /**
       * Check if the covariance matrix is full of 0's, this can occur if all
       * data passed in is stacked on top of its self.
       **/
      virtual bool isZero(const double threshold = 1E-9) const noexcept = 0;

      virtual const NormalizationState & getNormalizationState() const noexcept = 0;

      // Specific to Normalizer class
      virtual void set(PassKey<Normalizer>, NormalizationState state) = 0;
      virtual double & operator()(PassKey<Normalizer>, const int row, const int col) = 0;

      virtual void print() const = 0;

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          std::any cov_instance);

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          std::any cov_instance);

      /*
       * Factory method
       **/
      static std::unique_ptr<Covariance> create(
          const BaseDescriptorWrapper & desc_wrap,
          const settings::KernelCorrelation corr,
          const CovarianceOption opt = CovarianceOption::Strict);

      static std::unique_ptr<Covariance> create(
          const BaseDescriptorWrapper & desc_wrap,
          const settings::KernelCorrelation corr,
          const settings::KernelAlgorithm opt);

      static std::unique_ptr<Covariance> create(
          const settings::KernelCorrelation corr);


      static std::unique_ptr<Covariance> create(
          const settings::KernelCorrelation,
          std::unique_ptr<Matrix> matrix,
          std::unique_ptr<Vector> mean,
          int total_num_pts,
          const CovarianceOption opt = CovarianceOption::Strict);
  };

}
#endif // PANACEA_PRIVATE_COVARIANCE_H

