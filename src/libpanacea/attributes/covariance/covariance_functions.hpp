#ifndef PANACEA_PRIVATE_COVARIANCE_FUNCTIONS_H
#define PANACEA_PRIVATE_COVARIANCE_FUNCTIONS_H
#pragma once

// Local private PANACEA includes
#include "attributes/covariance.hpp"
#include "error.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Local public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <optional>
#include <vector>

namespace panacea {
namespace covariance {

/**
 * Provided a bunch of descriptors, cycle through each dimension of the
 * descriptors and calculate the mean.
 **/
void calculateMean(Vector &mean, const BaseDescriptorWrapper &desc_wrap);

/**
 * Update the mean with a new batch of descriptors.
 *
 * The third argument represents the total number of descriptor points that
 * were used to calculate the first argument (mean) before it was passed into
 * this function.
 **/
void updateMean(Vector &mean, const BaseDescriptorWrapper &desc_wrap,
                const int current_num_desc_pts);

void updateCovariance(Matrix &covariance, const Vector &current_mean,
                      const Vector &new_mean, const int current_num_pts,
                      const BaseDescriptorWrapper &desc_wrap);

/**
 * Check that the covariance matrix is symmetric.
 *
 * Points on either side of the diagonal are compared if the difference between
 * all the points is less than the threshold provided in the third argument the
 * matrix is considered symmetric. If it is not symmetric than an error is
 *thrown. This check is only turned on if compiled with debug mode on.
 **/
void checkForSymmetry(Matrix &matrix, const double threshold);

/**
 * This method provides flexibility when defining a covariance matrix.
 *
 * In the scenario that the covariance matrix is rigoursly defined and you
 * end up with a 0 matrix for the covariance matrix this function will throw
 * an error. However, if opt is specified as flexible and this occurs the
 * covariance matrix is assumed to be the identity matrix.
 **/
void accountForZeroMatrix(const CovarianceOption opt, Matrix &matrix);

/**
 * Similar to the above method but only checks the diagonal.
 **/
void accountForZeroOnDiagonal(const CovarianceOption opt, Matrix &matrix);

/**
 * Prints the contents of the covariance matrix to the screen.
 **/
void printCovariance(const Matrix &matrix, const Vector &mean,
                     const int total_number_data_pts);

} // namespace covariance
} // namespace panacea

#endif // PANACEA_PRIVATE_COVARIANCE_FUNCTIONS_H
