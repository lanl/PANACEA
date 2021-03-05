
#ifndef PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#define PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#pragma once

namespace panacea {
  
  class Matrix;
  class Vector
  class BaseKernelWrapper;
  /*
   * Class provides a generic means of passing in common attributes needed by the primitives
   */
  class PrimitiveAttributes {
    public:
      BaseKernelWrapper * kernel_wrapper;
      ReducedInvCovariance * reduced_inv_covariance;       
      double determinant;
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
