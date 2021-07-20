
// Local private PANACEA includes
#include "attribute_manipulators/normalizer.hpp"

#include "attributes/covariance.hpp"
#include "io/file_io_factory.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:normalizer write & read using fileio",
          "[integration,panacea]") {
  std::vector<double> coeffs = {1.0, 3.0, 2.0};
  Normalizer normalizer(coeffs);

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

  restart_file->write(&normalizer, "test_normalizer_file_io.restart");

  Normalizer normalizer2;

  restart_file->read(&normalizer2, "test_normalizer_file_io.restart");

  const auto &norm_coeffs = normalizer.getNormalizationCoeffs();
  const auto &norm_coeffs2 = normalizer2.getNormalizationCoeffs();

  REQUIRE(norm_coeffs.size() == norm_coeffs2.size());
  REQUIRE(norm_coeffs.at(0) == norm_coeffs2.at(0));
  REQUIRE(norm_coeffs.at(1) == norm_coeffs2.at(1));
  REQUIRE(norm_coeffs.at(2) == norm_coeffs2.at(2));
}
