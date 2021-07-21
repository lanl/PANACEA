// Local private PANACEA includes
#include "entropy_term_common.hpp"

// Standard includes
#include <limits>

namespace panacea {

bool is_neg_inf(const double val) {
  if (val == -1.0 * std::numeric_limits<double>::infinity())
    return true;
  return false;
}
bool is_pos_inf(const double val) {
  if (val == std::numeric_limits<double>::infinity())
    return true;
  return false;
}

} // namespace panacea
