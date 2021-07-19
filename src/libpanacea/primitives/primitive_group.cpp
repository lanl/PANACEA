
// Local private includes
#include "primitive_group.hpp"

#include "attribute_manipulators/inverter.hpp"
#include "attribute_manipulators/normalizer.hpp"
#include "attribute_manipulators/reducer.hpp"
#include "attributes/covariance.hpp"
#include "error.hpp"
#include "kernels/base_kernel_wrapper.hpp"
#include "kernels/kernel_wrapper_factory.hpp"
#include "primitive_attributes.hpp"
#include "primitive_factory.hpp"
#include "type_map.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

namespace panacea {

namespace {
static std::unique_ptr<Normalizer>
createNormalizer(const KernelSpecification &specification) {

  if (specification.is(settings::KernelAlgorithm::Strict)) {
    return std::make_unique<Normalizer>(
        specification.get<settings::KernelNormalization>(),
        NormalizerOption::Strict);
  }
  // Flexible option will avoid errors if coefficients are 0.0,
  // e.g. if variance is 0.0, will set such coefficients to 1.0
  return std::make_unique<Normalizer>(
      specification.get<settings::KernelNormalization>(),
      NormalizerOption::Flexible);
}
} // namespace

PrimitiveAttributes PrimitiveGroup::createPrimitiveAttributes() noexcept {

  assert(this->kernel_wrapper != nullptr);
  assert(this->covariance != nullptr);
  assert(this->reduced_covariance != nullptr);
  assert(this->reduced_inv_covariance != nullptr);

  return PrimitiveAttributes{
      .normalizer = this->normalizer.get(), // must return address and not copy
      .kernel_wrapper = this->kernel_wrapper.get(),
      .covariance = this->covariance.get(),
      .reduced_covariance = this->reduced_covariance.get(),
      .reduced_inv_covariance = this->reduced_inv_covariance.get()};
}

void PrimitiveGroup::update(const BaseDescriptorWrapper &dwrapper) {

  PrimitiveFactory prim_factory;
  prim_factory.update(PassKey<PrimitiveGroup>(), dwrapper, *this);
}

void PrimitiveGroup::initialize(const BaseDescriptorWrapper &dwrapper) {

  PrimitiveFactory prim_factory;
  prim_factory.initialize(PassKey<PrimitiveGroup>(), dwrapper, *this);
}

std::vector<std::any> PrimitiveGroup::write(const settings::FileType file_type,
                                            std::ostream &os,
                                            std::any prim_grp_instance) {

  const PrimitiveGroup &prim_grp = [&]() -> const PrimitiveGroup & {
    if (std::type_index(prim_grp_instance.type()) ==
        std::type_index(typeid(PrimitiveGroup *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return const_cast<const PrimitiveGroup &>(
          *std::any_cast<PrimitiveGroup *>(prim_grp_instance));

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(PrimitiveGroup &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return const_cast<const PrimitiveGroup &>(
          std::any_cast<PrimitiveGroup &>(prim_grp_instance));

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(const PrimitiveGroup *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return *std::any_cast<const PrimitiveGroup *>(prim_grp_instance);

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(const PrimitiveGroup &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return std::any_cast<const PrimitiveGroup &>(prim_grp_instance);
    } else {
      std::string error_msg = "Unable to cast to PrimitiveGroup * while";
      error_msg += " trying to write primitive group to restart file.";
      PANACEA_FAIL(error_msg);
    }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return std::any_cast<const PrimitiveGroup &>(prim_grp_instance);
  }();

  std::vector<std::any> nested_values;
  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    os << "[Primitive Group]\n";
    os << prim_grp.name << "\n";
    nested_values = std::vector<std::any>{
        &prim_grp.specification, prim_grp.normalizer.get(),
        prim_grp.kernel_wrapper.get(), prim_grp.covariance.get()};
  }

  return nested_values;
}

io::ReadInstantiateVector
PrimitiveGroup::read(const settings::FileType file_type, std::istream &is,
                     std::any prim_grp_instance) {

  PrimitiveGroup &prim_grp = [&]() -> PrimitiveGroup & {
    if (std::type_index(prim_grp_instance.type()) ==
        std::type_index(typeid(PrimitiveGroup *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return *std::any_cast<PrimitiveGroup *>(prim_grp_instance);

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(PrimitiveGroup &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return std::any_cast<PrimitiveGroup &>(prim_grp_instance);

    } else {
      std::string error_msg = "Unable to cast to PrimitiveGroup * while";
      error_msg += " trying to write primitive group to restart file.";
      PANACEA_FAIL(error_msg);
    }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return std::any_cast<PrimitiveGroup &>(prim_grp_instance);
  }();

  io::ReadInstantiateVector nested_values;
  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    std::string line = "";
    while (line.find("[Primitive Group]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg =
            "Did not find [Primitive Group] header while trying ";
        error_msg += "to read in primitive group from restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }
    std::getline(is, line);
    prim_grp.name = line;

    nested_values.emplace_back(&prim_grp.specification,
                               postReadKernelSpecsInitialization);
    nested_values.emplace_back(&(prim_grp.normalizer), std::nullopt);

    // Data of objects created with unique pointers need to first be created but
    // after the specifications have been read in
    nested_values.emplace_back(&(prim_grp.kernel_wrapper), std::nullopt);

    // Note we must use pointers to the unique_pointers because the memory has
    // not yet been allocated
    nested_values.emplace_back(&(prim_grp.covariance), std::nullopt);
  }

  return nested_values;
}

void PrimitiveGroup::postReadKernelSpecsInitialization(
    std::any prim_grp_instance) {

  PrimitiveGroup &prim_grp = [&]() -> PrimitiveGroup & {
    if (std::type_index(prim_grp_instance.type()) ==
        std::type_index(typeid(PrimitiveGroup &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return std::any_cast<PrimitiveGroup &>(prim_grp_instance);

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(PrimitiveGroup *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return *std::any_cast<PrimitiveGroup *>(prim_grp_instance);
    } else {
      std::string error_msg = "Unable to cast to PrimitiveGroup * while";
      error_msg += " trying to run postReadKernelSpecsInitialization method.";
      if (type_map.count(std::type_index(prim_grp_instance.type()))) {
        error_msg += "\nType is: " +
                     type_map.at(std::type_index(prim_grp_instance.type()));
      }
      PANACEA_FAIL(error_msg);
    }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return std::any_cast<PrimitiveGroup &>(prim_grp_instance);
  }();

  // Now that we have read in the kernel specs we should switch the ownership of
  // the kernel data if it is set as share to own
  if (prim_grp.specification.template get<settings::KernelMemory>() ==
      settings::KernelMemory::Share) {
    std::cout
        << "Note restart file indicates kernel memory was in the shared state.";
    std::cout << " Because we do not have access to the descriptor wrapper ";
    std::cout << "class and its heap memory while reading the data from a ";
    std::cout << "restart file kernel memory management is being switched ";
    std::cout << "to the own state.\n";
    prim_grp.specification.set(settings::KernelMemory::Own);
  }

  prim_grp.normalizer = createNormalizer(prim_grp.specification);
  // Now we need to actually create the kernels and covariance matrix because
  // they are allocated with unique pointers
  prim_grp.covariance = Covariance::create(
      prim_grp.specification.get<settings::KernelCorrelation>());

  KernelWrapperFactory kfactory;
  // Essentially create a kernel wrapper shell.
  prim_grp.kernel_wrapper = kfactory.create(prim_grp.specification);
  assert(prim_grp.kernel_wrapper.get() != nullptr);
}

void PrimitiveGroup::postReadInitialization(const settings::FileType file_type,
                                            std::any prim_grp_instance) {

  PrimitiveGroup &prim_grp = [&]() -> PrimitiveGroup & {
    if (std::type_index(prim_grp_instance.type()) ==
        std::type_index(typeid(PrimitiveGroup &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return std::any_cast<PrimitiveGroup &>(prim_grp_instance);

    } else if (std::type_index(prim_grp_instance.type()) ==
               std::type_index(typeid(PrimitiveGroup *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return *std::any_cast<PrimitiveGroup *>(prim_grp_instance);

    } else {
      std::string error_msg = "Unable to cast to PrimitiveGroup * while";
      error_msg += " trying to run postReadInitialization method.";
      if (type_map.count(std::type_index(prim_grp_instance.type()))) {
        error_msg += "\nType is: " +
                     type_map.at(std::type_index(prim_grp_instance.type()));
      }
      PANACEA_FAIL(error_msg);
    }
    std::cout << __FILE__ << ":" << __LINE__ << std::endl;
    return std::any_cast<PrimitiveGroup &>(prim_grp_instance);
  }();

  if (file_type == settings::FileType::TXTRestart) {
    PrimitiveFactory prim_factory;
    prim_factory.reset(PassKey<PrimitiveGroup>(), prim_grp);
  }
}
} // namespace panacea
