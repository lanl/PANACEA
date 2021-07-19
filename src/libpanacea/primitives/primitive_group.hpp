
#ifndef PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#define PANACEA_PRIVATE_PRIMITIVE_GROUP_H
#pragma once

// Local private includes
#include "attribute_manipulators/normalizer.hpp"
#include "attributes/covariance.hpp"
#include "attributes/reduced_covariance.hpp"
#include "attributes/reduced_inv_covariance.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_specifications.hpp"
#include "primitive.hpp"
#include "primitive_attributes.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <memory>
#include <string>
#include <vector>

namespace panacea {

class Normalizer;

/**
 * The specifications are fixed for each primitive group
 *
 * Note that each of the attributes used by a kernel must be a unique pointer,
 * the reason being that when we call move on an entropy term it will
 * invalidate any of the internal pointers, if they are not invaiant. Using
 * unique pointers ensures that the data will be invariant.
 *
 * It is not necessary to make the kernel specifications a unique pointer
 * because there are no pointers to the kernel specifications
 **/
class PrimitiveGroup {
private:
  KernelSpecification specification;

public:
  PrimitiveGroup() = default;
  explicit PrimitiveGroup(const KernelSpecification &specific)
      : specification(specific){};

  const KernelSpecification &getSpecification() { return specification; }
  std::string name = "";
  std::unique_ptr<Normalizer> normalizer = nullptr;
  std::unique_ptr<BaseKernelWrapper> kernel_wrapper = nullptr;
  std::unique_ptr<Covariance> covariance = nullptr;
  std::unique_ptr<ReducedCovariance> reduced_covariance = nullptr;
  std::unique_ptr<ReducedInvCovariance> reduced_inv_covariance = nullptr;
  std::vector<std::unique_ptr<Primitive>> primitives;

  PrimitiveAttributes createPrimitiveAttributes() noexcept;

  void update(const BaseDescriptorWrapper &dwrapper);

  void initialize(const BaseDescriptorWrapper &dwrapper);

  static std::vector<std::any> write(const settings::FileType file_type,
                                     std::ostream &,
                                     std::any prim_grp_instance);

  static io::ReadInstantiateVector read(const settings::FileType file_type,
                                        std::istream &,
                                        std::any prim_grp_instance);

  static void postReadInitialization(const settings::FileType file_type,
                                     std::any prim_grp_instance);

  static void postReadKernelSpecsInitialization(std::any obj);
};
} // namespace panacea

#endif // PANACEA_PRIVATE_PRIMITIVE_GROUP_H
