
// Local private PANACEA includes
#include "mean.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:mean", "[unit,panacea]") {

  class Data {
    std::vector<std::vector<double>> data_{
        {1.0, 5.0},
        {2.0, 6.0},
        {3.0, 7.0},
    };

  public:
    int rows() const { return data_.size(); }
    int cols() const { return data_.at(0).size(); }

    double operator()(const int row, const int col) const {
      return data_.at(row).at(col);
    }
  };

  Data data2d;

  Mean mean;

  WHEN("Calculating mean along rows.") {
    auto mean_vec = mean.calculate(data2d);

    REQUIRE(mean_vec.at(0) == Approx(3.0));
    REQUIRE(mean_vec.at(1) == Approx(4.0));
    REQUIRE(mean_vec.at(2) == Approx(5.0));
  }

  WHEN("Calculating mean along cols.") {
    auto mean_vec = mean.calculate<Data, Direction::AlongColumns>(data2d);

    REQUIRE(mean_vec.at(0) == Approx(2.0));
    REQUIRE(mean_vec.at(1) == Approx(6.0));
  }
}
