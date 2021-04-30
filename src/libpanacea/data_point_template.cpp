// Local private PANACEA includes
#include "data_point_template.hpp"

namespace panacea {

  std::ostream& operator<<(std::ostream& os, const Arrangement& arrange) {
    if( arrange == Arrangement::PointsAlongRowsDimensionsAlongCols ) {
      os << "Points along rows dimensions along columns";
    } else if( arrange == Arrangement::DimensionsAlongRowsPointsAlongCols) {
      os << "Dimensions along rows points along columns";
    }
    return os;
  }
}
