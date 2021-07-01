#ifndef PANACEA_PRIVATE_RANDOMIZER_H
#define PANACEA_PRIVATE_RANDOMIZER_H
#pragma once

namespace panacea {

  class Dimensions;

  class Randomizer {
    public:
      Randomizer() = default;

      /**
       * Designed to randomize dimensions
       **/
      void randomize(Dimensions & dimensions) const;
  };
}

#endif // PANACEA_PRIVATE_RANDOMIZER_H
