
#ifndef PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#define PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
#pragma once

namespace panacea {
  
  class BaseKernelWrapper;
  class ReducedCovariance;
  class ReducedInvCovariance;

  /*
   * Class provides a generic means of passing in common attributes needed by the primitives
   */
  class PrimitiveAttributes {
    public:
      BaseKernelWrapper * kernel_wrapper = nullptr;
      ReducedCovariance * reduced_covariance = nullptr;
      ReducedInvCovariance * reduced_inv_covariance = nullptr;       
  };
}

#endif // PANACEA_PRIVATE_PRIMITIVEATTRIBUTES_H
