
// Local private PANACEA includes
#include "entropy_settings.hpp"
#include "distribution/distribution_settings/kernel_distribution_settings.hpp"
#include "kernels/kernel_specifications.hpp"
#include "private_settings.hpp"

// Standard includes
#include <cassert>
#include <iostream>
#include <typeindex>

using namespace panacea::settings;

namespace panacea {

EntropySettings::EntropySettings(const EntropySettings &entropy_settings) {

  settings::EntropyType type = entropy_settings.type;
  settings::Memory memory_policy = entropy_settings.memory_policy;
  std::optional<double> weight = entropy_settings.weight;
  std::optional<bool> numerical_grad_switch =
      entropy_settings.numerical_grad_switch;
  std::optional<double> numerical_grad_inc =
      entropy_settings.numerical_grad_inc;

  settings::EquationSetting compute_equation_settings =
      entropy_settings.compute_equation_settings;
  settings::EquationSetting grad_equation_settings =
      entropy_settings.grad_equation_settings;

  // Here we will create a heap allocation if the types are not the same.
  // If the types are the same then we can assume that the memory has already
  // been allocated for the members and we can simply copy from instance to the
  // other.
  if (dist_settings == nullptr) {
    dist_settings =
        DistributionSettings::create(*entropy_settings.dist_settings);
  } else if (dist_settings->type() != entropy_settings.dist_settings->type()) {
    dist_settings =
        DistributionSettings::create(*entropy_settings.dist_settings);
  } else {
    // Here because they are the same type we will simply copy the content over
    // without requiring a heap allocation.
    *dist_settings = *entropy_settings.dist_settings;
  }
}

EntropySettings &EntropySettings::
operator=(const EntropySettings &entropy_settings) {

  settings::EntropyType type = entropy_settings.type;
  settings::Memory memory_policy = entropy_settings.memory_policy;
  std::optional<double> weight = entropy_settings.weight;
  std::optional<bool> numerical_grad_switch =
      entropy_settings.numerical_grad_switch;
  std::optional<double> numerical_grad_inc =
      entropy_settings.numerical_grad_inc;

  settings::EquationSetting compute_equation_settings =
      entropy_settings.compute_equation_settings;
  settings::EquationSetting grad_equation_settings =
      entropy_settings.grad_equation_settings;

  // Here we will create a heap allocation if the types are not the same.
  // If the types are the same then we can assume that the memory has already
  // been allocated for the members and we can simply copy from instance to the
  // other.
  if (dist_settings == nullptr) {
    dist_settings =
        DistributionSettings::create(*entropy_settings.dist_settings);
  } else if (dist_settings->type() != entropy_settings.dist_settings->type()) {
    dist_settings =
        DistributionSettings::create(*entropy_settings.dist_settings);
  } else {
    // Here because they are the same type we will simply copy the content over
    // without requiring a heap allocation.
    *dist_settings = *entropy_settings.dist_settings;
  }
  return *this;
}

EntropySettings::EntropySettings(const PANACEASettings &in,
                                 const Memory memory_policy) {
  if (auto val = in.get<EntropyType>()) {
    this->type = *val;
  }
  if (auto val = in.getWeight()) {
    this->weight = *val;
  }
  if (auto val = in.numericalGrad()) {
    this->numerical_grad_switch = *val;
    // Only matters if calculating numerical gradiants
    if (auto val2 = in.getIncrement()) {
      this->numerical_grad_inc = *val2;
    }
  }
  if (auto dist_type = in.get<DistributionType>()) {
    if (dist_type == DistributionType::Kernel) {
      auto kern_dist_settings = std::make_unique<KernelDistributionSettings>();

      if (auto val = in.get<KernelCorrelation>()) {
        kern_dist_settings->dist_settings.set(*val);
      }
      if (auto val = in.get<KernelCount>()) {
        kern_dist_settings->dist_settings.set(*val);
      }
      if (auto val = in.get<KernelPrimitive>()) {
        kern_dist_settings->dist_settings.set(*val);
      }
      if (auto val = in.get<KernelNormalization>()) {
        kern_dist_settings->dist_settings.set(*val);
      }
      if (auto val = in.get<KernelCenterCalculation>()) {
        kern_dist_settings->dist_settings.set(*val);
      }
      if (auto val = in.get<PANACEAAlgorithm>()) {
        if (*val == PANACEAAlgorithm::Flexible) {
          kern_dist_settings->dist_settings.set(KernelAlgorithm::Flexible);
        } else {
          kern_dist_settings->dist_settings.set(KernelAlgorithm::Strict);
        }
      }
      if (this->type == EntropyType::Self) {
        // Make the kernels share the data from the descriptors if it is a
        // Self entropy term, the only time this switches to ownership is if
        // data is loaded from a restart file.
        kern_dist_settings->dist_settings.set(KernelMemory::OwnIfRestart);
      } else {
        kern_dist_settings->dist_settings.set(KernelMemory::Own);

        // If a cross entropy term that is single where the primitive is set
        // to Gaussian or Exponential we are going to Ignore the exponential
        // part when calculating the gradiant because makes the gradiant
        // unstable
        if (in.get<KernelCount>() == KernelCount::Single) {
          grad_equation_settings = EquationSetting::IgnoreExpAndPrefactor;
        }
      }
      this->dist_settings = std::move(kern_dist_settings);
    }
  }
}

void EntropySettings::setDistributionSettings(
    std::unique_ptr<DistributionSettings> dist_settings_in) {
  dist_settings = std::move(dist_settings_in);
}
// We can make method const because we not changing the pointer
const DistributionSettings &
EntropySettings::getDistributionSettings(const Method method) const {
  assert(dist_settings != nullptr);
  if (method == Method::Compute) {
    dist_settings->set(compute_equation_settings);
  } else if (method == Method::ComputeGradiant) {
    dist_settings->set(grad_equation_settings);
  }
  // If it is Method::Create it doesn't matter
  // equation settings are not needed

  return *dist_settings;
}

std::vector<std::any> EntropySettings::write(const settings::FileType file_type,
                                             std::ostream &os,
                                             std::any ent_settings_instance) {

  const EntropySettings &ent_settings = [&]() -> const EntropySettings & {
    if (std::type_index(ent_settings_instance.type()) ==
        std::type_index(typeid(EntropySettings &))) {
      // We are adding const because we are writing data there is no reason
      // that the entropy term needs to be non const
      return const_cast<const EntropySettings &>(
          std::any_cast<EntropySettings &>(ent_settings_instance));

    } else if (std::type_index(ent_settings_instance.type()) ==
               std::type_index(typeid(EntropySettings *))) {
      return const_cast<const EntropySettings &>(
          *std::any_cast<EntropySettings *>(ent_settings_instance));

    } else if (std::type_index(ent_settings_instance.type()) ==
               std::type_index(typeid(const EntropySettings &))) {
      return std::any_cast<const EntropySettings &>(ent_settings_instance);

    } else if (std::type_index(ent_settings_instance.type()) ==
               std::type_index(typeid(const EntropySettings *))) {
      return *std::any_cast<const EntropySettings *>(ent_settings_instance);

    } else {
      PANACEA_FAIL("Unsupported EntropySettings encountered.");
    }
    return std::any_cast<const EntropySettings &>(ent_settings_instance);
  }();

  std::vector<std::any> nested_values;
  if (file_type == settings::FileType::TXTRestart) {

    os << "[Entropy Settings]\n";
    os << ent_settings.type << "\n";
    os << ent_settings.memory_policy << "\n";
    if (auto weight = ent_settings.weight) {
      os << *weight << "\n";
    } else {
      os << "N/A\n";
    }
    if (auto grad_switch = ent_settings.numerical_grad_switch) {
      os << *grad_switch << "\n";
    } else {
      os << "N/A\n";
    }
    if (auto grad_inc = ent_settings.numerical_grad_inc) {
      os << *grad_inc << "\n";
    } else {
      os << "N/A\n";
    }
    os << ent_settings.compute_equation_settings << "\n";
    os << ent_settings.grad_equation_settings << "\n";
  }
  return nested_values;
}

io::ReadInstantiateVector
EntropySettings::read(const settings::FileType file_type, std::istream &is,
                      std::any ent_settings_instance) {

  EntropySettings &ent_settings = [&]() -> EntropySettings & {
    if (std::type_index(ent_settings_instance.type()) ==
        std::type_index(typeid(EntropySettings &))) {
      return std::any_cast<EntropySettings &>(ent_settings_instance);
    } else if (std::type_index(ent_settings_instance.type()) ==
               std::type_index(typeid(EntropySettings *))) {
      return *std::any_cast<EntropySettings *>(ent_settings_instance);
    } else {
      PANACEA_FAIL("Unsupported EntropySettings encountered.");
    }
    return std::any_cast<EntropySettings &>(ent_settings_instance);
  }();

  io::ReadInstantiateVector nested_values;
  if (file_type == settings::FileType::TXTRestart) {
    std::string line = "";
    while (line.find("[Entropy Settings]", 0) == std::string::npos) {
      if (is.peek() == EOF) {
        std::string error_msg = "While reading entropy settings section of ";
        error_msg +=
            "restart file, file does not contain the [Entropy Settings] tag.";
        PANACEA_FAIL(error_msg);
      }
      std::getline(is, line);
    }

    is >> ent_settings.type;
    is >> ent_settings.memory_policy;

    std::getline(is, line);
    if (line.find("N/A", 0) == std::string::npos) {
      ent_settings.weight = std::stod(line);
    }
    std::getline(is, line);
    if (line.find("N/A", 0) == std::string::npos) {
      ent_settings.numerical_grad_switch = std::stoi(line);
    }
    std::getline(is, line);
    if (line.find("N/A", 0) == std::string::npos) {
      ent_settings.numerical_grad_inc = std::stod(line);
    }
    is >> ent_settings.compute_equation_settings;
    is >> ent_settings.grad_equation_settings;
  }
  return nested_values;
}

bool operator==(const EntropySettings &settings1,
                const EntropySettings &settings2) {
  if (settings1.type != settings2.type)
    return false;
  if (settings1.memory_policy != settings2.memory_policy)
    return false;

  // If the weight is not set than we should assume a value of 1.0, because with
  // out the weight decorator it is 1.0
  double weight1 = 1.0;
  if (settings1.weight.has_value()) {
    weight1 = *settings1.weight;
  }
  double weight2 = 1.0;
  if (settings2.weight.has_value()) {
    weight2 = *settings2.weight;
  }
  if (weight1 != weight2)
    return false;

  bool switch1 = false;
  if (settings1.numerical_grad_switch.has_value()) {
    switch1 = *settings1.numerical_grad_switch;
  }
  bool switch2 = false;
  if (settings2.numerical_grad_switch.has_value()) {
    switch2 = *settings2.numerical_grad_switch;
  }
  if (switch1 != switch2)
    return false;

  if (switch1) {
    // If the numerical grad switch is off there is no need to check the
    // increment value
    if (settings1.numerical_grad_inc != settings2.numerical_grad_inc)
      return false;
  }
  return true;
}

bool operator!=(const EntropySettings &settings1,
                const EntropySettings &settings2) {
  return !(settings1 == settings2);
}

} // namespace panacea
