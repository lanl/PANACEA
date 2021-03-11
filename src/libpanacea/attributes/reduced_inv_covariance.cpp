
// Local private PANACEA includes
#include "reduced_inv_covariance.hpp"

namespace panacea {

  double ReducedInvCovariance::operator()(const int row, const int col) const {
    return (*matrix_)(row,col);
  }
}
