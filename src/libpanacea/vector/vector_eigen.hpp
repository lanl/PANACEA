
#ifndef PANACEA_PRIVATE_VECTOREIGEN_H
#define PANACEA_PRIVATE_VECTOREIGEN_H
#pragma once

// Local PANACEA includes
#include "panacea/vector.hpp"

// Third party includes
#include <Eigen/Dense>

namespace panacea {

  /**
   * Here the underlying vector is owned by the memory manager so we only have a pointer
   * here.
   */
  class VectorEigen : public Vector {
    private: 
      Eigen::VectorXd * vector_;
    public:
      VectorEigen(Eigen::VectorXd * vector) : vector_(vector) {};

      virtual double& operator()(const int row) final;
      virtual double operator()(const int row) const final;
      virtual void resize(const int rows) final;
  };
}

#endif // PANACEA_PRIVATE_VECTOREIGEN_H
