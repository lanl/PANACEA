#ifndef PANACEA_PRIVATE_VARIANCE_H
#define PANACEA_PRIVATE_VARIANCE_H
#pragma once

// Local private PANACEA includes
#include "mean.hpp"

// Standard includes
#include <cassert>
#include <iostream>
#include <type_traits>
#include <vector>

namespace panacea {

class Variance {
public:
  template <class T> std::vector<double> calculate(T data2d) {
    // Sample variance requires larger than 1 number of points in each
    // dimension
    if constexpr (std::is_pointer<typename std::remove_const<T>::type>::value) {
      assert(data2d->rows() > 1);
      Mean mean;
      std::vector<double> mean_vec =
          mean.calculate<T, Direction::AlongColumns>(data2d);

      std::vector<double> variance_vec(mean_vec.size(), 0.0);

      for (int row = 0; row < data2d->rows(); ++row) {
        for (int col = 0; col < data2d->cols(); ++col) {
          const double diff = (data2d->operator()(row, col) - mean_vec.at(col));
          variance_vec.at(col) += diff * diff;
        }
      }

      const double divisor = 1.0 / static_cast<double>(data2d->rows() - 1);
      for (int col = 0; col < data2d->cols(); ++col) {
        variance_vec.at(col) *= divisor;
      }
      return variance_vec;
    } else {
      assert(data2d.rows() > 1);
      Mean mean;
      std::vector<double> mean_vec =
          mean.calculate<T, Direction::AlongColumns>(data2d);

      std::vector<double> variance_vec(mean_vec.size(), 0.0);

      for (int row = 0; row < data2d.rows(); ++row) {
        for (int col = 0; col < data2d.cols(); ++col) {
          const double diff = (data2d(row, col) - mean_vec.at(col));
          variance_vec.at(col) += diff * diff;
        }
      }

      const double divisor = 1.0 / static_cast<double>(data2d.rows() - 1);
      for (int col = 0; col < data2d.cols(); ++col) {
        variance_vec.at(col) *= divisor;
      }
      return variance_vec;
    }
  }
};
} // namespace panacea

#endif // PANACEA_PRIVATE_VARIANCE_H
