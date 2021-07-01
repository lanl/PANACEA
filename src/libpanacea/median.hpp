#ifndef PANACEA_PRIVATE_MEDIAN_H
#define PANACEA_PRIVATE_MEDIAN_H
#pragma once

// Local private includes
#include "data_settings.hpp"

// Standard includes
#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

namespace panacea {

  class Median {

    template<class T>
      std::vector<double> createSortedRow(
          const T & data2d,
          const int row){

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
      std::vector<double> createSortedColumn(
          const T & data2d,
          const int col){

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

    /**
     * Will calculate the median of a 2d set of data
     *
     * For even numbers of rows or columns will use the average
     * of the two centeral elements.
     *
     * Can calculate the median along the rows or along the columns
     *
     *       col1   col2
     * row1  0.0    7.0
     * row2  3.0    5.0
     * row3  2.0    3.0
     *
     * Along rows will return
     *
     * row1 3.5
     * row2 4.0
     * row3 2.5
     *
     * Along columns will return
     *
     *     col1      col2
     *     2.0       5.0
     *
     **/
    template<class T, const Direction dir = Direction::AlongRows>
      std::vector<double> calculate(const T & data2d) {

        std::vector<double> median;
        if constexpr(std::is_pointer<typename std::remove_const<T>::type>::value) {
          if constexpr(dir == Direction::AlongRows) {
            median.reserve(data2d->rows());
            if( data2d->cols() % 2 == 1) {
              // is odd
              for( int row = 0; row < data2d->rows(); ++row){
                const auto local_row = createSortedRow(*data2d, row);
                median.push_back(local_row.at(data2d->cols() / 2));
              }
            } else {
              // is even
              for( int row = 0; row < data2d->rows(); ++row){
                const auto local_row = createSortedRow(*data2d, row);
                const double avg_of_two =
                  (local_row.at(data2d->cols() / 2) +
                   local_row.at((data2d->cols() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          } else {
            median.reserve(data2d->cols());
            if( data2d->rows() % 2 == 1) {
              // is odd
              for( int col = 0; col < data2d->cols(); ++col){
                const auto local_column = createSortedColumn(*data2d, col);
                median.push_back(local_column.at(data2d->rows() / 2));
              }
            } else {
              // is even
              for( int col = 0; col < data2d->cols(); ++col){
                const auto local_column = createSortedColumn(*data2d, col);
                const double avg_of_two =
                  (local_column.at(data2d->rows() / 2) +
                   local_column.at((data2d->rows() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          }
        } else if constexpr(std::is_same<typename std::remove_const<T>::type, std::vector<std::deque<double>>>::value) {

          if(data2d.size() == 0) return median;

          const int rows = data2d.size();
          const int cols = data2d.at(0).size();
          if constexpr(dir == Direction::AlongRows) {
            median.reserve(rows);
            if( cols % 2 == 1) {
              // is odd
              for( int row = 0; row < rows; ++row){
                const auto local_row = createSortedRow(data2d, row);
                median.push_back(local_row.at(cols / 2));
              }
            } else {
              // is even
              for( int row = 0; row < rows; ++row){
                const auto local_row = createSortedRow(data2d, row);
                const double avg_of_two =
                  (local_row.at(cols / 2) +
                   local_row.at((cols / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          } else {
            median.reserve(cols);
            if( rows % 2 == 1) {
              // is odd
              for( int col = 0; col < cols; ++col){
                const auto local_column = createSortedColumn(data2d, col);
                median.push_back(local_column.at(rows / 2));
              }
            } else {
              // is even
              for( int col = 0; col < cols; ++col){
                const auto local_column = createSortedColumn(data2d, col);
                const double avg_of_two =
                  (local_column.at(rows / 2) +
                   local_column.at(rows / 2) - 1) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          }


        } else { // If not a pointer type
          if constexpr(dir == Direction::AlongRows) {
            median.reserve(data2d.rows());
            if( data2d.cols() % 2 == 1) {
              // is odd
              for( int row = 0; row < data2d.rows(); ++row){
                const auto local_row = createSortedRow(data2d, row);
                median.push_back(local_row.at(data2d.cols() / 2));
              }
            } else {
              // is even
              for( int row = 0; row < data2d.rows(); ++row){
                const auto local_row = createSortedRow(data2d, row);
                const double avg_of_two =
                  (local_row.at(data2d.cols() / 2) +
                   local_row.at((data2d.cols() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          } else {
            median.reserve(data2d.cols());
            if( data2d.rows() % 2 == 1) {
              // is odd
              for( int col = 0; col < data2d.cols(); ++col){
                const auto local_column = createSortedColumn(data2d, col);
                median.push_back(local_column.at(data2d.rows() / 2));
              }
            } else {
              // is even
              for( int col = 0; col < data2d.cols(); ++col){
                const auto local_column = createSortedColumn(data2d, col);
                const double avg_of_two =
                  (local_column.at(data2d.rows() / 2) +
                   local_column.at((data2d.rows() / 2) - 1)) * 0.5;
                median.push_back(avg_of_two);
              }
            }
          }
        }
        return median;
      }
  };

  template<>
  inline std::vector<double> Median::createSortedRow
    <std::vector<std::deque<double>>>(
        const std::vector<std::deque<double>> & data2d,
        const int row){

      std::vector<double> local_column;
      local_column.reserve(data2d.at(0).size());
      // Create local copy
      for( int col = 0; col < data2d.at(0).size(); ++col) {
        local_column.push_back(data2d.at(row).at(col));
      }
      std::sort(local_column.begin(), local_column.end());
      return local_column;
    }

  template<>
  inline std::vector<double> Median::createSortedColumn
    <std::vector<std::deque<double>>>(
        const std::vector<std::deque<double>> & data2d,
        const int col){

      std::vector<double> local_row;
      local_row.reserve(data2d.size());
      // Create local copy
      for( int row = 0; row < data2d.size(); ++row) {
        local_row.push_back(data2d.at(row).at(col));
      }
      std::sort(local_row.begin(), local_row.end());
      return local_row;
    }
}

#endif // PANACEA_PRIVATE_MEAN_H
