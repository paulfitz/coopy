#include <coopy/CsvSheet.h>
#include <coopy/CsvWrite.h>
#include <coopy/Dbg.h>

#include <stdio.h>
#include <stdlib.h>

using namespace coopy::store;

void CsvSheet::addField(const char *s) {
  if (th>=1&&tw>=w) {
    fprintf(stderr,"Attempt to add a spurious field to a sheet (\"%s\")\n",
	    s);
    if (tw==w) {
      addRecord();
    }
    fprintf(stderr,"w*h = %d*%d, tw*th = %d*%d\n",
	    w,h,tw,th);
    const char *name = "__spurious_field.prior.csv";
    fprintf(stderr,"Saving sheet as is to %s\n",name);
    CsvFile::write(*this,name);
    coopy_print_trace(stdout,__FILE__,__LINE__);
    exit(1);
  }
  rec.push_back(s);
  tw++;
}
