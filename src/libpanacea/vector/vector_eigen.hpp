
#ifndef PANACEA_PRIVATE_VECTOREIGEN_H
#define PANACEA_PRIVATE_VECTOREIGEN_H
#pragma once

// Local PANACEA includes
#include "panacea/vector.hpp"

// Third party includes
#include <Eigen/Dense>

// Third party includes
#include <memory>

namespace panacea {

  /**
   * Here the underlying vector is owned by the memory manager so we only have a pointer
   * here.
   */
  class VectorEigen : public Vector {
    private: 
      std::unique_ptr<Eigen::VectorXd> vector_;
    public:
      VectorEigen();
      virtual ~VectorEigen() final {};
      virtual VectorEigen& operator=(const Vector * vec) final;
      virtual double& operator()(const int row) final;
      virtual double operator()(const int row) const final;
      virtual void resize(const int rows) final;
      virtual int rows() const final;
  };
}

#endif // PANACEA_PRIVATE_VECTOREIGEN_H
