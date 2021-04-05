
// Local private PANACEA includes
#include "variance.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:variance","[unit,panacea]"){

  class Data {
      std::vector<std::vector<double>> data_ {
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

  Variance variance;

  auto variance_vec = variance.calculate(data2d);
  REQUIRE(variance_vec.size() == 2);

  REQUIRE(variance_vec.at(0) == Approx(4.0));
  REQUIRE(variance_vec.at(1) == Approx(1.0));

}

