

// Local private PANACEA includes
#include "vector_eigen.hpp"

// Third party includes
#include <Eigen/Dense>

// Standard includes
#include <cassert>
#include <memory>

namespace panacea {

  VectorEigen::VectorEigen() {
    vector_ = std::make_unique<Eigen::VectorXd>();
  }

  void VectorEigen::resize(const int rows) {
    assert(rows>=0);
    vector_->resize(rows);
  }

  double& VectorEigen::operator()(const int row) {
    assert(row>=0);
    assert(row<vector_->rows());
    return (*vector_)(row);
  }

  double VectorEigen::operator()(const int row) const {
    assert(row>=0);
    assert(row<vector_->rows());
    return (*vector_)(row);
  }

  int VectorEigen::rows() const {
    return vector_->rows();
  }
}
