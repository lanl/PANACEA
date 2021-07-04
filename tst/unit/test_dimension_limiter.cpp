
// Local private PANACEA includes
#include "attributes/dimensions.hpp"
#include "attribute_manipulators/dimension_limiter.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:dimension limiter","[integration,panacea]"){

  Dimensions dimensions(5);

  REQUIRE(dimensions.size() == 5);
  DimensionLimiter dim_limit;

  dim_limit.limit(dimensions, 3);
  REQUIRE(dimensions.size() == 3);

}


