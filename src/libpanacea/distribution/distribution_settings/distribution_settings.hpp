#ifndef PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#define PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
#pragma once

// Local private PANACEA includes
#include "private_settings.hpp"

// Standard includes
#include <any>

namespace panacea {

struct DistributionSettings {
  virtual void set(std::any) = 0;
  virtual settings::DistributionType type() const noexcept = 0;
  virtual ~DistributionSettings() = 0;

  /**
   * Will allocate space on the heap for a new distribution
   **/
  static std::unique_ptr<DistributionSettings>
  create(const DistributionSettings &settings);
};

} // namespace panacea

#endif // PANACEA_PRIVATE_DISTRIBUTIONSETTINGS_H
