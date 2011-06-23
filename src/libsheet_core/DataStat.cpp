#include <math.h>

#include <coopy/DataStat.h>
#include <coopy/CsvWrite.h>

#include <map>

using namespace std;
using namespace coopy::store;
using namespace coopy::cmp;

void DataStat::evaluate(const DataSheet& sheet) {
  int hh = 50;
  if (sheet.height()<hh) hh = sheet.height();
  clear();
  col.clear();
  if (sheet.width()==0) return;
  if (hh==0) return;
  oddness.resize(sheet.width(),hh,0);
  oddness_accum.resize(1,hh,0);
  for (int j=0; j<sheet.width(); j++) {
    col.push_back(DataColumn(sheet,j,hh));
    ct.push_back(ColumnType());
  }
  for (int j=0; j<sheet.width(); j++) {
    col[j].evaluate();
  }
  for (int j=0; j<sheet.width(); j++) {
    DataColumn& c = col[j];
    Nature n = c.getNature();
    vector<float> cmp;
    float tot = 0;
    float tot2 = 0;
    for (int i=0; i<hh; i++) {
      float r = n.compare(sheet.cellString(j,i).c_str());
      cmp.push_back(r);
      tot += r;
      tot2 += r*r;
    }
    float mean = tot;
    float dev = 1;
    if (hh>0) {
      mean /= hh;
      dev = tot2 / hh - mean*mean;
      // occasionally dev is just slightly less than zero due to round-offs
      if (dev<0) { 
	dev = 0; 
      } else {
	dev = sqrt(dev);
      }
    }
    if (dev<0.1) dev = 0.1;
    for (int i=0; i<hh; i++) {
      float v = (cmp[i]-mean)/dev;
      v = -v;
      if (v<0) v = 0;
      if (sheet.cellString(j,i)=="") {
	v = 0;
      }
      oddness.cell(j,i) = v;
      oddness_accum.cell(0,i) += v;
    }
  }

  oddness_accum.normalize(-1,-1,1);

  //CsvFile::write(oddness,"-");
  //CsvFile::write(oddness_accum,"oddness_accum.csv");

  int top = -1;
  int evidence = 0;
  for (int i=0; i<hh; i++) {
    float v = oddness_accum.cell(0,i);
    if (v>=1) {
      if (i<10) {
	top = i;
      } else {
	top = -1;
      }
    }
    if (v<0.5) {
      evidence++;
    }
  }
  /*
  if (top>=0) {
    if (evidence<top*2+2) {
      top = -1;
    }
  }
  */

  if (top>=0) {
    // correct type checking
    for (int j=0; j<sheet.width(); j++) {
      DataColumn& c = col[j];
      c.unevaluate(top);
    }
  }

  for (int j=0; j<sheet.width(); j++) {
    DataColumn& c = col[j];
    Nature n = c.getNature();
    ColumnType mct;
    if (n.couldBeInteger()) {
      mct.setType("INTEGER");
    }
    ct[j] = mct;
  }

  if (top>=0) {
    dbg_printf("Header guess: [%d]", top);
    for (int j=0; j<sheet.width(); j++) {
      dbg_printf(" [%s]", sheet.cellString(j,top).c_str());
    }
    dbg_printf("\n");
  } else {
    dbg_printf("Cannot guess header\n");
  }
  rowDivider = top;
}


void DataStat::evaluate2(const coopy::store::DataSheet& sheet,
			 const coopy::cmp::CompareFlags& flags) {
  int hh = 50;
  if (sheet.height()<hh) hh = sheet.height();
  clear();
  col.clear();
  if (sheet.width()==0) return;
  if (hh==0) return;
  int top = -1;
  for (int j=0; j<sheet.width(); j++) {
    col.push_back(DataColumn(sheet,j,hh));
    ct.push_back(ColumnType());
  }
  for (int j=0; j<sheet.width(); j++) {
    col[j].evaluate();
  }

  if (flags.ids.size()>0) {
    //printf("CHECKING IDS\n");
    map<string,int> ids;
    for (int i=0; i<(int)flags.ids.size(); i++) {
      ids[flags.ids[i]] = 1;
    }
    for (int i=0; i<hh; i++) {
      int found = 0;
      for (int j=0; j<sheet.width(); j++) {
	string txt = sheet.cellString(j,i);
	if (ids.find(txt)!=ids.end()) {
	  //printf("FOUND %s\n", txt.c_str());
	  found++;
	}
      }
      if (found==(int)flags.ids.size()) {
	//printf("GOT IT on %d\n", i);
	top = i;
	break;
      }
    }
  } else {

    oddness.resize(sheet.width(),hh,0);
    oddness_accum.resize(1,hh,0);
    
    for (int j=0; j<sheet.width(); j++) {
      DataColumn& c = col[j];
      Nature n = c.getNature();
      //n.show();
      vector<float> cmp;
      float tot = 0;
      float tot2 = 0;
      for (int i=0; i<hh; i++) {
	float r = n.compare(sheet.cellString(j,i).c_str());
	cmp.push_back(r);
	tot += r;
	tot2 += r*r;
      }
      float mean = tot;
      float dev = 1;
      if (hh>0) {
	mean /= hh;
	dev = tot2 / hh - mean*mean;
	// occasionally dev is just slightly less than zero due to round-offs
	if (dev<0) { 
	  dev = 0; 
	} else {
	  dev = sqrt(dev);
	}
      }
      if (dev<0.1) dev = 0.1;
      for (int i=0; i<hh; i++) {
	float v = (cmp[i]-mean)/dev;
	v = -v;
	if (v<0) v = 0;
	if (sheet.cellString(j,i)=="") {
	  v = 0;
	}
	oddness.cell(j,i) = v;
      }
    }
    
    for (int i=0; i<hh; i++) {
      for (int j=0; j<sheet.width(); j++) {
	float v = oddness.cell(j,i);
	if (oddness_accum.cell(0,i)<v) {
	  oddness_accum.cell(0,i) = v;
	}
      }
    }

    //oddness_accum.normalize(-1,-1,1);

    /*
      Property p;
      p.put("file","-");
      p.put("header",-1);
      CsvFile::write(oddness,p);
      if (oddness_accum.height()>1) {
      p.put("file","oddness_accum.csv");
      CsvFile::write(oddness_accum,p);
      }
    */
    
    int evidence = 0;
    float peak = 0;
    for (int i=0; i<hh; i++) {
      float v = oddness_accum.cell(0,i);
      if (v>=0.99&&v>peak*0.75) {
	if (i<10) {
	  top = i;
	  peak = v;
	} else {
	  top = -1;
	}
      }
      if (v<0.5) {
	evidence++;
      }
    }

    if (top>=0) {
      // correct type checking
      for (int j=0; j<sheet.width(); j++) {
	DataColumn& c = col[j];
	c.unevaluate(top);
      }
    }
  }

  for (int j=0; j<sheet.width(); j++) {
    DataColumn& c = col[j];
    Nature n = c.getNature();
    ColumnType mct;
    if (n.couldBeInteger()) {
      mct.setType("INTEGER");
    }
    ct[j] = mct;
  }

  if (top>=0) {
    dbg_printf("Header guess: [%d]", top);
    for (int j=0; j<sheet.width(); j++) {
      dbg_printf(" [%s]", sheet.cellString(j,top).c_str());
    }
    dbg_printf("\n");
  } else {
    dbg_printf("Cannot guess header\n");
  }
  rowDivider = top;
}


