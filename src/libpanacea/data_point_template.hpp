#ifndef PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#define PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
#pragma once

// Local private PANACEA includes
#include "error.hpp"

// Standard includes
#include <algorithm>
#include <any>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

namespace panacea {

enum class Arrangement {
  PointsAlongRowsDimensionsAlongCols,
  DimensionsAlongRowsPointsAlongCols
};

/**
 * This MemoryLayout class is used in cases where a single allocation is used
 * for the memory, instead, of a nested array of pointers or vectors.
 *
 * Cache locality for RowMajor is exploited if the inner nested loop is
 * indexed by column
 *
 * Cache locality for ColumnMajor is exploited if the inner nested loop is
 * indexed by row
 *
 * If Default is specified it is an indication row major will be chosen, but
 * it may indicate also that the layout patterns may not really apply e.g.
 * in nested vectors there is no need to map a column and row to a single
 * index.
 **/
enum class MemoryLayout { Default, RowMajor, ColumnMajor };

std::ostream &operator<<(std::ostream &os, const Arrangement &arrange);
/* Allows abstraction
 * T should be a (double) pointer type
 * E.g
 *
 * std::vector<std::vector<double>> *
 *
 * double ***
 *
 * std::vector<double> *
 *
 * If a non-nested vector is provided it is assumed that the data is present
 * in row major form, so if the Arrangement::PointsAlongRowsDimensionsAlongCols
 * then the cache locality will be exploited if the dimensions are indexed
 * within the inner loop of a nested for loop.
 *
 * Will by default assume template is basic of the form double *** and has
 * therefore access to the elements is provided via double bracket notation
 *
 * If a single dimension type is used e.g. a vector<double> then it is assumed
 * that it is for a single point only, and that each variable in the vector
 * represents a different dimension.
 */
template <class T, MemoryLayout lay = MemoryLayout::Default>
class DataPointTemplate {
protected:
  T data_;
  Arrangement arrangement_ = Arrangement::PointsAlongRowsDimensionsAlongCols;
  static constexpr MemoryLayout layout_ = lay;
  int number_dimensions_ = 0;
  int number_points_ = 0;
  int rows_ = 0;
  int cols_ = 0;

  inline int getIndex_(const int row, const int col) const noexcept {
    if constexpr (layout_ == MemoryLayout::ColumnMajor) {
      return col * cols_ + row;
    } else {
      return row * rows_ + col;
    }
  }

public:
  DataPointTemplate() = default;

  DataPointTemplate(const int rows, const int cols)
      : rows_(rows), cols_(cols), number_dimensions_(cols),
        number_points_(rows) {

    // This requires we know the underlying type to make sure
    // memory is available.
    if constexpr (std::is_same<T, std::vector<std::vector<double>>>::value) {
      data_.reserve(rows);
      for (int row = 0; row < rows; ++row) {
        data_.emplace_back(cols, 0.0);
      }
    } else {
      std::string error_msg =
          "Constructor without data is not supported for the";
      error_msg += " underlying type.";
      PANACEA_FAIL(error_msg);
    }
  }

  DataPointTemplate(const T &data, const int rows, const int cols)
      : data_(data), rows_(rows), cols_(cols), number_dimensions_(cols),
        number_points_(rows){
        };

  /*
   * Allows the user to change how the rows and columns
   * are interpreted by default, assumes that the points
   * are located along the rows and the dimensions along
   * the columns.
   */
  void set(const Arrangement arrangement);
  const Arrangement &arrangement() const noexcept { return arrangement_; }

  /**
   * Memory layout indicates whether the unerlying memory pattern is
   * Row or Column major if appropriate.
   *
   * Must be set at compile time
   **/
  inline const MemoryLayout &layout() const noexcept { return layout_; }

  /*
   * Access operator is used such that you will pass in the point and the
   * dimension not the row and column.
   */
  double &operator()(const int point_ind, const int dim_ind);
  double operator()(const int point_ind, const int dim_ind) const;

  double &at(const int row, const int col);
  double at(const int row, const int col) const;

  int getNumberDimensions() const noexcept { return number_dimensions_; }
  int getNumberPoints() const noexcept { return number_points_; }

  int rows() const noexcept { return rows_; }

  int cols() const noexcept { return cols_; }

  void resize(const int rows, const int cols);
  /*
   * This function allows one to check if two groups of data are pointing to
   * the same data.
   *
   * Be careful that you check the type with const if the template owns
   * the data
   */
  std::any getPointerToRawData() const {
    if (std::is_pointer<T>::value) {
      return data_;
    } else {
      return &data_;
    }
  }

  void print() const;
};

template <class T, MemoryLayout lay>
inline void DataPointTemplate<T, lay>::set(const Arrangement arrangement) {
  arrangement_ = arrangement;
  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    number_points_ = rows_;
    number_dimensions_ = cols_;
  } else {
    number_dimensions_ = rows_;
    number_points_ = cols_;
  }
}

template <class T, MemoryLayout lay>
inline void DataPointTemplate<T, lay>::print() const {
  std::cout << "Rows " << rows() << " Columns " << cols() << "\n";
  std::cout << "Dimensions " << getNumberDimensions();
  std::cout << " Points " << getNumberPoints() << "\n";
  std::cout << "\nData\n";
  for (int row = 0; row < rows(); ++row) {
    for (int col = 0; col < cols(); ++col) {
      std::cout << at(row, col) << "\n";
    }
    std::cout << std::endl;
  }
}

template <class T, MemoryLayout lay>
inline double &DataPointTemplate<T, lay>::operator()(const int point_ind,
                                                     const int dim_ind) {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);
  if constexpr (std::is_pointer<T>::value) {
    if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
      return (*data_)[point_ind][dim_ind];
    }
    return (*data_)[dim_ind][point_ind];
  } else {
    if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
      return (data_)[point_ind][dim_ind];
    }
    return (data_)[dim_ind][point_ind];
  }
}

template <class T, MemoryLayout lay>
inline double DataPointTemplate<T, lay>::operator()(const int point_ind,
                                                    const int dim_ind) const {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if constexpr (std::is_pointer<T>::value) {
    if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
      return (*data_)[point_ind][dim_ind];
    }
    return (*data_)[dim_ind][point_ind];
  } else {
    if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
      return (data_)[point_ind][dim_ind];
    }
    return (data_)[dim_ind][point_ind];
  }
}

/*
 * Specialization of the template in the case that a nested
 * vector pointer is used
 */
template <>
inline double &DataPointTemplate<std::vector<std::vector<double>> *>::
operator()(const int point_ind, const int dim_ind) {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_->at(point_ind).at(dim_ind);
  }
  return data_->at(dim_ind).at(point_ind);
}

template <>
inline double DataPointTemplate<std::vector<std::vector<double>> *>::
operator()(const int point_ind, const int dim_ind) const {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_->at(point_ind).at(dim_ind);
  }
  return data_->at(dim_ind).at(point_ind);
}

/*
 * Specialization of the template in the case that a vector pointer is used
 */
template <>
inline double &DataPointTemplate<std::vector<double> *>::
operator()(const int point_ind, const int dim_ind) {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_->at(getIndex_(point_ind, dim_ind));
  }
  return data_->at(getIndex_(dim_ind, point_ind));
}

template <>
inline double DataPointTemplate<std::vector<double> *>::
operator()(const int point_ind, const int dim_ind) const {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_->at(getIndex_(point_ind, dim_ind));
  }
  return data_->at(getIndex_(dim_ind, point_ind));
}

/*
 * Specialization of the template in the case that a vector is used
 */
template <>
inline double &DataPointTemplate<std::vector<double>>::
operator()(const int point_ind, const int dim_ind) {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_.at(getIndex_(point_ind, dim_ind));
  }
  return data_.at(getIndex_(dim_ind, point_ind));
}

template <>
inline double DataPointTemplate<std::vector<double>>::
operator()(const int point_ind, const int dim_ind) const {
  assert(point_ind >= 0 && point_ind < number_points_);
  assert(dim_ind >= 0 && dim_ind < number_dimensions_);

  if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
    return data_.at(getIndex_(point_ind, dim_ind));
  }
  return data_.at(getIndex_(dim_ind, point_ind));
}

template <class T, MemoryLayout lay>
inline double &DataPointTemplate<T, lay>::at(const int row, const int col) {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);
  if constexpr (std::is_pointer<T>::value) {
    return (*data_)[row][col];
  } else {
    return (data_)[row][col];
  }
}

template <class T, MemoryLayout lay>
inline double DataPointTemplate<T, lay>::at(const int row,
                                            const int col) const {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  if constexpr (std::is_pointer<T>::value) {
    return (*data_)[row][col];
  } else {
    return (data_)[row][col];
  }
}

/*
 * Specialization of the template in the case that a nested
 * vector pointer is used
 */
template <>
inline double &
DataPointTemplate<std::vector<std::vector<double>> *>::at(const int row,
                                                          const int col) {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  return data_->at(row).at(col);
}

template <>
inline double
DataPointTemplate<std::vector<std::vector<double>> *>::at(const int row,
                                                          const int col) const {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  return data_->at(row).at(col);
}

/*
 * Specialization of the template in the case that a
 * vector pointer is used
 */
template <>
inline double &DataPointTemplate<std::vector<double> *>::at(const int row,
                                                            const int col) {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  return data_->at(getIndex_(row, col));
}

template <>
inline double
DataPointTemplate<std::vector<double> *>::at(const int row,
                                             const int col) const {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  return data_->at(getIndex_(row, col));
}

/*
 * Specialization of the template in the case that a vector is used
 */
template <>
inline double &DataPointTemplate<std::vector<double>>::at(const int row,
                                                          const int col) {
  assert(col >= 0 && col < cols_);
  assert(col >= 0 && col < cols_);

  return data_.at(getIndex_(row, col));
}

template <>
inline double DataPointTemplate<std::vector<double>>::at(const int row,
                                                         const int col) const {
  assert(row >= 0 && row < rows_);
  assert(col >= 0 && col < cols_);

  return data_.at(getIndex_(row, col));
}

template <class T, MemoryLayout lay>
inline void DataPointTemplate<T, lay>::resize(const int rows, const int cols) {
  assert(rows > 0);
  assert(cols > 0);
  if (std::is_pointer<T>::value) {
    std::string error_msg = "Cannot resize data, it is a pointer, and is not ";
    error_msg += "owned by the underlying object.\n";
    PANACEA_FAIL(error_msg);
  }
  if constexpr (std::is_same<T, std::vector<std::vector<double>>>::value) {
    rows_ = rows;
    cols_ = cols;
    if (arrangement_ == Arrangement::PointsAlongRowsDimensionsAlongCols) {
      number_points_ = rows_;
      number_dimensions_ = cols_;
    } else {
      number_dimensions_ = rows;
      number_points_ = cols;
    }
    data_.resize(rows);
    for (int row = 0; row < rows; ++row) {
      data_.at(row).resize(cols);
    }
  } else if constexpr (std::is_same<T, std::vector<double>>::value) {
    rows_ = rows;
    cols_ = cols;
    // If have a one dimensional vector, only the dimensions are allowed to
    // be greater than 1
    number_points_ = rows_;
    number_dimensions_ = cols_;
    data_.resize(rows * cols);

  } else {
    std::string error_msg = "Currently resize method is ownly supported for";
    error_msg += " types of:\nstd::vector<std::vector<double>>.\n";
    error_msg += " std::vector<double>.\n";
    PANACEA_FAIL(error_msg);
  }
}

template class DataPointTemplate<std::vector<std::vector<double>> *>;
template class DataPointTemplate<std::vector<std::vector<double>>>;
template class DataPointTemplate<std::vector<double>>;
template class DataPointTemplate<std::vector<double> *>;
template class DataPointTemplate<double ***>;
} // namespace panacea

#endif // PANACEA_PRIVATE_DATAPOINTTEMPLATE_H
