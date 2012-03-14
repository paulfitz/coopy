#include <coopy/ColMan.h>
#include <coopy/FMap.h>

#include <vector>

#include <math.h>

using namespace coopy::cmp;
using namespace std;

#define FColMap FMultiMap

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
	FColMap m(pass.match,hb);
	int c = m.getCtrlMax();
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(anames[i],false,false,c);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(bnames[j],false,true,c);
	}
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(bnames[j],true,false,c);
	}
	dbg_printf("added column names to column comparison\n");
      }
    }
  }

  vector<int> aa;
  vector<int> bb;
  for (int rb=0; rb<hb; rb++) {
    int ra = comp.b2a(rb);
    if (ra!=-1) {
      aa.push_back(ra);
      bb.push_back(rb);
    }
  }
  int hh = (int)aa.size();

  int step = (int)(hh/pow(2,ctrl+4));
  if (step<1) step = 1;
  dbg_printf("Desperation %d, step size %d\n", ctrl, step);
  int ct = hh;
  FColMap m(pass.match,hb);
  int c = m.getCtrlMax();
  for (int rr=0; rr<hh; rr+=step) {
    int rb = bb[rr];
    int ra = aa[rr];
    for (int i=0; i<wa; i++) {
      m.setCurr(i,i);
      m.add(pass.a.cellString(i,ra),false,false,c);
    }
  }
  for (int rr=0; rr<hh; rr+=step) {
    int rb = bb[rr];
    int ra = aa[rr];
    for (int j=0; j<wb; j++) {
      m.setCurr(j,j);
      m.add(pass.b.cellString(j,rb),false,true,c);
    }
  }
  for (int rr=0; rr<hh; rr+=step) {
    int rb = bb[rr];
    int ra = aa[rr];
    for (int j=0; j<wb; j++) {
      m.setCurr(j,j);
      m.add(pass.b.cellString(j,rb),true,false,c);
    }
  }
  if (step==1 && (ha<10 || hb<10) && (ct<ha/2||ct<hb/2)) {
    pass.va.meta.sniff();
    pass.vb.meta.sniff();
    const std::vector<std::string>& anames = pass.va.meta.suggestNames();
    const std::vector<std::string>& bnames = pass.vb.meta.suggestNames();
    FColMap m(pass.match,hb);
    int c = m.getCtrlMax();

    if (wa==(int)anames.size()) {
      for (int i=0; i<wa; i++) {
	m.setCurr(i,i);
	m.add(anames[i],false,false,c);
      }
      for (int rb=0; rb<hb; rb++) {
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cellString(j,rb),false,true,c);
	}
      }
      for (int rb=0; rb<hb; rb++) {
	for (int j=0; j<wb; j++) {
	  m.setCurr(j,j);
	  m.add(pass.b.cellString(j,rb),true,false,c);
	}
      }
    }

    if (wb==(int)bnames.size()) {
      for (int ra=0; ra<ha; ra++) {
	for (int i=0; i<wa; i++) {
	  m.setCurr(i,i);
	  m.add(pass.a.cellString(i,ra),false,false,c);
	}
      }
      for (int j=0; j<wb; j++) {
	m.setCurr(j,j);
	m.add(bnames[j],false,true,c);
      }
      for (int j=0; j<wb; j++) {
	m.setCurr(j,j);
	m.add(bnames[j],true,false,c);
      }
    }
  }
  /*
  static int ctt = 0;
  char buf[256];
  sprintf(buf,"/tmp/col_%04d.csv",ctt);
  ctt++;
  FILE *fout = fopen(buf,"w");
  fprintf(fout,"%s\n", pass.match.toString().c_str());
  fclose(fout);
  */
}
