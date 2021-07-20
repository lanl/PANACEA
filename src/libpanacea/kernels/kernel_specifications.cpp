
// Local private PANACEA includes
#include "kernel_specifications.hpp"

#include "error.hpp"
#include "private_settings.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <iostream>
#include <string>
#include <typeindex>
#include <vector>

namespace panacea {

std::vector<std::any>
KernelSpecification::write(const settings::FileType file_type, std::ostream &os,
                           std::any kern_spec_instance) {

  const KernelSpecification &kern_spec = [&]() -> const KernelSpecification & {
    if (std::type_index(kern_spec_instance.type()) ==
        std::type_index(typeid(KernelSpecification *))) {
      return const_cast<const KernelSpecification &>(
          *std::any_cast<KernelSpecification *>(kern_spec_instance));
    } else if (std::type_index(kern_spec_instance.type()) ==
               std::type_index(typeid(KernelSpecification &))) {
      return const_cast<const KernelSpecification &>(
          std::any_cast<KernelSpecification &>(kern_spec_instance));
    } else if (std::type_index(kern_spec_instance.type()) ==
               std::type_index(typeid(const KernelSpecification *))) {
      return *std::any_cast<const KernelSpecification *>(kern_spec_instance);
    } else if (std::type_index(kern_spec_instance.type()) ==
               std::type_index(typeid(const KernelSpecification &))) {
      return std::any_cast<const KernelSpecification &>(kern_spec_instance);
    } else {
      std::string error_msg = "Unable to cast to KernelSpecification * while";
      error_msg += " trying to write kernel specs to restart file.";
      PANACEA_FAIL(error_msg);
    }
    return std::any_cast<const KernelSpecification &>(kern_spec_instance);
  }();

  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {
    os << "[Kernel Specifications]\n";
    os << kern_spec.kern_correlation_ << "\n";
    os << kern_spec.kern_count_ << "\n";
    os << kern_spec.kern_prim_ << "\n";
    os << kern_spec.kern_normalization_ << "\n";
    os << kern_spec.kern_memory_ << "\n";
    os << kern_spec.kern_center_ << "\n";
    os << kern_spec.kern_algorithm_ << "\n";
    os << kern_spec.randomize_dims_ << "\n";
    os << kern_spec.randomize_num_dims_ << "\n";
    os << kern_spec.max_number_dimensions_ << "\n";
    os << "\n";
  }
  return std::vector<std::any>{};
}

io::ReadInstantiateVector
KernelSpecification::read(const settings::FileType file_type, std::istream &is,
                          std::any kern_spec_instance) {

  KernelSpecification &kern_spec = [&]() -> KernelSpecification & {
    if (std::type_index(kern_spec_instance.type()) ==
        std::type_index(typeid(KernelSpecification *))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return *std::any_cast<KernelSpecification *>(kern_spec_instance);
    } else if (std::type_index(kern_spec_instance.type()) ==
               std::type_index(typeid(KernelSpecification &))) {
      std::cout << __FILE__ << ":" << __LINE__ << std::endl;
      return (std::any_cast<KernelSpecification &>(kern_spec_instance));
    } else {
      std::string error_msg = "Unable to cast to KernelSpecification * while";
      error_msg += " trying to read kernel specs to restart file.";
      PANACEA_FAIL(error_msg);
    }
    return (std::any_cast<KernelSpecification &>(kern_spec_instance));
  }();

  if (file_type == settings::FileType::TXTRestart ||
      file_type == settings::FileType::TXTKernelDistribution) {

    std::string line = "";
    while (line.find("[Kernel Specifications]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg =
            "Did not find [Kernel Specifications] header while trying ";
        error_msg += "to read in kernel specifications from restart file.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }
    is >> kern_spec.kern_correlation_;
    is >> kern_spec.kern_count_;
    is >> kern_spec.kern_prim_;
    is >> kern_spec.kern_normalization_;
    is >> kern_spec.kern_memory_;
    is >> kern_spec.kern_center_;
    is >> kern_spec.kern_algorithm_;
    is >> kern_spec.randomize_dims_;
    is >> kern_spec.randomize_num_dims_;
    is >> kern_spec.max_number_dimensions_;
  }
  return io::ReadInstantiateVector();
}

bool operator==(const KernelSpecification &spec1,
                const KernelSpecification &spec2) {
  if (spec2.kern_correlation_ != spec1.kern_correlation_)
    return false;
  if (spec2.kern_count_ != spec1.kern_count_)
    return false;
  if (spec2.kern_prim_ != spec1.kern_prim_)
    return false;
  if (spec2.kern_normalization_ != spec1.kern_normalization_)
    return false;
  if (spec2.kern_memory_ != spec1.kern_memory_)
    return false;
  if (spec2.kern_center_ != spec1.kern_center_)
    return false;
  if (spec2.kern_algorithm_ != spec1.kern_algorithm_)
    return false;
  if (spec2.randomize_num_dims_ != spec1.randomize_num_dims_)
    return false;
  if (spec2.randomize_dims_ != spec1.randomize_dims_)
    return false;
  if (spec2.max_number_dimensions_ != spec1.max_number_dimensions_)
    return false;
  return true;
}

bool operator!=(const KernelSpecification &spec1,
                const KernelSpecification &spec2) {
  return !(spec1 == spec2);
}

bool operator==(const std::unique_ptr<KernelSpecification> &spec1,
                const std::unique_ptr<KernelSpecification> &spec2) {
  return *spec1.get() == *spec2.get();
}

bool operator!=(const std::unique_ptr<KernelSpecification> &spec1,
                const std::unique_ptr<KernelSpecification> &spec2) {
  return *spec1.get() != *spec2.get();
}
} // namespace panacea
