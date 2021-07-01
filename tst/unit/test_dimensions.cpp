
// Local private PANACEA includes
#include "attributes/dimensions.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:dimensions","[unit,panacea]"){

  Dimensions dimensions(5);

  REQUIRE(dimensions.size() == 5);
  REQUIRE(dimensions.state() == DimensionState::Ordered);

  int expected_val = 0;
  for( const auto & val : dimensions ) {
    REQUIRE(expected_val == val);
    ++expected_val;
  }
}

