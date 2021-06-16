
// Local private PANACEA includes
#include "helper.hpp"

#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "primitives/primitive_attributes.hpp"

namespace panacea {

  namespace test {

    std::vector<std::vector<double>> generateVecData() {
      std::vector<std::vector<double>> data;

      std::vector<double> values = {1.0, 2.0, 3.0};
      data.push_back(values);
      data.push_back(values);

      return data;
    }

    ArrayData::ArrayData() {
      data = new double*[2];
      data[0] = new double[3];
      data[1] = new double[3];

      data[0][0] = 1.0;
      data[1][0] = 1.0;
      data[0][1] = 2.0;
      data[1][1] = 2.0;
      data[0][2] = 3.0;
      data[1][2] = 3.0;
    }

    ArrayData::~ArrayData() {
      delete[] data[0];
      delete[] data[1];
      delete[] data;
    }

    ArrayData2::ArrayData2() {
      data = new double*[3];
      data[0] = new double[2];
      data[1] = new double[2];
      data[2] = new double[2];

      data[0][0] = 1.0;
      data[1][0] = 2.0;
      data[2][0] = 3.0;
      data[0][1] = 4.0;
      data[1][1] = 5.0;
      data[2][1] = 6.0;
    }

    ArrayData2::~ArrayData2() {
      delete[] data[0];
      delete[] data[1];
      delete[] data[2];
      delete[] data;
    }

  }
}
