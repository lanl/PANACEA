#ifndef PANACEA_PRIVATE_ROWECHELON_H
#define PANACEA_PRIVATE_ROWECHELON_H
#pragma once

// Standard includes
#include <vector>

namespace panacea {

  class Matrix;

  class RowEchelon {
    private:
      double threshold_ = 1E-9;

    public:
      explicit RowEchelon(const double threshold) : threshold_(threshold) {};

      /**
       * Places a matrix 'mat' in row echelon form
       *
       * Will return the list of indices in the order that they have been
       * swapped. E.g. the index 0 of the returned vector will point to
       * the current row in mat after it has been put in row echelon form.
       *
       **/
      std::vector<int> operate(Matrix * mat);
  };
}

#endif // PANACEA_PRIVATE_ROWECHELON_H

