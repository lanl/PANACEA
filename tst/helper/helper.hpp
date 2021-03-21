
#ifndef PANACEA_PRIVATE_HELPER_H
#define PANACEA_PRIVATE_HELPER_H
#pragma once

// Local private PANACEA includes
#include "primitives/primitive_attributes.hpp"

// Standard includes
#include <vector>

namespace panacea {
  
  class MemoryManager;
namespace test {

  /*
   * Creates data containing two rows of 
   * vectors with the values: 
   *
   * at(0).at(0) = 1.0 at(0).at(1) = 2.0 at(0).at(2) = 3.0
   * at(1).at(0) = 1.0 at(1).at(1) = 2.0 at(1).at(2) = 3.0
   */ 
  std::vector<std::vector<double>> generateVecData();

  /*
   * Creates data containing two rows of 
   * arrays with the values: 
   *
   * [0][0] = 10 [0][1] = 20 [0][2] = 30
   * [1][0] = 10 [1][1] = 20 [1][2] = 30
   */ 
  class ArrayData {
    public:
      ArrayData();
      ~ArrayData();
      double ** data;
  }; 

  /*
   * Will populate primitive attributes class with
   * appropriate members:
   *
   * - BaseKernelWrapper
   * - ReducedCovariance matrix
   * - ReducedInvCovariance matrix
   */
  //PrimitiveAttributes generatePrimitiveAttributes(::panacea::MemoryManager & mem_manager);

} // test

} // panacea
#endif // PANACEA_PRIVATE_HELPER_H