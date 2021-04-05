#ifndef PANACEA_PRIVATE_MEDIAN_H
#define PANACEA_PRIVATE_MEDIAN_H
#pragma once

// Local private includes
#include "data_settings.hpp"

// Standard includes
#include <vector>

namespace panacea {

  class Median {

    template<class T> 
      std::vector<double> createSortedColumn(const T & data2d, const int row){
        std::vector<double> local_column;
        local_column.reserve(data2d.cols());
        // Create local copy
        for( int col = 0; col < data2d.cols(); ++col) {
          local_column.push_back(data2d(row,col)); 
        }
        std::sort(local_column.begin(), local_column.end());
        return local_column;
      } 

    template<class T> 
      std::vector<double> createSortedRow(const T & data2d, const int col){
        std::vector<double> local_row;
        local_row.reserve(data2d.rows());
        // Create local copy
        for( int row = 0; row < data2d.rows(); ++row) {
          local_row.push_back(data2d(row,col)); 
        }
        std::sort(local_row.begin(), local_row.end());
        return local_row;
      } 


    public:

      template<class T, const Direction dir = Direction::AlongRows> 
        std::vector<double> calculate(const T & data2d) {

          std::vector<double> median;
          if constexpr(dir == Direction::AlongRows) {
            median.reserve(data2d.cols());
            if( data2d.rows() % 2 == 1) {
              // is odd 
              for( int row = 0; row < data2d.rows(); ++row){
                const auto local_column = createSortedColumn(data2d, row);
                median.push_back(local_column.at(data2d.rows() / 2));
              }
            } else {
              // is even
              for( int row = 0; row < data2d.rows(); ++row){
                const auto local_column = createSortedColumn(data2d, row);
                const double avg_of_two = 
                  (local_column.at(data2d.rows() / 2) + 
                   local_column.at((data2d.rows() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          } else {
            median.reserve(data2d.rows());
            if( data2d.cols() % 2 == 1) {
              // is odd 
              for( int col = 0; col < data2d.cols(); ++col){
                const auto local_row = createSortedRow(data2d, col);
                median.push_back(local_row.at(data2d.cols() / 2));
              }
            } else {
              // is even
              for( int col = 0; col < data2d.cols(); ++col){
                const auto local_row = createSortedRow(data2d, col);
                const double avg_of_two = 
                  (local_row.at(data2d.cols() / 2) + 
                   local_row.at((data2d.cols() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          }
          return median; 
        }
  };
}

#endif // PANACEA_PRIVATE_MEAN_H
