#ifndef PANACEA_PRIVATE_ENTROPYSETTINGS_H
#define PANACEA_PRIVATE_ENTROPYSETTINGS_H
#pragma once

// Standard includes
#include <memory>

namespace panacea {

  class DistributionSettings;

  struct EntropySettings {
    settings::EntropyType type = settings::EntropyType::Self;
    std::unique_ptr<DistributionSettings> dist_settings = nullptr;
  };

}

#endif // PANACEA_PRIVATE_ENTROPYSETTINGS_H
