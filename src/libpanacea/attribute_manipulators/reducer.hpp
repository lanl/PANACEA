#ifndef PANACEA_PRIVATE_REDUCER_H
#define PANACEA_PRIVATE_REDUCER_H
#pragma once

// Local private PANACEA includes
#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"

// Standard includes
#include <vector>

namespace panacea {

  class Reducer {
    private:
      /*
       * The starting theshold is used to determine when linear dependence is 
       * occuring. The lower this value is the less likely that linear dependence
       * will be detected, initially. However, the threshold is iteratively 
       * increased until the determinant is > 0.
       */   
      double starting_threshold_ = 1E-9;

    public:
      Reducer() = default;
      explicit Reducer(double threshold) : starting_threshold_(threshold) {}; 
      /**
       * @brief Designed to reduce/rearrange the covariance matrix
       *
       * Sometimes it is impossible to create the inverse of a covariance 
       * matrix, e.g. if the matrix has linearly dependent terms. 
       *
       * The purpose of the Reducer class is to eliminate dimensions that lead
       * to linear dependence while prioritizing the dimensions as they are 
       * passed in with the preferred_dimensions argument.
       *
       * @param cov
       * @param preferred_dimensions
       *
       * @return 
       */
      ReducedCovariance reduce(
          const Covariance & cov,
          std::vector<int> preferred_dimensions) const;
  };
}

#endif // PANACEA_PRIVATE_REDUCER_H
