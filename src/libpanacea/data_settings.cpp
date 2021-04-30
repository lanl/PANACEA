
// Local pivate PANACEA includes
#include "data_settings.hpp"

// Standard includes
#include <fstream>

namespace panacea {

  std::ostream& operator<<(std::ostream& os, const Direction & dir ) {
    if( dir == Direction::AlongRows ) {
      os << "AlongRows";
    }else if(dir == Direction::AlongColumns) {
      os << "AlongColumns";
    }
    return os;
  }

  std::ostream& operator<<(std::ostream& os, const NormalizationState & state) {
    if(state == NormalizationState::Normalized ) {
      os << "Normalized";
    }else if(state == NormalizationState::Unnormalized) {
      os << "Unnormalized";
    }
    return os; 
  }
}

