
// Local private PANACEA includes
#include "attributes/dimensions.hpp"
#include "attribute_manipulators/randomizer.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:randomizer","[integration,panacea]"){

  Dimensions dimensions(5);

  Randomizer randomizer;

  randomizer.randomize(dimensions);

  REQUIRE(dimensions.state() == DimensionsState::Randomized);

  bool out_of_order = false;
  int ordered_val = 0;
  for( const auto & val : dimensions ) {
    if( val != ordered_val ) {
      out_of_order = true;
      break;
    }
    ++ordered_val;
  }
  REQUIRE(out_of_order);
}

