#ifndef PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#define PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#pragma once

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <algorithm>
#include <any>
#include <cassert>
#include <iostream>
#include <vector>

namespace panacea {

  enum class Arrangement {
    PointsAlongRowsDimensionsAlongCols,
    DimensionsAlongRowsPointsAlongCols
  };

  /* Allows abstraction
   * T should be a (double) pointer type
   * E.g
   *
   * std::vector<std::vector<double>> *
   *
   * double ***
   *
   * Will by default assume template is basic of the form double *** and has 
   * therefore access to the elements is provided via double bracket notation
   */ 
  template<class T> 
    class DataPointTemplate {
      protected:
        T data_;
        Arrangement arrangement_ = Arrangement::PointsAlongRowsDimensionsAlongCols;
        int number_dimensions_ = 0;
        int number_points_ = 0;
        int rows_ = 0;
        int cols_ = 0;
      public:
  
        DataPointTemplate() = default;
        DataPointTemplate(T data, const int & rows, const int & cols) : 
          data_(data),
          rows_(rows),
          cols_(cols),
          number_dimensions_(cols),
          number_points_(rows) {};

        /*
         * Allows the user to change how the rows and columns
         * are interpreted by default, assumes that the points
         * are located along the rows and the dimensions along
         * the columns.
         */
        void set(const Arrangement arrangement);

        /*
         * Access operator is used such that you will pass in the point and the dimension
         * not the row and column. 
         */
        double& operator()(const int point_ind, const int dim_ind);
        double operator()(const int point_ind, const int dim_ind) const;

        double & at(const int row, const int col);
        double at(const int row, const int col) const;

        int getNumberDimensions() const noexcept { return number_dimensions_; }
        int getNumberPoints() const noexcept { return number_points_; }

        int rows() const noexcept { return rows_; }

        int cols() const noexcept { return cols_; }

        /*
         * This function allows one to check if two groups of data are pointing to
         * the same data. 
         */
        const std::any getPointerToRawData() const {
          if( std::is_pointer<T>::value ) {
            return data_;
          } else {
            return &data_;
          }
        }

        void print() const;
    };

  template<class T>
    inline void DataPointTemplate<T>::set(const Arrangement arrangement) { 
      arrangement_ = arrangement;
      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols ) {
        number_points_ = rows_;
        number_dimensions_ = cols_;
      } else {
        number_dimensions_ = rows_;
        number_points_ = cols_;
      } 
    }

  template<class T>
    inline void DataPointTemplate<T>::print() const {
      std::cout << "Rows " << rows() << " Columns " << cols() << "\n";
      std::cout << "Dimensions " << getNumberDimensions();
      std::cout << " Points " << getNumberPoints() << "\n";
      std::cout << "\nData\n";
      for( int row = 0; row < rows(); ++row) {
        for( int col = 0; col < cols(); ++col) {
          std::cout << "\n";
        }
        std::cout << std::endl;
      }
    }

  template<class T> 
    inline double& DataPointTemplate<T>::operator()(const int point_ind, const int dim_ind) {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);
      if constexpr(std::is_pointer<T>::value) {
        if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
          return (*data_)[point_ind][dim_ind];
        }
        return (*data_)[dim_ind][point_ind];
      } else {
        if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
          return (data_)[point_ind][dim_ind];
        }
        return (data_)[dim_ind][point_ind];
      }
    }

  template<class T> 
    inline double DataPointTemplate<T>::operator()(const int point_ind, const int dim_ind) const {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if constexpr(std::is_pointer<T>::value) {
        if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
          return (*data_)[point_ind][dim_ind];
        }
        return (*data_)[dim_ind][point_ind];
      } else {
        if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
          return (data_)[point_ind][dim_ind];
        }
        return (data_)[dim_ind][point_ind];
      }
    } 

  /*
   * Specialization of the template in the case that a vector pointer is used
   */
  template<> 
    inline double& DataPointTemplate<std::vector<std::vector<double>> *>::operator()(
        const int point_ind, const int dim_ind) {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return data_->at(point_ind).at(dim_ind);
      }
      return data_->at(dim_ind).at(point_ind);
    }

  template<> 
    inline double DataPointTemplate<std::vector<std::vector<double>> *>::operator()(
        const int point_ind, const int dim_ind) const {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return data_->at(point_ind).at(dim_ind);
      }
      return data_->at(dim_ind).at(point_ind);
    } 

  /*
   * Specialization of the template in the case that a vector is used
   */
  template<> 
    inline double& DataPointTemplate<std::vector<double>>::operator()(
        const int point_ind, const int dim_ind) {
      assert(point_ind == 0 );
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return data_.at(dim_ind);
      }
      return data_.at(point_ind);
    }

  template<> 
    inline double DataPointTemplate<std::vector<double>>::operator()(
        const int point_ind, const int dim_ind) const {
      assert(point_ind == 0);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return data_.at(dim_ind);
      }
      return data_.at(point_ind);
    } 


  template<class T> 
    inline double& DataPointTemplate<T>::at(const int row, const int col) {
      assert(row >= 0 && row < cols_);
      assert(col >= 0 && col < rows_);
      if constexpr(std::is_pointer<T>::value) {
        return (*data_)[row][col];
      } else {
        return (data_)[row][col];
      }
    }

  template<class T> 
    inline double DataPointTemplate<T>::at(const int row, const int col) const {
      assert(row >= 0 && row < rows_);
      assert(col >= 0 && col < cols_);

      if constexpr(std::is_pointer<T>::value) {
        return (*data_)[row][col];
      } else {
        return (data_)[row][col];
      }
    } 

  /*
   * Specialization of the template in the case that a vector pointer is used
   */
  template<> 
    inline double& DataPointTemplate<std::vector<std::vector<double>> *>::at(
        const int row, const int col) {
      assert(row >= 0 && row < rows_);
      assert(col >= 0 && col < cols_);

      return data_->at(row).at(col);
    }

  template<> 
    inline double DataPointTemplate<std::vector<std::vector<double>> *>::at(
        const int row, const int col) const {
      assert(row >= 0 && row < rows_);
      assert(col >= 0 && col < cols_);

      return data_->at(row).at(col);
    } 

  /*
   * Specialization of the template in the case that a vector is used
   */
  template<> 
    inline double& DataPointTemplate<std::vector<double>>::at(
        const int row, const int col) {
      assert(row == 0 );
      assert(col >= 0 && col < cols_);

      return data_.at(col);
    }

  template<> 
    inline double DataPointTemplate<std::vector<double>>::at(
        const int row, const int col) const {
      assert(row == 0);
      assert(col >= 0 && col < cols_);

      return data_.at(col);
    } 


  template class DataPointTemplate<std::vector<std::vector<double>>*>;
  template class DataPointTemplate<double ***>;
}

#endif // PANACEA_PRIVATE_DATAPOINTTEMPLATE_H



