
// Local private PANACEA includes
#include "attribute_manipulators/inverter.hpp"

#include "attributes/covariance.hpp"
#include "attributes/dimensions.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:inverter trivial","[unit,panacea]"){

  auto mat = createMatrix(4,4);

  auto vec = createVector(4);

  vec->operator()(0) =  2.0;
  vec->operator()(1) =  1.0;
  vec->operator()(2) =  3.0;
  vec->operator()(3) = -9.0;

  const int num_pts = 10;

  // Contents of matrix
  //
  //   1  0  1  0
  //   0  1  0  0
  //   1  0  1  0
  //   0  0  0  1

  // Row 0
  mat->operator()(0,0) =  1.0;
  mat->operator()(0,1) =  0.0;
  mat->operator()(0,2) =  1.0;
  mat->operator()(0,3) =  0.0;

  // Row 1
  mat->operator()(1,0) =  0.0;
  mat->operator()(1,1) =  1.0;
  mat->operator()(1,2) =  0.0;
  mat->operator()(1,3) =  0.0;

  // Row 2
  mat->operator()(2,0) =  1.0;
  mat->operator()(2,1) =  0.0;
  mat->operator()(2,2) =  1.0;
  mat->operator()(2,3) =  0.0;

  // Row 3
  mat->operator()(3,0) =  0.0;
  mat->operator()(3,1) =  0.0;
  mat->operator()(3,2) =  0.0;
  mat->operator()(3,3) =  1.0;

  mat->print();

  auto cov_ptr = Covariance::create(
      settings::KernelCorrelation::Correlated,
      std::move(mat),
      std::move(vec),
      num_pts);

  auto & covar = *cov_ptr;

  WHEN("Priority rows are sequential") {
    Reducer reducer;

    std::vector<int> priority_rows { 0, 1, 2, 3};
    ReducedCovariance reduced_covar = reducer.reduce(covar, Dimensions(priority_rows));

    Inverter inverter;
    ReducedInvCovariance reduced_inv_cov = inverter.invert(reduced_covar);

    // The inverse should also be the identity matrix
    REQUIRE(reduced_inv_cov.getNumberDimensions() == 3);

    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(0) == 0);
    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(1) == 1);
    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(2) == 3);

    // The reduced matrix should simply be the identity matrix
    //
    // 1 0 0
    // 0 1 0
    // 0 0 1
    for( int row = 0; row < reduced_inv_cov.getNumberDimensions(); ++row ) {
      for( int col = 0; col < reduced_inv_cov.getNumberDimensions(); ++col ) {
        if( row == col ) {
          REQUIRE(reduced_inv_cov(row,col) == Approx(1.0));
        }else {
          REQUIRE(reduced_inv_cov(row,col) == Approx(0.0));
        }
      }
    }

  }
}

TEST_CASE("Testing:inverter less trivial","[unit,panacea]"){

  auto mat = createMatrix(4,4);

  auto vec = createVector(4);

  vec->operator()(0) =  2.0;
  vec->operator()(1) =  1.0;
  vec->operator()(2) =  3.0;
  vec->operator()(3) = -9.0;

  const int num_pts = 10;

  // Contents of matrix
  //
  //   2  0  2  0
  //   0  1  0  0
  //   2  0  2  0
  //   0  0  0  1

  // Row 0
  mat->operator()(0,0) =  2.0;
  mat->operator()(0,1) =  0.0;
  mat->operator()(0,2) =  2.0;
  mat->operator()(0,3) =  0.0;

  // Row 1
  mat->operator()(1,0) =  0.0;
  mat->operator()(1,1) =  1.0;
  mat->operator()(1,2) =  0.0;
  mat->operator()(1,3) =  0.0;

  // Row 2
  mat->operator()(2,0) =  2.0;
  mat->operator()(2,1) =  0.0;
  mat->operator()(2,2) =  2.0;
  mat->operator()(2,3) =  0.0;

  // Row 3
  mat->operator()(3,0) =  0.0;
  mat->operator()(3,1) =  0.0;
  mat->operator()(3,2) =  0.0;
  mat->operator()(3,3) =  1.0;

  mat->print();

  auto cov_ptr = Covariance::create(
      settings::KernelCorrelation::Correlated,
      std::move(mat),
      std::move(vec),
      num_pts);

  auto & covar = *cov_ptr;

  WHEN("Priority rows are sequential") {
    Reducer reducer;

    std::vector<int> priority_rows { 0, 1, 2, 3};
    ReducedCovariance reduced_covar = reducer.reduce(covar, Dimensions(priority_rows));

    Inverter inverter;
    ReducedInvCovariance reduced_inv_cov = inverter.invert(reduced_covar);

    // The inverse should also be the identity matrix
    REQUIRE(reduced_inv_cov.getNumberDimensions() == 3);

    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(0) == 0);
    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(1) == 1);
    REQUIRE(reduced_inv_cov.getChosenDimensionIndices().at(2) == 3);

    // The reduced matrix should simply be the identity matrix
    //
    // 0.5 0.0 0.0
    // 0.0 1.0 0.0
    // 0.0 0.0 1.0
    //
    REQUIRE(reduced_inv_cov(0,0) == Approx(0.5));
    REQUIRE(reduced_inv_cov(1,1) == Approx(1.0));
    REQUIRE(reduced_inv_cov(2,2) == Approx(1.0));

    // Check all off diagonal elements are 0.0
    for( int row = 0; row < reduced_inv_cov.getNumberDimensions(); ++row ) {
      for( int col = 0; col < reduced_inv_cov.getNumberDimensions(); ++col ) {
        if( row != col ) {
          REQUIRE(reduced_inv_cov(row,col) == Approx(0.0));
        }
      }
    }

  }
}

