
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "data_point_template.hpp"

// Public PANACEA includes
#include "panacea/file_io_types.hpp"

// Standard includes
#include <any>
#include <iomanip>
#include <iostream>
#include <vector>

namespace panacea {

std::vector<std::any>
BaseDescriptorWrapper::write(const settings::FileType file_type,
                             std::ostream &os, std::any dwrapper_instance) {

  std::vector<std::any> nested_values;
  if (file_type == settings::FileType::TXTDescriptors) {
    auto dwrapper = std::any_cast<BaseDescriptorWrapper *>(dwrapper_instance);
    os << "Descriptors\n";
    os << dwrapper->arrangement() << "\n";
    os << dwrapper->rows() << " " << dwrapper->cols() << "\n";
    for (int row = 0; row < dwrapper->rows(); ++row) {
      for (int col = 0; col < dwrapper->cols(); ++col) {
        os << std::setfill(' ') << std::setw(14) << std::setprecision(8)
           << std::right << dwrapper->operator()(row, col) << " ";
      }
      os << "\n";
    }
  } else {
    std::string error_msg =
        "Descriptors cannot be written to the specified file type.";
    PANACEA_FAIL(error_msg);
  }
  return nested_values;
}

io::ReadInstantiateVector
BaseDescriptorWrapper::read(const settings::FileType file_type,
                            std::istream &is, std::any dwrapper_instance) {

  if (file_type == settings::FileType::TXTDescriptors) {
    auto dwrapper = std::any_cast<BaseDescriptorWrapper *>(dwrapper_instance);

    // Before doing any reading ensure that the underlying descriptor data type
    // is actually compatible, right now the Descriptor must be of type
    // std::vector<std::vector<double>> as in the data must be owned by the
    // descriptor wrapper and not simply a pointer to it
    if (dwrapper->getTypeIndex() !=
        std::type_index(typeid(std::vector<std::vector<double>>))) {
      std::string error_msg =
          "Reading descriptor data in requires that the descriptor ";
      error_msg += "wrapper actually own the data so the underlying type must "
                   "be of type";
      error_msg += " std::vector<std:vector<double>>, it cannot be a pointer.";
    }

    std::string line;
    std::getline(is, line);
    // First line should be header
    if (line.find("Descriptors", 0) == std::string::npos) {
      std::cout
          << "Warning while reading descriptor file, header does not contain ";
      std::cout << "the Descriptors word, which is typically placed at the top "
                   "of the";
      std::cout << "descriptors text files. Attempting to read.\n";
      std::cout << "line is: " << line << "\n";
    }

    // Second line should indicate the arrangement
    std::getline(is, line);
    if (line.find("Points along rows dimensions along columns", 0) !=
        std::string::npos) {
      dwrapper->set(Arrangement::PointsAlongRowsDimensionsAlongCols);
    } else if (line.find("Dimensions along rows points along columns", 0) !=
               std::string::npos) {
      dwrapper->set(Arrangement::DimensionsAlongRowsPointsAlongCols);
    } else {
      std::cout
          << "Warning alignment line missing from descriptors txt file. The ";
      std::cout << "available options are:\n";
      std::cout << "Points along rows dimensions along columns\n";
      std::cout << "Dimensions along rows points along columns\n";
      std::cout << "\nAssuming:\n";
      std::cout << "Points along rows dimensions along columns\n";
      dwrapper->set(Arrangement::PointsAlongRowsDimensionsAlongCols);
    }

    std::getline(is, line);
    std::istringstream ss(line);

    int rows;
    try {
      ss >> rows;
    } catch (...) {
      std::string error_msg = "Unable to read in rows from Descriptors file.\n";
      error_msg += "line is: " + line + "\n";
      PANACEA_FAIL(error_msg);
    }
    int cols;
    try {
      ss >> cols;
    } catch (...) {
      std::string error_msg =
          "Unable to read in columns from Descriptors file.\n";
      error_msg += "line is: " + line + "\n";
      PANACEA_FAIL(error_msg);
    }

    try {

      dwrapper->resize(rows, cols);
      for (int row = 0; row < rows; ++row) {
        std::getline(is, line);
        std::istringstream ss_data(line);
        for (int col = 0; col < cols; ++col) {
          double value;
          ss_data >> value;
          dwrapper->operator()(row, col) = value;
        }
      }
    } catch (...) {
      std::string error_msg =
          "Error encountered while attempting to read in descriptor ";
      error_msg += "coefficients from Descriptor data file.\n";
      error_msg += "line is: " + line + "\n";
      PANACEA_FAIL(error_msg);
    }

  } else {
    std::string error_msg =
        "Descriptors cannot be written to the specified file type.";
    PANACEA_FAIL(error_msg);
  }
  io::ReadInstantiateVector nested_values;
  return nested_values;
}

BaseDescriptorWrapper::~BaseDescriptorWrapper(){};
} // namespace panacea
