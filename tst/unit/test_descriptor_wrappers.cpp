
// Local private includes
#include "descriptors/descriptor_wrapper.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:descriptor_wrapper_constructor1","[unit,panacea]"){

  std::vector<std::vector<double>> data;
  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,0,0);

}

TEST_CASE("Testing:descriptor_wrapper_constructor2","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2, 3);
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);
}

TEST_CASE("Testing:descriptor_wrapper_access","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2,3);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(1,0) == 1.0);

  REQUIRE(dwrapper(0,1) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(0,2) == 3.0);
  REQUIRE(dwrapper(1,2) == 3.0);
}

TEST_CASE("Testing:descriptor_arrangements","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2,3);

  // Flip how the rows and columns are being interpreted
  dwrapper.set(Arrangement::DimensionsAlongRowsPointsAlongCols);

  // Rows and columns don't change
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(dwrapper.getNumberPoints() == 3);
  REQUIRE(dwrapper.getNumberDimensions() == 2);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(0,1) == 1.0);

  REQUIRE(dwrapper(1,0) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(2,0) == 3.0);
  REQUIRE(dwrapper(2,1) == 3.0);
}

TEST_CASE("Testing:descriptor_different_template_type","[unit,panacea]"){
  double ** data;
  data = new double*[2];
  data[0] = new double[3]; 
  data[1] = new double[3]; 

  data[0][0] = 1.0; 
  data[1][0] = 1.0; 
  data[0][1] = 2.0; 
  data[1][1] = 2.0; 
  data[0][2] = 3.0; 
  data[1][2] = 3.0; 

  DescriptorWrapper<double ***> dwrapper(&data,2,3);

  // Rows and columns don't change
  REQUIRE(dwrapper.rows() == 2);
  REQUIRE(dwrapper.cols() == 3);

  // But where the dimensions and points are being accessed does
  REQUIRE(dwrapper.getNumberPoints() == 2);
  REQUIRE(dwrapper.getNumberDimensions() == 3);

  REQUIRE(dwrapper(0,0) == 1.0);
  REQUIRE(dwrapper(1,0) == 1.0);

  REQUIRE(dwrapper(0,1) == 2.0);
  REQUIRE(dwrapper(1,1) == 2.0);

  REQUIRE(dwrapper(0,2) == 3.0);
  REQUIRE(dwrapper(1,2) == 3.0);

  delete[] data[0];
  delete[] data[1];
  delete[] data;
}

TEST_CASE("Testing:descriptor_reduced_dimensions","[unit,panacea]"){

  std::vector<std::vector<double>> data;

  std::vector<double> values = {1.0, 2.0, 3.0};
  data.push_back(values);
  data.push_back(values);

  DescriptorWrapper<std::vector<std::vector<double>>*> dwrapper(&data,2,3);

  std::vector<int> reduced_var = { 0, 2}; 
  dwrapper.setReducedNumberDimensions(reduced_var);

  int count1 = 0;
  int count2 = 0;
  for( int pt_ind = 0; pt_ind < data.size(); ++pt_ind){
    for(const int dim : dwrapper.getReducedDimensions()){
      if( dim == 0) {
        REQUIRE(dwrapper(pt_ind,dim) == 1.0);
        ++count1;
      }else if(dim == 2) {
        REQUIRE(dwrapper(pt_ind,dim) == 3.0);
        ++count2;
      }
    }
  }
  REQUIRE(count1 == 2);
  REQUIRE(count2 == 2);
}
