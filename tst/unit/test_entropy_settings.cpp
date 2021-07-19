
// Local private PANACEA includes
#include "entropy/entropy_settings/entropy_settings.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Standard includes
#include <fstream>
#include <iostream>

using namespace std;
using namespace panacea;

TEST_CASE("Testing:entropy settings != and ==", "[unit,panacea]") {
  EntropySettings settings1;
  EntropySettings settings2;

  REQUIRE(settings1 == settings2);

  settings1.weight = 5.0;
  REQUIRE(settings1 != settings2);

  settings1.weight = 1.0;
  REQUIRE(settings1 == settings2);
}

TEST_CASE("Testing:entropy settings write read restart", "[unit,panacea]") {

  EntropySettings settings1;
  settings1.weight = 10.0;
  settings1.numerical_grad_switch = true;
  settings1.numerical_grad_inc = 0.001;

  std::fstream fs;
  fs.open("test_entropy_settings.restart", std::fstream::out);
  settings1.write(settings::FileType::TXTRestart, fs, &settings1);
  fs.close();

  EntropySettings settings2;
  fs.open("test_entropy_settings.restart", std::fstream::in);
  settings2.read(settings::FileType::TXTRestart, fs, &settings2);
  fs.close();

  REQUIRE(settings1 == settings2);
}
