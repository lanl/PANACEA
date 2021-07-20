
// Local private PANACEA includes
#include "attribute_manipulators/reducer.hpp"

#include "attributes/covariance.hpp"
#include "attributes/dimensions.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:reducer simple test", "[integration,panacea]") {

  GIVEN("A small data set with linearly independent data.") {
    std::vector<std::vector<double>> data{{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}};

    DescriptorWrapper<std::vector<std::vector<double>> *> dwrapper_init(&data,
                                                                        3, 2);

    WHEN("Creating a correlated covariance matrix.") {
      auto cov_ptr = Covariance::create(dwrapper_init,
                                        settings::KernelCorrelation::Correlated,
                                        settings::KernelAlgorithm::Flexible);

      Reducer reducer;

      std::vector<int> priority_rows{0, 1};
      ReducedCovariance reduced_covar =
          reducer.reduce(*cov_ptr, Dimensions(priority_rows));

      // Dimensions should be reduced because correlated and the second
      // dimension is linearly dependent on the first.
      REQUIRE(reduced_covar.getNumberDimensions() == 1);
    }
    WHEN("Creating an uncorrelated covariance matrix.") {
      auto cov_ptr = Covariance::create(
          dwrapper_init, settings::KernelCorrelation::Uncorrelated,
          settings::KernelAlgorithm::Flexible);

      Reducer reducer;

      std::vector<int> priority_rows{0, 1};
      ReducedCovariance reduced_covar =
          reducer.reduce(*cov_ptr, Dimensions(priority_rows));

      // Dimensions should also be reduced
      REQUIRE(reduced_covar.getNumberDimensions() == 1);
    }
  }
}

TEST_CASE("Testing:reducer1", "[integration,panacea]") {

  auto mat = createMatrix(4, 4);

  auto vec = createVector(4);

  vec->operator()(0) = 2.0;
  vec->operator()(1) = 1.0;
  vec->operator()(2) = 3.0;
  vec->operator()(3) = -9.0;

  const int num_pts = 10;

  // Contents of matrix
  //
  //   1  0  1  0
  //   0  1  0  0
  //   1  0  1  0
  //   0  0  0  1

  // Row 0
  mat->operator()(0, 0) = 1.0;
  mat->operator()(0, 1) = 0.0;
  mat->operator()(0, 2) = 1.0;
  mat->operator()(0, 3) = 0.0;

  // Row 1
  mat->operator()(1, 0) = 0.0;
  mat->operator()(1, 1) = 1.0;
  mat->operator()(1, 2) = 0.0;
  mat->operator()(1, 3) = 0.0;

  // Row 2
  mat->operator()(2, 0) = 1.0;
  mat->operator()(2, 1) = 0.0;
  mat->operator()(2, 2) = 1.0;
  mat->operator()(2, 3) = 0.0;

  // Row 3
  mat->operator()(3, 0) = 0.0;
  mat->operator()(3, 1) = 0.0;
  mat->operator()(3, 2) = 0.0;
  mat->operator()(3, 3) = 1.0;

  mat->print();

  auto cov_ptr = Covariance::create(settings::KernelCorrelation::Correlated,
                                    std::move(mat), std::move(vec), num_pts);

  auto &covar = *cov_ptr;

  WHEN("Priority rows are sequential") {
    Reducer reducer;

    std::vector<int> priority_rows{0, 1, 2, 3};
    ReducedCovariance reduced_covar =
        reducer.reduce(covar, Dimensions(priority_rows));

    // The order of the dimensions should be consistent
    REQUIRE(reduced_covar.getNumberDimensions() == 3);

    REQUIRE(reduced_covar.getReducedDimensions().at(0) == 0);
    REQUIRE(reduced_covar.getReducedDimensions().at(1) == 1);
    REQUIRE(reduced_covar.getReducedDimensions().at(2) == 3);

    // The reduced matrix should simply be the identity matrix
    //
    // 1 0 0
    // 0 1 0
    // 0 0 1
    for (int row = 0; row < reduced_covar.getNumberDimensions(); ++row) {
      for (int col = 0; col < reduced_covar.getNumberDimensions(); ++col) {
        if (row == col) {
          REQUIRE(reduced_covar(row, col) == Approx(1.0));
        } else {
          REQUIRE(reduced_covar(row, col) == Approx(0.0));
        }
      }
    }
  }
  WHEN("Priority rows are non sequential") {
    Reducer reducer;

    std::vector<int> priority_rows{2, 0, 1, 3};
    ReducedCovariance reduced_covar =
        reducer.reduce(covar, Dimensions(priority_rows));

    // The order of the dimensions should be consistent
    REQUIRE(reduced_covar.getNumberDimensions() == 3);

    REQUIRE(reduced_covar.getReducedDimensions().at(0) == 2);
    REQUIRE(reduced_covar.getReducedDimensions().at(1) == 1);
    REQUIRE(reduced_covar.getReducedDimensions().at(2) == 3);

    // The reduced matrix should also in this case simply be the identity matrix
    //
    // 1 0 0
    // 0 1 0
    // 0 0 1
    for (int row = 0; row < reduced_covar.getNumberDimensions(); ++row) {
      for (int col = 0; col < reduced_covar.getNumberDimensions(); ++col) {
        if (row == col) {
          REQUIRE(reduced_covar(row, col) == Approx(1.0));
        } else {
          REQUIRE(reduced_covar(row, col) == Approx(0.0));
        }
      }
    }
  }
}
