
#ifndef PANACEA_VECTOR_H
#define PANACEA_VECTOR_H
#pragma once

// Public PANACEA includes
#include "panacea/file_io_types.hpp"
#include "panacea/settings.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <memory>
#include <vector>

namespace panacea {
  enum class VectorType {
    Default,
    Eigen
  };

  enum class Direction;

  class Vector {

    private:
      virtual void direction(const Direction & direction) = 0;

    public:
      virtual ~Vector() {};
      virtual const VectorType type() const = 0;
      virtual Vector& operator=(const Vector & vec) = 0;
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

      static std::vector<std::any> write(
          const settings::FileType file_type,
          std::ostream &,
          std::any vector_instance);

      static io::ReadInstantiateVector read(
          const settings::FileType file_type,
          std::istream &,
          std::any vector_instance);
  };

  std::ostream& operator<<(std::ostream& os, const VectorType& vec_type);

  std::unique_ptr<Vector> createVector(const int rows, const VectorType type = VectorType::Default);

}

#endif // PANACEA_VECTOR_H
