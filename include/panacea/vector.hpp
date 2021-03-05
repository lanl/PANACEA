
#ifndef PANACEA_VECTOR_H
#define PANACEA_VECTOR_H
#pragma once

namespace panacea {

  class Vector {

    public:

      virtual double& operator()(const int row) = 0;
      virtual double operator()(const int row) const = 0;

      virtual void resize(const int rows) = 0;
  };
}

#endif // PANACEA_VECTOR_H
