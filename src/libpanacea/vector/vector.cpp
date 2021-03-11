// Local public PANACEA includes
#include "panacea/vector.hpp"

// Local private PANACEA includes
#include "vector_eigen.hpp"

namespace panacea {

  std::unique_ptr<Vector> createVector(const int rows, const VectorTypes type) {
    auto vec_eig = std::unique_ptr<VectorEigen>(new VectorEigen);
    vec_eig->resize(rows);
    return vec_eig;
  }
}
