
#ifndef PANACEA_PRIVATE_COVARIANCE_H
#define PANACEA_PRIVATE_COVARIANCE_H
#pragma once

// Third party includes
#include <Eigen/Dense>

// Local PANACEA includes
#include "panacea/matrix.hpp"
#include "panacea/vector.hpp"

namespace panacea {

  class BaseDescriptorWrapper;

  /*
   * Class for storing the covariance matrix
   */
  class Covariance {
    private:
      /// The covariance matrix
      Matrix matrix_;
      
      /// A is used for updating the covariance matrix
      Matrix A_;

      /// B is used for updating the covariance matrix
      Vector B_;

      /// Mean of the values used to create the covariance matrix
      Vector mean_;

      /// Total number of data points used in the creation of the covariance matrix and in updating it
      int total_number_data_pts_ = 0;
    public:
      Covariance(BaseDescriptorWrapper * desc_wrap);
      /// Designed to update the covariance matrix
      void update(BaseDescriptorWrapper * desc_wrap);
  }
}
#define PANACEA_PRIVATE_COVARIANCE_H
