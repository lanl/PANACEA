
// Public PANACEA includes
#include "panacea/base_descriptor_wrapper.hpp"

// Local private PANACEA includes
#include "data_point_template.hpp"

namespace panacea {

  std::vector<std::any> BaseDescriptorWrapper::write(
      settings::FileType file_type,
      std::ostream & os,
      std::any dwrapper_instance) {
    
    std::vector<std::any> nested_values;
    if( file_type == settings::FileType::TXTDescriptors ) {
      auto dwrapper = std::any_cast<BaseDescriptorWrapper *>(dwrapper_instance);
      os << "Descriptors\n";
      os << dwrapper->arrangement() << "\n";
      os << dwrapper->rows() << " " << dwrapper->cols() << "\n";
      for( int row = 0; row < dwrapper->rows(); ++row ) {
        for( int col = 0; col < dwrapper->cols(); ++row ) {
          os << std::setfill(' ') 
            << std::setw(14) 
            << std::setprecision(8) 
            << std::right
            << dwrapper->operator()(row,col) << " ";
        }
        os << "\n";
      }
      os << "\n";
    } else {
      std::string error_msg = "Descriptors cannot be written to the specified file type.";
      PANACEA_FAIL(error_msg);
    }
    return nested_values;
  }

  BaseDescriptorWrapper::~BaseDescriptorWrapper() {};
}
