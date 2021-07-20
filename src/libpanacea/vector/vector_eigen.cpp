

// Local private PANACEA includes
#include "vector_eigen.hpp"

// Third party includes
#include <Eigen/Dense>

// Standard includes
#include <cassert>
#include <iostream>
#include <memory>

namespace panacea {

VectorEigen::VectorEigen() { vector_ = std::make_unique<Eigen::VectorXd>(); }

const VectorType VectorEigen::type() const { return VectorType::Eigen; }

void VectorEigen::setZero() { vector_->setZero(); }

void VectorEigen::resize(const int size) {
  assert(vector_->rows() >= 0);
  vector_->resize(size);
}

VectorEigen &VectorEigen::operator=(const Vector &vec) {
  this->resize(vec.rows());
  for (int row = 0; row < vec.rows(); ++row) {
    this->operator()(row) = vec(row);
  }
  return *this;
}

double &VectorEigen::operator()(const int index) {
  assert(index >= 0);
  assert(index < vector_->rows());
  return (*vector_)(index);
}

double VectorEigen::operator()(const int index) const {
  assert(index >= 0);
  assert(index < vector_->rows());
  return (*vector_)(index);
}

int VectorEigen::rows() const {
  if (direction_ == Direction::AlongRows) {
    return vector_->rows();
  } else {
    return 1;
  }
}

Direction VectorEigen::direction() const { return direction_; }

int VectorEigen::cols() const {
  if (direction_ == Direction::AlongRows) {
    return 1;
  } else {
    return vector_->rows();
  }
}

void VectorEigen::print() const { std::cout << (*vector_) << std::endl; }
} // namespace panacea
