
// Local private PANACEA includes
#include "primitives/primitive_factory.hpp"

// Public PANACEA includes
#include "descriptors/descriptor_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:primitive_factory trivial constructor", "[unit,panacea]") {
  PrimitiveFactory prim_factory;
}

