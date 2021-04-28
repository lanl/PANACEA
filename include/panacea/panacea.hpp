
#ifndef PANACEA_H
#define PANACEA_H
#pragma once

// Standard includes
#include <memory>

namespace panacea {

  class PANACEA {

    std::unique_ptr<EntropyTerm> create();

  };

};

#endif // PANACEA_H
