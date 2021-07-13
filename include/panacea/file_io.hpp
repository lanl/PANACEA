#ifndef PANACEA_FILEIO_H
#define PANACEA_FILEIO_H
#pragma once

// Local public PANACEA settings
#include "settings.hpp"

// Standard includes
#include <any>
#include <fstream>
#include <string>
#include <vector>

namespace panacea {

  namespace io {
    class FileIO {
      public:
        /**
         * Documentation of how to use
         **/
        virtual settings::FileType type() const noexcept = 0;
        virtual void read(std::any obj, std::istream & is) = 0;
        virtual void read(std::any obj, const std::string & filename) = 0;
        /**
         * std::unique_ptr<EntropyTerm> self_ent;
         * '' cross_ent;
         *
         * std::unique_ptr<io::FileIO> restart_file_io = panacea.create(settings::FileType::RestartTXT);
         *
         * std::iostream out_stream;
         * out_stream.open("file.restart");
         *
         * restart_file_io.write(self_ent.get(), out_stream);
         * restart_file_io.write(cross_ent.get(), out_stream);
         *
         * out_stream.close();
         *
         * restart_file_io.read(self_ent.get(), in_stream);
         * restart_file_io.read(cross_ent.get(), in_stream);
         *
         **/
        virtual void write(std::any  obj, std::ostream & os) = 0;
        virtual void write(std::any  obj, const std::string & filename) = 0;

        virtual ~FileIO() = 0;
    };
  }

}
#endif // PANACEA_FILEIO_H
