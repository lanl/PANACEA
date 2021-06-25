
// Local private PANACEA includes
#include "attribute_manipulators/normalizer.hpp"

#include "attributes/covariance.hpp"
#include "matrix/matrix.hpp"
#include "vector/vector.hpp"
#include "io/file_io_factory.hpp"

// Public PANACEA includes
#include "panacea/file_io.hpp"

// Third party includes
#include <catch2/catch.hpp>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:normalizer1","[unit,panacea]"){

  auto matrix = createMatrix(3,3);
  auto vector = createVector(3);
  const int total_num_pts = 10;

  // Make diagonal 2, 3, 4, and off diagonal elements 1
  // Make vector unity
  //
  // 2 1 1
  // 1 3 1
  // 1 1 4
  //
  for( int i = 0; i < 3; ++i){
    vector->operator()(i) = 1;
    matrix->operator()(i, i) = i+2;
    for( int j = i+1; j< 3; ++j ){
      matrix->operator()(i, j) = 1;
      matrix->operator()(j, i) = 1;
    }
  }

  auto cov_ptr = Covariance::create(
      settings::KernelCorrelation::Correlated,
      std::move(matrix),
      std::move(vector),
      total_num_pts);

  auto & covar = *cov_ptr;

  std::vector<double> coeffs = { 1.0, 3.0, 2.0 };
  Normalizer normalizer(coeffs);

  REQUIRE(covar.getNormalizationState() == NormalizationState::Unnormalized);
  normalizer.normalize(covar);
  REQUIRE(covar.getNormalizationState() == NormalizationState::Normalized);

  // Coefficients should be divided by 1.0, 6.0 and 2.0
  REQUIRE(covar(0,0) == Approx(2.0));
  REQUIRE(covar(1,1) == Approx(1.0/3.0));
  REQUIRE(covar(2,2) == Approx(1.0));

  REQUIRE(covar(0,1) == Approx(1.0/3.0));
  REQUIRE(covar(1,0) == Approx(1.0/3.0));
  REQUIRE(covar(2,0) == Approx(1.0/2.0));
  REQUIRE(covar(0,2) == Approx(1.0/2.0));
  REQUIRE(covar(1,2) == Approx(1.0/(2.0 * 3.0)));
  REQUIRE(covar(2,1) == Approx(1.0/(2.0 * 3.0)));

  normalizer.unnormalize(covar);
  REQUIRE(covar.getNormalizationState() == NormalizationState::Unnormalized);

  REQUIRE(covar(0,0) == Approx(2.0));
  REQUIRE(covar(1,1) == Approx(3.0));
  REQUIRE(covar(2,2) == Approx(4.0));

  REQUIRE(covar(0,1) == Approx(1.0));
  REQUIRE(covar(1,0) == Approx(1.0));
  REQUIRE(covar(2,0) == Approx(1.0));
  REQUIRE(covar(0,2) == Approx(1.0));
  REQUIRE(covar(1,2) == Approx(1.0));
  REQUIRE(covar(2,1) == Approx(1.0));

}

TEST_CASE("Testing:normalizer write & read","[unit,panacea]"){
  std::vector<double> coeffs = { 1.0, 3.0, 2.0 };
  Normalizer normalizer(coeffs);

  std::fstream fs;
  fs.open("test_normalizer.restart", std::fstream::out);
  auto data_out = Normalizer::write(settings::FileType::TXTRestart, fs, &normalizer);
  fs.close();

  Normalizer normalizer2;
  // The internal matrix and vectors will not be read in from this call
  std::fstream fs2;
  fs2.open("test_normalizer.restart", std::fstream::in);
  auto data_out2 = Normalizer::read(settings::FileType::TXTRestart, fs2, &normalizer2);
  fs2.close();

  const auto & norm_coeffs = normalizer.getNormalizationCoeffs();
  const auto & norm_coeffs2 = normalizer2.getNormalizationCoeffs();

  REQUIRE(norm_coeffs.size() == norm_coeffs2.size());
  REQUIRE(norm_coeffs.at(0) == norm_coeffs2.at(0));
  REQUIRE(norm_coeffs.at(1) == norm_coeffs2.at(1));
  REQUIRE(norm_coeffs.at(2) == norm_coeffs2.at(2));
}

TEST_CASE("Testing:normalizer write & read using fileio","[integration,panacea]"){
  std::vector<double> coeffs = { 1.0, 3.0, 2.0 };
  Normalizer normalizer(coeffs);

  io::FileIOFactory file_io_factory;
  auto restart_file = file_io_factory.create(settings::FileType::TXTRestart);

  restart_file->write(&normalizer,"test_normalizer_file_io.restart");

  Normalizer normalizer2;

  restart_file->read(&normalizer2,"test_normalizer_file_io.restart");

  const auto & norm_coeffs = normalizer.getNormalizationCoeffs();
  const auto & norm_coeffs2 = normalizer2.getNormalizationCoeffs();

  REQUIRE(norm_coeffs.size() == norm_coeffs2.size());
  REQUIRE(norm_coeffs.at(0) == norm_coeffs2.at(0));
  REQUIRE(norm_coeffs.at(1) == norm_coeffs2.at(1));
  REQUIRE(norm_coeffs.at(2) == norm_coeffs2.at(2));
}

