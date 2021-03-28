#ifndef PANACEA_PRIVATE_MEAN_H
#define PANACEA_PRIVATE_MEAN_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  enum class Direction {
    AlongRows,
    AlongColumns
  };

  class Mean {
    public:

      template<class T, const Direction dir = Direction::AlongRows> 
        std::vector<double> calculate(T data2d) {

          std::vector<double> mean;
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
              mean.push_back(data2d(0,col));
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
          return mean; 
        }
  };
}

#endif // PANACEA_PRIVATE_MEAN_H
