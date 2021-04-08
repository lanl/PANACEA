#ifndef PANACEA_PRIVATE_DATA_SETTINGS_H
#define PANACEA_PRIVATE_DATA_SETTINGS_H
#pragma once

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


}
#endif // PANACEA_PRIVATE_DATA_SETTINGS_H
