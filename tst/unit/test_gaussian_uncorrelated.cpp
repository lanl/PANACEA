
// Local private includes
#include "primitives/gaussian_uncorrelated.hpp"

#include "attributes/covariance.hpp"
#include "descriptors/descriptor_wrapper.hpp"
#include "kernels/kernel_wrapper.hpp"

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

  // Assumes we are dealing with two dimensions and a single point
  std::vector<std::vector<double>> raw_desc_data;
  std::vector<double> raw_desc_values = {0.0, 1.0};
  raw_desc_data.push_back(raw_desc_values);
  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&raw_desc_data,1, 2);

  Covariance cov(&dwrapper);

  // Assumes we are dealing with two dimensions and a single point
  std::vector<std::vector<double>> raw_kern_data;
  std::vector<double> raw_kern_values = {0.0, 0.5};
  raw_kern_data.push_back(raw_kern_values);
  KernelWrapper<std::vector<std::vector<double>>*> kwrapper(&raw_kern_data,1, 2);

  PrimitiveAttributes attr;
  attr.kernel_wrapper = &kwrapper;
}
