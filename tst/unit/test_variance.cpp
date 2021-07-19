
// Local private PANACEA includes
#include "variance.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:variance", "[unit,panacea]") {

  class Data {
    std::vector<std::vector<double>> data_{
        {1.0, 5.0},
        {3.0, 6.0},
        {5.0, 7.0},
    };

  public:
    int rows() const { return data_.size(); }
    int cols() const { return data_.at(0).size(); }

    double operator()(const int row, const int col) const {
      return data_.at(row).at(col);
    }
  };

  Data data2d;

  Variance variance(VarianceType::Sample);

  auto variance_vec = variance.calculate(data2d);
  REQUIRE(variance_vec.size() == 2);

  REQUIRE(variance_vec.at(0) == Approx(4.0));
  REQUIRE(variance_vec.at(1) == Approx(1.0));
}

TEST_CASE("Testing:variance consistency", "[unit,panacea]") {

  class Data3 {
    std::vector<std::vector<double>> data_{
        {-3.0},
        {0.0},
        {3.0},
    };

  public:
    int rows() const { return data_.size(); }
    int cols() const { return data_.at(0).size(); }

    double operator()(const int row, const int col) const {
      return data_.at(row).at(col);
    }
  };

  Data3 data_3_pts;

  Variance variance(VarianceType::Population);

  auto variance_vec_3_pts = variance.calculate(data_3_pts);
  REQUIRE(variance_vec_3_pts.size() == 1);

  // (9 + 9 + 0 / (3)) = 18/3 = 6.0
  REQUIRE(variance_vec_3_pts.at(0) == Approx(6.0));

  // Now if we add three more points
  // ((9 + 9 + 0 + 9 + 9 + 0)/6) = (36)/6 = 6.0
  class Data6 {
    std::vector<std::vector<double>> data_{
        {-3.0}, {0.0}, {3.0}, {-3.0}, {0.0}, {3.0},
    };

  public:
    int rows() const { return data_.size(); }
    int cols() const { return data_.at(0).size(); }

    double operator()(const int row, const int col) const {
      return data_.at(row).at(col);
    }
  };

  Data6 data_6_pts;

  auto variance_vec_6_pts = variance.calculate(data_6_pts);
  REQUIRE(variance_vec_6_pts.size() == 1);
  REQUIRE(variance_vec_3_pts.at(0) == Approx(variance_vec_6_pts.at(0)));
}
