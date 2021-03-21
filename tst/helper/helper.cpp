
// Local private PANACEA includes
#include "helper.hpp"

#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "kernels/kernel_wrapper.hpp"
#include "memory.hpp"
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
/*
    PrimitiveAttributes generatePrimitiveAttributes(::panacea::MemoryManager & mem_manager) {

      // Lets assume there are a total of three dimensions
      // the reduced matrices will only two of the dimensions
      std::vector<std::vector<double>> data(2);
      std::vector<double> data_vals;

      data_vals.push_back(1.0); 
      data_vals.push_back(2.0); 
      data_vals.push_back(3.0); 
      data.push_back(data_vals);
      
      mem_manager.createNode(
          std::make_unique<std::vector<std::vector<double>>>(data), "prim_attr_data");

      const int rows = 1;
      const int cols = 3;
      auto kern_wrapper_ptr = 
        std::make_unique<
        KernelWrapper<std::vector<std::vector<double>>*>>(ptr_to_data, rows, cols);
      
      mem_manager.createNode(std::move(kern_wrapper_ptr), "prim_kern_wrapper");
  
      const int rows_reduced_cov = 2;
      const int cols_reduced_cov = 2;
      auto r_cov_mat = createMatrix(rows_reduced_cov,cols_reduced_cov);
      (*r_cov_mat)(0,0) = 1.0;
      (*r_cov_mat)(1,0) = 0.5;
      (*r_cov_mat)(0,1) = 0.5;
      (*r_cov_mat)(1,1) = 1.0;
      // Set covariance matrix equal to
      // 1.0 0.5
      // 0.5 1.0
      auto r_cov_inv_mat = createMatrix(rows_reduced_cov,cols_reduced_cov);
      (*r_cov_inv_mat)(0,0) = 1.333333333333;
      (*r_cov_inv_mat)(1,0) =-0.666666666666;
      (*r_cov_inv_mat)(0,1) =-0.666666666666;
      (*r_cov_inv_mat)(1,1) = 1.333333333333;

      PrimitiveAttributes prim_attr;
      auto ptr_to_data = mem_manager.getRawPointer<
        std::vector<std::vector<double>>*>("prim_attr_data");

      prim_attr.kernel_wrapper = mem_manager.getRawPointer<
        BaseKernelWrapper>("prim_kern_wrapper");

      prim_attr.reduced_covariance = ReducedCovariance(std::move(r_cov_mat)); 
      prim_attr.reduced_inv_covariance = ReducedInvCovariance(std::move(r_cov_inv_mat)); 

      return prim_attr;
    }*/
  }
}
