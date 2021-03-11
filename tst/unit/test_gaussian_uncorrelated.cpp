
// Local private includes
#include "primitives/gaussian_uncorrelated.hpp"
#include "descriptors/descriptor_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:creation of gaussian uncorrelated primitive","[unit,panacea]"){
  GaussUncorrelated gauss(0);
}

TEST_CASE("Testing:compute of gaussian uncorrelated primitive","[unit,panacea]"){
  std::vector<std::vector<double>> data;

  std::vector<double> values = {0.0, 1.0};
  data.push_back(values);

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,1, 2);


}
