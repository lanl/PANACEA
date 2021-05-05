
#ifndef PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#define PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#pragma once

// Local private includes
#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_specifications.hpp"
#include "primitive.hpp"
#include "primitive_attributes.hpp"

// Standard includes
#include <memory>
#include <string>
#include <vector>

namespace panacea {

  class Normalizer;

  /**
   * The specifications are fixed for each primitive group
   **/
  class PrimitiveGroup {
    private:
      KernelSpecification specification;
    public:
      PrimitiveGroup() = default;
      explicit PrimitiveGroup(const KernelSpecification & specific) : specification(specific) {};

      const KernelSpecification & getSpecification() { return specification; }
      std::string name = "";
      Normalizer normalizer;
      std::unique_ptr<BaseKernelWrapper> kernel_wrapper = nullptr;
      std::unique_ptr<Covariance> covariance = nullptr;
      std::unique_ptr<ReducedCovariance> reduced_covariance = nullptr;
      std::unique_ptr<ReducedInvCovariance> reduced_inv_covariance = nullptr;
      std::vector<std::unique_ptr<Primitive>> primitives;

      PrimitiveAttributes createPrimitiveAttributes() noexcept;

      void update(
          const BaseDescriptorWrapper * dwrapper);

      static std::vector<std::any> write(
          const settings::FileType & file_type,
          std::ostream &,
          std::any prim_grp_instance);

      static std::vector<std::any> read(
          const settings::FileType & file_type,
          std::istream &,
          std::any prim_grp_instance);

      static void postReadInitialization(
          const settings::FileType & file_type,
          std::any prim_grp_instance);

  };
}

#endif // PANACEA_PRIVATE_PRIMITIVE_GROUP_H
