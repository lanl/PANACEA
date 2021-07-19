
// Local private PANACEA includes
#include "file_txt_descriptor.hpp"

#include "error.hpp"

// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"
#include "panacea/file_io_types.hpp"

// Standard includes
#include <fstream>
#include <iostream>

namespace panacea {

namespace io {
/***************************************************************
 * Declaring private static Member function maps
 **************************************************************/

std::unordered_map<std::type_index, FileDescriptorTXT::WriteMethod>
    FileDescriptorTXT::write_methods_;

std::unordered_map<std::type_index, FileDescriptorTXT::ReadMethod>
    FileDescriptorTXT::read_methods_;

FileDescriptorTXT::FileDescriptorTXT() {
  // Alaways registers as a pointer
  registerWriteMethod<BaseDescriptorWrapper>();
  registerReadMethod<BaseDescriptorWrapper>();
}

/***************************************************************************
 * Private Methods
 ***************************************************************************/

void FileDescriptorTXT::write_(std::vector<std::any> &objs, std::ostream &os) {

  for (auto &obj : objs) {
    if (write_methods_.count(obj.type()) == 0) {
      std::string error_msg =
          "Unable to write object to descriptor txt file, write ";
      error_msg += "method is missing.";
      PANACEA_FAIL(error_msg);
    }
    auto data = write_methods_[obj.type()](type(), os, obj);
    write_(data, os);
  }
}

void FileDescriptorTXT::read_(std::any parent, ReadInstantiateVector &objs,
                              std::istream &is) {

  for (auto &obj : objs) {
    if (read_methods_.count(obj.instance.type()) == 0) {
      std::string error_msg =
          "Unable to read object from descriptor txt file, read ";
      error_msg += "method is missing.";
      PANACEA_FAIL(error_msg);
    }
    auto data = read_methods_[obj.instance.type()](type(), is, obj.instance);
    read_(obj, data, is);
    if (auto method = obj.method) {
      (*method)(parent);
    }
  }
}

/***************************************************************************
 * Public Methods
 ***************************************************************************/

void FileDescriptorTXT::write(std::any obj, const std::string &filename) {

  std::fstream fs;
  fs.open(filename, std::fstream::out);
  write(obj, fs);
  fs.close();
}

void FileDescriptorTXT::write(std::any obj, std::ostream &os) {

  // Check if object type is registered
  if (write_methods_.count(obj.type())) {
    auto data = write_methods_[obj.type()](type(), os, obj);
    write_(data, os);
  } else {
    std::string error_msg = "Unable to write object it does not contain ";
    error_msg += "a registered write method.";
    PANACEA_FAIL(error_msg);
  }
}

void FileDescriptorTXT::read(std::any obj, std::istream &is) {

  // Check if object type is registered
  if (read_methods_.count(obj.type())) {
    auto data = read_methods_[obj.type()](type(), is, obj);
    read_(obj, data, is);
  } else {
    std::string error_msg = "Unable to read object it does not contain ";
    error_msg += "a registered read method.";
    PANACEA_FAIL(error_msg);
  }
}

void FileDescriptorTXT::read(std::any obj, const std::string &filename) {

  std::fstream fs;
  fs.open(filename, std::fstream::in);
  read(obj, fs);
  fs.close();
}

} // namespace io
} // namespace panacea
