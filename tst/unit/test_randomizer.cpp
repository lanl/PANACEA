
// Local private PANACEA includes
#include "attributes/dimensions.hpp"
#include "attribute_manipulators/randomizer.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <unordered_set>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:randomizer choice of dimensions","[integration,panacea]"){

  Dimensions dimensions(5);

  Randomizer randomizer;

  randomizer.randomize(
      dimensions,
      settings::RandomizeDimensions::Yes,
      settings::RandomizeNumberDimensions::No);

  REQUIRE(dimensions.state() == DimensionsState::Randomized);
  REQUIRE(dimensions.size() == 5);
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

TEST_CASE("Testing:randomizer number dimensions","[integration,panacea]"){
  const int total_num_dims = 20;
  Dimensions dimensions(total_num_dims);

  Randomizer randomizer;

  // If I randomize the count 30 times then the total number of dimensions
  // is highly likely to be less than 20.

  int sum_count = 0;
  const int samples = 30;
  for( int sample = 0; sample < samples; ++sample) {
    randomizer.randomize(
        dimensions,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::Yes);

    REQUIRE(dimensions.state() == DimensionsState::RandomizedCount);

    // Dimensions should be ordered
    int expected_dim = 0;
    for(const auto & dim : dimensions) {
      REQUIRE(expected_dim == dim);
      ++expected_dim;
    }

    sum_count += static_cast<double>(dimensions.size());
  }

  const double avg_dims = static_cast<double>(sum_count)/static_cast<double>(samples);
  REQUIRE(avg_dims < static_cast<double>(total_num_dims));
}

TEST_CASE("Testing:randomizer check range samples sizes from 1 to max","[integration,panacea]"){
  const int total_num_dims = 2;
  Dimensions dimensions(total_num_dims);

  Randomizer randomizer;

  const int samples = 30;

  // For 30 samples we should see the randomizer choose:
  // * both dimensions
  // * single dimension
  std::unordered_set<int> number_of_dimensions_used;
  for( int sample = 0; sample < samples; ++sample) {
    randomizer.randomize(
        dimensions,
        settings::RandomizeDimensions::No,
        settings::RandomizeNumberDimensions::Yes);

    number_of_dimensions_used.insert(dimensions.size());
    REQUIRE(dimensions.size() > 0);
    REQUIRE(dimensions.size() <= total_num_dims);
  }

  REQUIRE(number_of_dimensions_used.count(1));
  REQUIRE(number_of_dimensions_used.count(2));
  REQUIRE(number_of_dimensions_used.size() == 2);
}
