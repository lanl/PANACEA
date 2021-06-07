#ifndef PANACEA_PRIVATE_VECTOREIGEN_H
#define PANACEA_PRIVATE_VECTOREIGEN_H
#pragma once

// Local PANACEA includes
#include "vector.hpp"

#include "data_settings.hpp"

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
      Direction direction_ = Direction::AlongRows;
      std::unique_ptr<Eigen::VectorXd> vector_;
      virtual void direction(const Direction & direction) final { direction_ = direction; }
    public:
      VectorEigen();
      virtual ~VectorEigen() final {};
      virtual const VectorType type() const final;
      virtual VectorEigen& operator=(const Vector * vec) final;
      virtual double& operator()(const int index) final;
      virtual double operator()(const int index) const final;
      virtual Direction direction() const final;
      virtual void resize(const int size) final;
      virtual void setZero() final;
      virtual int rows() const final;
      virtual int cols() const final;
      virtual void print() const final;
  };
}

#endif // PANACEA_PRIVATE_VECTOREIGEN_H
