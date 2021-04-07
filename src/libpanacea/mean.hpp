#ifndef PANACEA_PRIVATE_MEAN_H
#define PANACEA_PRIVATE_MEAN_H
#pragma once

// Local private includes
#include "data_settings.hpp"

// Standard includes
#include <type_traits>
#include <vector>

namespace panacea {

  class Mean {
    public:

      template<class T, const Direction dir = Direction::AlongRows> 
        std::vector<double> calculate(const T & data2d) {

          std::vector<double> mean;
          if constexpr(std::is_pointer<typename std::remove_const<T>::type>::value) {
            if constexpr(dir == Direction::AlongRows) {
              mean.reserve(data2d->cols());
              const double divisor = 1.0/static_cast<double>(data2d->cols());
              for( int row = 0; row < data2d->rows(); ++row){
                double sum = 0.0;
                for( int col = 0; col < data2d->cols(); ++col) {
                  sum += data2d(row,col);
                }
                mean.push_back(sum * divisor);
              }
            } else {
              mean.reserve(data2d->rows());
              for( int col = 0; col < data2d->cols(); ++col) {
                int row = 0;
                mean.push_back(data2d->operator()(row,col));
              }
              for( int row = 1; row < data2d->rows(); ++row){
                for( int col = 0; col < data2d->cols(); ++col) {
                  mean.at(col) += data2d->operator()(row,col);
                }
              }
              const double divisor = 1.0/static_cast<double>(data2d->rows());
              for( int col = 0; col < data2d->cols(); ++col) {
                mean.at(col) *= divisor;
              }
            }
          } else {
            if constexpr(dir == Direction::AlongRows) {
              mean.reserve(data2d.cols());
              const double divisor = 1.0/static_cast<double>(data2d.cols());
              for( int row = 0; row < data2d.rows(); ++row){
                double sum = 0.0;
                for( int col = 0; col < data2d.cols(); ++col) {
                  sum += data2d(row,col);
                }
                mean.push_back(sum * divisor);
              }
            } else {
              mean.reserve(data2d.rows());
              for( int col = 0; col < data2d.cols(); ++col) {
                int row = 0;
                mean.push_back(data2d(row,col));
              }
              for( int row = 1; row < data2d.rows(); ++row){
                for( int col = 0; col < data2d.cols(); ++col) {
                  mean.at(col) += data2d(row,col);
                }
              }
              const double divisor = 1.0/static_cast<double>(data2d.rows());
              for( int col = 0; col < data2d.cols(); ++col) {
                mean.at(col) *= divisor;
              }
            }
          }
          return mean; 
        }
  };
}

#endif // PANACEA_PRIVATE_MEAN_H
