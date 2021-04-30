
// Local private PANACEA includes
#include "attribute_manipulators/normalizer.hpp"

#include "attributes/covariance.hpp"

// Local public PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:normalizer1","[unit,panacea]"){

  auto matrix = createMatrix(3,3);
  auto vector = createVector(3);
  const int total_num_pts = 10;

  // Make diagonal 2, 3, 4, and off diagonal elements 1
  // Make vector unity
  //
  // 2 1 1
  // 1 3 1
  // 1 1 4
  //
  for( int i = 0; i < 3; ++i){
    vector->operator()(i) = 1;
    matrix->operator()(i, i) = i+2;
    for( int j = i+1; j< 3; ++j ){
      matrix->operator()(i, j) = 1;
      matrix->operator()(j, i) = 1;
    }
  }

  Covariance covar(std::move(matrix), std::move(vector), total_num_pts);

  std::vector<double> coeffs = { 1.0, 3.0, 2.0 };
  Normalizer normalizer(coeffs);

  REQUIRE(covar.getNormalizationState() == NormalizationState::Unnormalized);
  normalizer.normalize(covar);
  REQUIRE(covar.getNormalizationState() == NormalizationState::Normalized);

  // Coefficients should be divided by 1.0, 6.0 and 2.0
  REQUIRE(covar(0,0) == Approx(2.0));
  REQUIRE(covar(1,1) == Approx(1.0/3.0));
  REQUIRE(covar(2,2) == Approx(1.0));

  REQUIRE(covar(0,1) == Approx(1.0/3.0));
  REQUIRE(covar(1,0) == Approx(1.0/3.0));
  REQUIRE(covar(2,0) == Approx(1.0/2.0));
  REQUIRE(covar(0,2) == Approx(1.0/2.0));
  REQUIRE(covar(1,2) == Approx(1.0/(2.0 * 3.0)));
  REQUIRE(covar(2,1) == Approx(1.0/(2.0 * 3.0)));

  normalizer.unnormalize(covar);
  REQUIRE(covar.getNormalizationState() == NormalizationState::Unnormalized);
  
  REQUIRE(covar(0,0) == Approx(2.0));
  REQUIRE(covar(1,1) == Approx(3.0));
  REQUIRE(covar(2,2) == Approx(4.0));

  REQUIRE(covar(0,1) == Approx(1.0));
  REQUIRE(covar(1,0) == Approx(1.0));
  REQUIRE(covar(2,0) == Approx(1.0));
  REQUIRE(covar(0,2) == Approx(1.0));
  REQUIRE(covar(1,2) == Approx(1.0));
  REQUIRE(covar(2,1) == Approx(1.0));

}
