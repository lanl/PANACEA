
// Local private PANACEA includes
#include "median.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:median","[unit,panacea]"){

  class Data {
      std::vector<std::vector<double>> data_ {
        {0.0, 5.0},
        {2.0, 6.0},
        {3.0, 11.0},
      };
    public:
      int rows() const { return data_.size(); }
      int cols() const { return data_.at(0).size(); } 

      double operator()(const int row, const int col) const {
        return data_.at(row).at(col);
      }
  };

  Data data2d;

  Median median;

  WHEN("Calculating median along rows.") {
    auto median_vec = median.calculate(data2d);

    REQUIRE(median_vec.at(0) == Approx(2.5));
    REQUIRE(median_vec.at(1) == Approx(4.0));
    REQUIRE(median_vec.at(2) == Approx(7.0));
  }

  WHEN("Calculating mean along cols.") {
    auto median_vec = median.calculate<Data,Direction::AlongColumns>(data2d);

    REQUIRE(median_vec.at(0) == Approx(2.0));
    REQUIRE(median_vec.at(1) == Approx(6.0));
  }
}

