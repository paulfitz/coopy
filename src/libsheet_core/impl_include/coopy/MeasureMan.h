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

  std::string cell(coopy::store::TextSheet& a, int x, int y) {
    if (rowLike) {
      return a.cell(x,y);
    }
    return a.cell(y,x);
  }

  void setup();

  void compare();

  void compare1(int ctrl);
};



#endif

