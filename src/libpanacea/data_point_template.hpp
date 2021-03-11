#ifndef PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#define PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#pragma once

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <algorithm>
#include <cassert>
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
        DataPointTemplate(T data, int rows, int cols) : 
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

        int getNumberDimensions() const noexcept { return number_dimensions_; }
        int getNumberPoints() const noexcept { return number_points_; }

        int rows() const noexcept { return rows_; }

        int cols() const noexcept { return cols_; }

        /*
         * This function allows one to check if two groups of data are pointing to
         * the same data. 
         */
        void * getPointerToRawData() {
          if( std::is_pointer<T>::value ) {
            return static_cast<void *>(data_);
          } else {
            return static_cast<void *>(&data_);
          }
        }
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
    inline double& DataPointTemplate<T>::operator()(const int point_ind, const int dim_ind) {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return (*data_)[point_ind][dim_ind];
      }
      return (*data_)[dim_ind][point_ind];
    }

  template<class T> 
    inline double DataPointTemplate<T>::operator()(const int point_ind, const int dim_ind) const {
      assert(point_ind >= 0 && point_ind < number_points_);
      assert(dim_ind >= 0 && dim_ind < number_dimensions_);

      if( arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols){ 
        return (*data_)[point_ind][dim_ind];
      }
      return (*data_)[dim_ind][point_ind];
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

  template class DataPointTemplate<std::vector<std::vector<double>>*>;
  template class DataPointTemplate<double ***>;
}

#endif // PANACEA_PRIVATE_DATAPOINTTEMPLATE_H



