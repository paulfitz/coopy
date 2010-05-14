#ifndef COOPY_COLMAN
#define COOPY_COLMAN

#include <coopy/Measure.h>

class ColMan : public Measure {
public:
  const OrderResult& comp;

  ColMan(const OrderResult& comp) : comp(comp) {
  }

  virtual void setup(MeasurePass& pass) {
    pass.setSize(pass.a.width(),pass.b.width());
  }


  virtual void measure(MeasurePass& pass, int ctrl) {
    int wa = pass.a.width();
    int wb = pass.b.width();
    int ha = pass.a.height();
    int hb = pass.b.height();

    dbg_printf("Column comparison\n");
    pass.clearMatch();
    int step = (int)(hb/pow(2,ctrl+4));
    if (step<1) step = 1;
    dbg_printf("Desperation %d, step size %d\n", ctrl, step);
    for (int rb=0; rb<hb; rb+=step) {
      int ra = comp.b2a(rb);
      if (ra!=-1) {
	FMap m(pass.match);
	int c = m.getCtrlMax();
	//m.resize(wa,wb);
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(pass.a.cell(i,ra),false,c);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cell(j,rb),true,c);
	}
      }
    }
  }

  virtual int getCtrlMax() {
    return 5;
  }

};



#endif
