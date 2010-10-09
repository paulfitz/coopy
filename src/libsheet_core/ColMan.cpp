#include <coopy/ColMan.h>
#include <coopy/FMap.h>

#include <math.h>

using namespace coopy::cmp;

void ColMan::measure(MeasurePass& pass, int ctrl) {
    int wa = pass.a.width();
    int wb = pass.b.width();
    //int ha = pass.a.height();
    int hb = pass.b.height();

    dbg_printf("Column comparison\n");
    pass.clearMatch();
    int step = (int)(hb/pow(2,ctrl+4));
    if (step<1) step = 1;
    dbg_printf("Desperation %d, step size %d\n", ctrl, step);
    for (int rb=0; rb<hb; rb+=step) {
      int ra = comp.b2a(rb);
      if (ra!=-1) {
	FMultiMap m(pass.match);
	int c = m.getCtrlMax();
	//m.resize(wa,wb);
	for (int i=0; i<wa; i++) {
	  if (1) { //pass.asel.cell(0,i)==-1) {
	    m.setCurr(i,i);
	    m.add(pass.a.cell(i,ra),false,c);
	  }
	}
	for (int j=0; j<wb; j++) {
	  if (1) { //pass.bsel.cell(0,j)==-1) {
	    m.setCurr(j,j);
	    m.add(pass.b.cell(j,rb),true,c);
	  }
	}
      }
    }
  }
