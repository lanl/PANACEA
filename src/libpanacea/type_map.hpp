#ifndef PANACEA_PRIVATE_TYPEMAP_H
#define PANACEA_PRIVATE_TYPEMAP_H
#pragma once

// Standard includes
#include <deque>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

namespace panacea {

  extern const std::unordered_map<std::type_index,std::string> type_map; 

}

#endif // PANACEA_PRIVATE_TYPEMAP_H

