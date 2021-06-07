
// Local private includes
#include "matrix/matrix.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:matrix test factory method","[unit,panacea]"){

  WHEN("rows 0 and columns 0") {
    auto matrix = createMatrix(0,0, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);
    REQUIRE(matrix->rows() == 0);
    REQUIRE(matrix->cols() == 0);
  }

  WHEN("rows < 0 and columns < 0") {
    REQUIRE_THROWS(createMatrix(-1,-1, MatrixType::Eigen));
    REQUIRE_THROWS(createMatrix(0,-1, MatrixType::Eigen));
    REQUIRE_THROWS(createMatrix(-1,0, MatrixType::Eigen));
  }

  WHEN("rows 3 and columns 3") {
    auto matrix = createMatrix(3,3, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);
    REQUIRE(matrix->rows() == 3);
    REQUIRE(matrix->cols() == 3);
  }
}


TEST_CASE("Testing:matrix assignment","[unit,panacea]"){

  WHEN("rows 3 and columns 3") {
    const int rows = 3;
    const int cols = 3;
    auto matrix = createMatrix(rows, cols, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);

    // Assign values
    double value = 0.0;
    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        matrix->operator()(row,col) = value;
        value += 1.0;
      }
    }

    // Check values stored correctly
    value = 0.0;
    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        REQUIRE(matrix->operator()(row,col) == Approx(value));
        value += 1.0;
      }
    }
  }
}

TEST_CASE("Testing:matrix test resize","[unit,panacea]"){

  WHEN("rows 3 and columns 3") {
    const int rows = 3;
    const int cols = 3;
    auto matrix = createMatrix(rows, cols, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);
    REQUIRE(matrix->rows() == rows);
    REQUIRE(matrix->cols() == cols);

    matrix->resize(2,4);
    REQUIRE(matrix->rows() == 2);
    REQUIRE(matrix->cols() == 4);
  }
}


TEST_CASE("Testing:matrix test set Zero","[unit,panacea]"){

  WHEN("rows 3 and columns 3") {
    const int rows = 3;
    const int cols = 3;
    auto matrix = createMatrix(rows, cols, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);

    // Assign values
    double value = 0.0;
    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        matrix->operator()(row,col) = value;
        value += 1.0;
      }
    }

    // First change some of the values
    matrix->setZero();

    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        REQUIRE(matrix->operator()(row,col) == Approx(0.0));
      }
    }
  }
}

TEST_CASE("Testing:matrix test make Identity","[unit,panacea]"){

  WHEN("rows 3 and columns 3") {
    const int rows = 3;
    const int cols = 3;
    auto matrix = createMatrix(rows, cols, MatrixType::Eigen);
    REQUIRE(matrix != nullptr);
    REQUIRE(matrix.get() != nullptr);

    // Assign values
    double value = 0.0;
    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        matrix->operator()(row,col) = value;
        value += 1.0;
      }
    }

    // First change some of the values
    matrix->makeIdentity();

    for(int row = 0; row < rows; ++row) {
      for( int col = 0; col < cols; ++col) {
        if( row == col ) {
          REQUIRE(matrix->operator()(row,col) == Approx(1.0));
        }else {
          REQUIRE(matrix->operator()(row,col) == Approx(0.0));
        }
      }
    }
  }
}
