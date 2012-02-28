#include <coopy/ColMan.h>
#include <coopy/FMap.h>

#include <math.h>

using namespace coopy::cmp;

void ColMan::measure(MeasurePass& pass, int ctrl) {
  int wa = pass.a.width();
  int wb = pass.b.width();
  int ha = pass.a.height();
  int hb = pass.b.height();

  dbg_printf("Column comparison\n");
  pass.clearMatch();

  if (ha<10 || hb<10) {
    pass.va.meta.sniff();
    pass.vb.meta.sniff();
    if (true) { //!pass.va.meta.isEmbedded() || !pass.va.meta.isEmbedded()) {
      const std::vector<std::string>& anames = pass.va.meta.suggestNames();
      const std::vector<std::string>& bnames = pass.vb.meta.suggestNames();
      dbg_printf("anames.size() %d wa %d ha %d   bnames.size() %d wb %d hb %d\n",
		 anames.size(), wa, ha, bnames.size(), wb, hb);
      if (anames.size()==wa && bnames.size()==wb) {
	FMultiMap m(pass.match);
	int c = m.getCtrlMax();
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(anames[i],false,c);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(bnames[j],true,c);
	}
	dbg_printf("added column names to column comparison\n");
      }
    }
  }

  int step = (int)(hb/pow(2,ctrl+4));
  if (step<1) step = 1;
  dbg_printf("Desperation %d, step size %d\n", ctrl, step);
  int ct = 0;
  for (int rb=0; rb<hb; rb+=step) {
    int ra = comp.b2a(rb);
    if (ra!=-1) {
      ct++;
      FMultiMap m(pass.match);
      int c = m.getCtrlMax();
      //m.resize(wa,wb);
      for (int i=0; i<wa; i++) {
	if (1) { //pass.asel.cell(0,i)==-1) {
	  m.setCurr(i,i);
	  m.add(pass.a.cellString(i,ra),false,c);
	}
      }
      for (int j=0; j<wb; j++) {
	if (1) { //pass.bsel.cell(0,j)==-1) {
	  m.setCurr(j,j);
	  m.add(pass.b.cellString(j,rb),true,c);
	}
      }
    }
  }
  if (step==1 && (ha<10 || hb<10) && (ct<ha/2||ct<hb/2)) {
    pass.va.meta.sniff();
    pass.vb.meta.sniff();
    const std::vector<std::string>& anames = pass.va.meta.suggestNames();
    const std::vector<std::string>& bnames = pass.vb.meta.suggestNames();
    FMultiMap m(pass.match);
    int c = m.getCtrlMax();

    if (wa==(int)anames.size()) {
      for (int i=0; i<wa; i++) {
	m.setCurr(i,i);
	m.add(anames[i],false,c);
      }
      for (int rb=0; rb<hb; rb++) {
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cellString(j,rb),true,c);
	}
      }
    }

    if (wb==(int)bnames.size()) {
      for (int ra=0; ra<ha; ra++) {
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(pass.a.cellString(i,ra),false,c);
	}
      }
      for (int j=0; j<wb; j++) {
	m.setCurr(j,j);
	m.add(bnames[j],true,c);
      }
    }
  }
  //printf(">>> I HAVE %s\n", pass.match.toString().c_str());
}
