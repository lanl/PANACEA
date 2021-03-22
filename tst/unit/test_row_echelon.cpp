
// Local private PANACEA includes
#include "attribute_manipulators/row_echelon.hpp"

// Third party includes
#include <catch2/catch.hpp>

// Local public PANACEA includes
#include "panacea/matrix.hpp"

using namespace std;
using namespace panacea;

TEST_CASE("Testing:row_echelon1","[unit,panacea]"){

  auto mat = createMatrix(4,4);

  // Contents of matrix
  //
  //   2  3  2  5
  //   3  1 -3  0
  //  -8  1  3  7
  //   0  1  0 -9

  // Row 0
  mat->operator()(0,0) =  2.0;
  mat->operator()(0,1) =  3.0;
  mat->operator()(0,2) =  2.0;
  mat->operator()(0,3) =  5.0;

  // Row 1
  mat->operator()(1,0) =  3.0;
  mat->operator()(1,1) =  1.0;
  mat->operator()(1,2) = -3.0;
  mat->operator()(1,3) =  0.0;

  // Row 2
  mat->operator()(2,0) = -8.0;
  mat->operator()(2,1) =  1.0;
  mat->operator()(2,2) =  3.0;
  mat->operator()(2,3) =  7.0;

  // Row 3
  mat->operator()(3,0) =  0.0;
  mat->operator()(3,1) =  1.0;
  mat->operator()(3,2) =  0.0;
  mat->operator()(3,3) = -9.0;

  const double threshold = 1E-4;
  RowEchelon row_echelon(threshold);

  row_echelon.operate(mat.get()); 

  mat->print();
  // Solution should be
  //
  //  2.0   3.0   2.0   5.0
  //  0.0  13.0  11.0  27.0
  //  0.0   0.0  -3.0   0.0
  //  0.0   0.0   0.0 -11.0 

  // Row 0
  REQUIRE(mat->operator()(0,0) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,1) == Approx(3.0).margin(1E-3));
  REQUIRE(mat->operator()(0,2) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,3) == Approx(5.0).margin(1E-3));

  // Row 1
  REQUIRE(mat->operator()(1,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,1) == Approx(-3.5).margin(1E-3));
  REQUIRE(mat->operator()(1,2) == Approx(-6.0).margin(1E-3));
  REQUIRE(mat->operator()(1,3) == Approx(-7.5).margin(1E-3));

  // Row 2
  REQUIRE(mat->operator()(2,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,2) == Approx(-11.286).margin(1E-3));
  REQUIRE(mat->operator()(2,3) == Approx(-0.857).margin(1E-3));

  // Row 3
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(-11.013).margin(1E-3));
}

TEST_CASE("Testing:row_echelon2","[unit,panacea]"){

  auto mat = createMatrix(4,4);

  // Contents of matrix
  //
  //   2  3  2   5
  //   0  2  0 -18
  //  -8  1  3   7
  //   0  1  0  -9

  // Row 0
  mat->operator()(0,0) =   2.0;
  mat->operator()(0,1) =   3.0;
  mat->operator()(0,2) =   2.0;
  mat->operator()(0,3) =   5.0;

  // Row 1
  mat->operator()(1,0) =   0.0;
  mat->operator()(1,1) =   2.0;
  mat->operator()(1,2) =   0.0;
  mat->operator()(1,3) = -18.0;

  // Row 2
  mat->operator()(2,0) =  -8.0;
  mat->operator()(2,1) =   1.0;
  mat->operator()(2,2) =   3.0;
  mat->operator()(2,3) =   7.0;

  // Row 3
  mat->operator()(3,0) =   0.0;
  mat->operator()(3,1) =   1.0;
  mat->operator()(3,2) =   0.0;
  mat->operator()(3,3) =  -9.0;

  const double threshold = 1E-4;
  RowEchelon row_echelon(threshold);

  row_echelon.operate(mat.get()); 

  mat->print();
  // Solution should be
  //
  //  2.0   3.0   2.0   5.0
  //  0.0   2.0   0.0 -18.0
  //  0.0   0.0  11.0 144.0
  //  0.0   0.0   0.0   0.0 

  // Row 0
  REQUIRE(mat->operator()(0,0) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,1) == Approx(3.0).margin(1E-3));
  REQUIRE(mat->operator()(0,2) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,3) == Approx(5.0).margin(1E-3));

  // Row 1
  REQUIRE(mat->operator()(1,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,1) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(1,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,3) == Approx(18.0).margin(1E-3));

  // Row 2
  REQUIRE(mat->operator()(2,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,2) == Approx(-11.0).margin(1E-3));
  REQUIRE(mat->operator()(2,3) == Approx(144.0).margin(1E-3));

  // Row 3
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(0.0).margin(1E-3));
}

TEST_CASE("Testing:row_echelon3","[unit,panacea]"){

  auto mat = createMatrix(4,4);

  // Contents of matrix
  //
  //   0  2  0        8
  //   1  3  2        1
  //   0  2  0        3
  //   9  5 17.9999  -9

  // Row 0
  mat->operator()(0,0) =   0.0;
  mat->operator()(0,1) =   2.0;
  mat->operator()(0,2) =   0.0;
  mat->operator()(0,3) =   8.0;

  // Row 1
  mat->operator()(1,0) =   1.0;
  mat->operator()(1,1) =   3.0;
  mat->operator()(1,2) =   2.0;
  mat->operator()(1,3) =   1.0;

  // Row 2
  mat->operator()(2,0) =   0.0;
  mat->operator()(2,1) =   2.0;
  mat->operator()(2,2) =   0.0;
  mat->operator()(2,3) =   3.0;

  // Row 3
  mat->operator()(3,0) =   9.0;
  mat->operator()(3,1) =   5.0;
  mat->operator()(3,2) =  17.9999;
  mat->operator()(3,3) =   7.0;

  const double threshold = 1E-4;
  RowEchelon row_echelon(threshold);

  row_echelon.operate(mat.get()); 

  mat->print();
  // Solution should be
  //
  //  1.0   3.0   2.0   1.0
  //  0.0   2.0   0.0   8.0
  //  0.0   0.0   0.0  -5.0
  //  0.0   0.0   0.0   0.0 

  // Row 0
  REQUIRE(mat->operator()(0,0) == Approx(1.0).margin(1E-3));
  REQUIRE(mat->operator()(0,1) == Approx(3.0).margin(1E-3));
  REQUIRE(mat->operator()(0,2) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,3) == Approx(1.0).margin(1E-3));

  // Row 1
  REQUIRE(mat->operator()(1,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,1) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(1,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,3) == Approx(8.0).margin(1E-3));

  // Row 2
  REQUIRE(mat->operator()(2,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,3) == Approx(-5.0).margin(1E-3));

  // Row 3
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(0.0).margin(1E-3));
}

TEST_CASE("Testing:row_echelon4","[unit,panacea]"){

  auto mat = createMatrix(5,4);

  // Contents of matrix
  //
  //   0.0   1.0   0.0  -9.0
  //   2.0   3.0   2.0   5.0
  //   0.0   2.0   0.0 -18.0
  //  -8.0   1.0   3.0   7.0
  //  -2.0   1.0   0.0  10.0
  
  // Row 0
  mat->operator()(0,0) =   0.0;
  mat->operator()(0,1) =   1.0;
  mat->operator()(0,2) =   0.0;
  mat->operator()(0,3) =  -9.0;

  // Row 1
  mat->operator()(1,0) =   2.0;
  mat->operator()(1,1) =   3.0;
  mat->operator()(1,2) =   2.0;
  mat->operator()(1,3) =   5.0;

  // Row 2
  mat->operator()(2,0) =   0.0;
  mat->operator()(2,1) =   2.0;
  mat->operator()(2,2) =   0.0;
  mat->operator()(2,3) = -18.0;

  // Row 3
  mat->operator()(3,0) =  -8.0;
  mat->operator()(3,1) =   1.0;
  mat->operator()(3,2) =   3.0;
  mat->operator()(3,3) =   7.0;

  // Row 4
  mat->operator()(4,0) =  -2.0;
  mat->operator()(4,1) =   1.0;
  mat->operator()(4,2) =   0.0;
  mat->operator()(4,3) =  10.0;

  const double threshold = 1E-4;
  RowEchelon row_echelon(threshold);

  row_echelon.operate(mat.get()); 

  mat->print();
  // Solution should be
  //
  //  2.0   3.0   2.0   5.0
  //  0.0   1.0   0.0  -9.0
  //  0.0   0.0  11.0 144.0
  //  0.0   0.0   0.0  24.814
  //  0.0   0.0   0.0   0.0

  // Row 0
  REQUIRE(mat->operator()(0,0) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,1) == Approx(3.0).margin(1E-3));
  REQUIRE(mat->operator()(0,2) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,3) == Approx(5.0).margin(1E-3));

  // Row 1
  REQUIRE(mat->operator()(1,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,1) == Approx(1.0).margin(1E-3));
  REQUIRE(mat->operator()(1,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,3) == Approx(-9.0).margin(1E-3));

  // Row 2
  REQUIRE(mat->operator()(2,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,2) == Approx(11.0).margin(1E-3));
  REQUIRE(mat->operator()(2,3) == Approx(144.0).margin(1E-3));

  // Row 3
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(24.814).margin(1E-3));

  // Row 4
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(0.0).margin(1E-3));
}

TEST_CASE("Testing:row_echelon5","[unit,panacea]"){

  auto mat = createMatrix(4,5);

  // Contents of matrix
  //
  //   2.0   3.0   2.0   5.0   0.0
  //   0.0   2.0   0.0 -18.0   7.0
  //  -8.0   1.0   3.0   7.0   2.0
  //   0.0   1.0   0.0  -9.0   4.0
  
  // Row 0
  mat->operator()(0,0) =   2.0;
  mat->operator()(0,1) =   3.0;
  mat->operator()(0,2) =   2.0;
  mat->operator()(0,3) =   5.0;
  mat->operator()(0,4) =   0.0;

  // Row 1
  mat->operator()(1,0) =   0.0;
  mat->operator()(1,1) =   2.0;
  mat->operator()(1,2) =   0.0;
  mat->operator()(1,3) = -18.0;
  mat->operator()(0,4) =   7.0;

  // Row 2
  mat->operator()(2,0) =  -8.0;
  mat->operator()(2,1) =   1.0;
  mat->operator()(2,2) =   3.0;
  mat->operator()(2,3) =   7.0;
  mat->operator()(0,4) =   2.0;

  // Row 3
  mat->operator()(3,0) =   0.0;
  mat->operator()(3,1) =   1.0;
  mat->operator()(3,2) =   0.0;
  mat->operator()(3,3) =  -9.0;
  mat->operator()(0,4) =   4.0;

  const double threshold = 1E-4;
  RowEchelon row_echelon(threshold);

  row_echelon.operate(mat.get()); 

  mat->print();
  // Solution should be
  //
  //  2.0   3.0   2.0   5.0
  //  0.0   1.0   0.0  -9.0
  //  0.0   0.0  11.0 144.0
  //  0.0   0.0   0.0  24.814
  //  0.0   0.0   0.0   0.0

  // Row 0
  REQUIRE(mat->operator()(0,0) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,1) == Approx(3.0).margin(1E-3));
  REQUIRE(mat->operator()(0,2) == Approx(2.0).margin(1E-3));
  REQUIRE(mat->operator()(0,3) == Approx(5.0).margin(1E-3));

  // Row 1
  REQUIRE(mat->operator()(1,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,1) == Approx(1.0).margin(1E-3));
  REQUIRE(mat->operator()(1,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(1,3) == Approx(-9.0).margin(1E-3));

  // Row 2
  REQUIRE(mat->operator()(2,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(2,2) == Approx(11.0).margin(1E-3));
  REQUIRE(mat->operator()(2,3) == Approx(144.0).margin(1E-3));

  // Row 3
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(24.814).margin(1E-3));

  // Row 4
  REQUIRE(mat->operator()(3,0) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,1) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,2) == Approx(0.0).margin(1E-3));
  REQUIRE(mat->operator()(3,3) == Approx(0.0).margin(1E-3));
}
