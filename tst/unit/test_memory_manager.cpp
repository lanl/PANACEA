
// Local private includes
#include "memory.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <iostream>
#include <vector>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:memory_manager constructor","[unit,panacea]"){
  MemoryManager mem_manager;
}

TEST_CASE("Testing:memory_manager managePointer","[unit,panacea]"){

  WHEN("Using a std vector"){
    MemoryManager mem_manager;

    std::vector<double> some_data = { 1.0, 2.0, 3.0 };

    // Data in nodes is not owned by mem_manager
    size_t index = mem_manager.managePointer(some_data);
    auto vec_ptr = mem_manager.getRawPointer<std::vector<double> *>(index);  

    // Should have the same address
    REQUIRE(vec_ptr == &some_data );
  }
  WHEN("Using heap allocated memory"){
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

    MemoryManager mem_manager;
    // Data in nodes is not owned by mem_manager
    double *** data_ptr = &data;
    size_t index = mem_manager.managePointer(data_ptr);
    auto data_ptr1 = mem_manager.getRawPointer<double ***>(index);  

    // Should have the same address
    REQUIRE(data_ptr == data_ptr1 );

    delete[] data[0];
    delete[] data[1];
    delete[] data;
  }
}

TEST_CASE("Testing:memory_manager manageMemory","[unit,panacea]"){

  WHEN("Using a std vector"){
    MemoryManager mem_manager;

    std::vector<double> some_data = { 1.0, 2.0, 3.0 };

    // Data in nodes is not owned by mem_manager
    size_t index = mem_manager.manageMemory(std::make_unique<std::vector<double>>(some_data));
    auto vec_ptr = mem_manager.getRawPointer<std::vector<double> *>(index);  

    // Should have the same address
    REQUIRE(vec_ptr != &some_data );

    // Ensure the data is there though
    REQUIRE(vec_ptr->at(0) == Approx(1.0));
    REQUIRE(vec_ptr->at(1) == Approx(2.0));
    REQUIRE(vec_ptr->at(2) == Approx(3.0));
  }

}
