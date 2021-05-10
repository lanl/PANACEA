
// Local private PANACEA includes
#include "file_io_factory.hpp"

#include "error.hpp"
#include "file_txt_descriptor.hpp"
#include "file_txt_kerneldistribution.hpp"
#include "file_txt_restart.hpp"

namespace panacea {

  namespace io {
    std::unique_ptr<FileIO> FileIOFactory::create(const settings::FileType & type) const {
      if(type == settings::FileType::TXTRestart) {
        return std::make_unique<FileRestartTXT>();
      } else if ( type == settings::FileType::TXTDescriptors ) {
        return std::make_unique<FileDescriptorTXT>();
      } else if ( type == settings::FileType::TXTKernelDistribution ) {
        return std::make_unique<FileKernelDistributionTXT>();
      } else {
        PANACEA_FAIL("File type not registered with factory.");
      }
      return nullptr;
    }
  }
}
