
#ifndef PANACEA_PRIVATE_BASEDESCRIPTORWRAPPER_H
#define PANACEA_PRIVATE_BASEDESCRIPTORWRAPPER_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  enum class Arrangement;
  /*
   * Base Descriptor interface
   */
  class BaseDescriptorWrapper {
    public:
      virtual double& operator()(const int point_ind, const int dim_ind) = 0;
      virtual double operator()(const int point_ind, const int dim_ind) const = 0;
      virtual int rows() const = 0;
      virtual int cols() const = 0;
      virtual int getNumberDimensions() const = 0; 
      virtual int getNumberPoints() const = 0; 
      virtual void set(const Arrangement arrangement) = 0;
      virtual void setReducedNumberDimensions(std::vector<int> reduced_chosen_dimensions) = 0;
      virtual const std::vector<int> getReducedDimensions() const = 0;
      virtual const size_t getNumberReducedDimensions() const = 0;
      virtual void * getPointerToRawData() noexcept = 0;
  };
}
#endif // PANACEA_PRIVATE_BASEDESCRIPTORWRAPPER_H