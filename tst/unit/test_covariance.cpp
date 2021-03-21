
// Local private includes
#include "attributes/covariance.hpp"

#include "descriptors/descriptor_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:covariance test trivial constructor","[unit,panacea]"){

  // 3 points 2 dimensions
  std::vector<std::vector<double>> data{
    {1.0, 4.0},
    {2.0, 5.0},
    {3.0, 6.0}};

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data, 3, 2);

  Covariance cov(&dwrapper);

  REQUIRE(cov.rows() == 2);
  REQUIRE(cov.cols() == 2);

  REQUIRE(cov.getMean(0) == Approx(2.0));
  REQUIRE(cov.getMean(1) == Approx(5.0));

  REQUIRE(cov.getCummulativeDescPoints() == 3);

  REQUIRE( cov(0,0) == Approx(1.0) );
  REQUIRE( cov(0,1) == Approx(1.0) );
  REQUIRE( cov(1,0) == Approx(1.0) );
  REQUIRE( cov(1,1) == Approx(1.0) );

  REQUIRE( cov.getDeterminant() == Approx(0,0) );
}


