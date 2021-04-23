
#ifndef PANACEA_VECTOR_H
#define PANACEA_VECTOR_H
#pragma once

// Standard includes
#include <memory>

namespace panacea {
  enum class VectorTypes {
    Default,
    Eigen
  };
  
  enum class Direction;

  class Vector {

    public:
      virtual ~Vector() {};
      virtual Vector& operator=(const Vector * vec) = 0;
      virtual double& operator()(const int row) = 0;
      virtual double operator()(const int row) const = 0;

      /**
       * Is it a row or column matrix 
       **/
      virtual Direction direction() const = 0;

      /**
       * Does not guarantee values are set to 0.
       **/
      virtual void resize(const int rows) = 0;

      /**
       * Sets all values in the vector equal to 0
       **/
      virtual void setZero() = 0;

      virtual int rows() const = 0;
      virtual int cols() const = 0;

      virtual void print() const = 0;
  };

  std::unique_ptr<Vector> createVector(const int rows, const VectorTypes type = VectorTypes::Default);

}

#endif // PANACEA_VECTOR_H
