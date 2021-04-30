#ifndef PANACEA_PRIVATE_DATA_SETTINGS_H
#define PANACEA_PRIVATE_DATA_SETTINGS_H
#pragma once

// Standard includes
#include <fstream>

namespace panacea {

  enum class Direction {
    AlongRows,
    AlongColumns
  };

  // Is an attribute normalized or not
  enum class NormalizationState {
    Normalized,
    Unnormalized
  };

  std::ostream& operator<<(std::ostream& os, const Direction & );
  std::ostream& operator<<(std::ostream& os, const NormalizationState & );

}
#endif // PANACEA_PRIVATE_DATA_SETTINGS_H
