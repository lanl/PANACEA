
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

TEST_CASE("Testing:covariance non-trivial","[unit,panacea]"){
  std::vector<std::vector<double>> data {
      {0.6787,    0.6948,    0.7094},
      {0.7577,    0.3171,    0.7547},
      {0.7431,    0.9502,    0.2760},
      {0.3922,    0.0344,    0.6797},
      {0.6555,    0.4387,    0.6551},
      {0.1712,    0.3816,    0.1626}};

  DescriptorWrapper<std::vector<std::vector<double>>*> 
    dwrapper(&data, data.size(), data.at(0).size());

  Covariance cov(&dwrapper);

  const int number_of_dimensions = 3;
  REQUIRE(cov.rows() == number_of_dimensions);
  REQUIRE(cov.cols() == number_of_dimensions);

  REQUIRE(cov.getMean(0) == Approx(0.5664));
  REQUIRE(cov.getMean(1) == Approx(0.4695));
  REQUIRE(cov.getMean(2) == Approx(0.5396));

  REQUIRE(cov.getCummulativeDescPoints() == data.size());

  REQUIRE( cov(0,0) == Approx(0.055) );
  REQUIRE( cov(0,1) == Approx(0.0378) );
  REQUIRE( cov(0,2) == Approx(0.0297) );
  REQUIRE( cov(1,0) == Approx(0.0378) );
  REQUIRE( cov(1,1) == Approx(0.1006) );
  REQUIRE( cov(1,2) == Approx(-0.0305) );
  REQUIRE( cov(2,0) == Approx(0.0297) );
  REQUIRE( cov(2,1) == Approx(-0.0305) );
  REQUIRE( cov(2,2) == Approx(0.0639) );


}
