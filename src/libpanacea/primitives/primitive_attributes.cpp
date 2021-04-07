
// Local private includes
#include "primitive_attributes.hpp"

namespace panacea {

  PrimitiveAttributes::PrimitiveAttributes( PrimitiveAttributes & prim_attr){
    normalizer = prim_attr.normalizer;
    kernel_wrapper = prim_attr.kernel_wrapper;
    covariance = std::move(prim_attr.covariance);
    reduced_covariance = std::move(prim_attr.reduced_covariance);
    reduced_inv_covariance = std::move(prim_attr.reduced_inv_covariance);
  }

  PrimitiveAttributes::PrimitiveAttributes( PrimitiveAttributes && prim_attr){
    normalizer = prim_attr.normalizer;
    kernel_wrapper = prim_attr.kernel_wrapper;
    covariance = std::move(prim_attr.covariance);
    reduced_covariance = std::move(prim_attr.reduced_covariance);
    reduced_inv_covariance = std::move(prim_attr.reduced_inv_covariance);
  }

  PrimitiveAttributes & PrimitiveAttributes::operator=( PrimitiveAttributes & prim_attr){
    normalizer = prim_attr.normalizer;
    kernel_wrapper = prim_attr.kernel_wrapper;
    covariance = std::move(prim_attr.covariance);
    reduced_covariance = std::move(prim_attr.reduced_covariance);
    reduced_inv_covariance = std::move(prim_attr.reduced_inv_covariance);
    return *this;
  }

  PrimitiveAttributes & PrimitiveAttributes::operator=( PrimitiveAttributes && prim_attr){
    normalizer = prim_attr.normalizer;
    kernel_wrapper = prim_attr.kernel_wrapper;
    covariance = std::move(prim_attr.covariance);
    reduced_covariance = std::move(prim_attr.reduced_covariance);
    reduced_inv_covariance = std::move(prim_attr.reduced_inv_covariance);
    return *this;
  }
}
