#ifndef COOPY_MEASUREMAN
#define COOPY_MEASUREMAN

#include <coopy/Measure.h>

namespace coopy {
  namespace cmp {
    class MeasureMan;
  }
}

class coopy::cmp::MeasureMan {
public:
  Measure& main;
  Measure& anorm;
  Measure& bnorm;
  MeasurePass& main_pass;
  MeasurePass& anorm_pass;
  MeasurePass& bnorm_pass;
  bool rowLike;

  MeasureMan(Measure& main,
	     MeasurePass& main_pass,
	     Measure& anorm,
	     MeasurePass& anorm_pass,
	     Measure& bnorm,
	     MeasurePass& bnorm_pass,
	     bool rowLike) : main(main), 
			     anorm(anorm), 
			     bnorm(bnorm),
			     main_pass(main_pass),
			     anorm_pass(anorm_pass),
			     bnorm_pass(bnorm_pass),
			     rowLike(rowLike) // just for debugging
  {
  }
	     

  int cellLength(coopy::store::DataSheet& a) {
    if (rowLike) {
      return a.width();
    }
    return a.height();
  }

  std::string cell(coopy::store::DataSheet& a, int x, int y) {
    if (!cellExists(a,x,y)) {
      return "(no-cell)";
    }
    if (rowLike) {
      return a.cellString(x,y);
    }
    return a.cellString(y,x);
  }

  bool cellExists(coopy::store::DataSheet& a, int x, int y) {
    if (x<0||y<0) return false;
    if (rowLike) {
      return x<a.width()&&y<a.height();
    }
    return y<a.width()&&x<a.height();
  }

  void setup();

  void compare();

  void compare1(int ctrl);
};



#endif

