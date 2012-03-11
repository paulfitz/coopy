#include <coopy/CsvSheet.h>
#include <coopy/CsvWrite.h>
#include <coopy/Dbg.h>

#include <stdio.h>
#include <stdlib.h>

using namespace coopy::store;

void CsvSheet::addField(const char *s, bool escaped) {
  /*
  if (isStrict()) {
    if (th>=1&&tw>=this->s.w) {
      fprintf(stderr,"Attempt to add a spurious field to a sheet (\"%s\")\n",
	      s);
      if (tw==this->s.w) {
	addRecord();
      }
      fprintf(stderr,"w*h = %d*%d, tw*th = %d*%d\n",
	      this->s.w,this->s.h,tw,th);
      const char *name = "__spurious_field.prior.csv";
      fprintf(stderr,"Saving sheet as is to %s\n",name);
      CsvFile::write(*this,name);
      coopy_print_trace(stdout,__FILE__,__LINE__);
      exit(1);
    }
  }
  */
  pairCellType p(s,escaped);
  rec.push_back(p);
  tw++;
}

void CsvSheet::addRecord() {
  s.arr.push_back(rec);
  rec.clear();
  if (s.w!=tw && s.w!=0) {
    valid = false;
  }
  if (tw>s.w) {
    s.w = tw;
  }
  tw = 0;
  th++;
  s.h = th;
}
