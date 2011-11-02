#include <stdio.h>
#include <stdlib.h>

#include <coopy/CsvWrite.h>
#include <coopy/CsvSheet.h>
#include <coopy/NameSniffer.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace std;

int CsvFile::write(const DataSheet& src, const char *fname) {
  Property p;
  p.put("file",fname);
  return write(src,p);
}

static int write(const DataSheet& src, const Property& config,
		 string *output) {
  dbg_printf("Writing a %dx%d csv file\n", src.width(), src.height());
  std::string fname = config.get("file",PolyValue::makeString("-")).asString();
  SheetStyle style;
  FILE *fp = NULL;
  if (!output) {
    if (fname=="-") {
      fp = stdout;
      //SheetStyle style;
      //std::string result = src.encode(style);
      //printf("%s",result.c_str());
    } else {
      bool append = config.get("append",
			       PolyValue::makeBoolean(false)).asBoolean();
      fp = fopen(fname.c_str(),append?"ab":"wb");
      if (!fp) {
	fprintf(stderr,"CsvFile: could not open %s\n", fname.c_str());
	exit(1);
      }
      style.setFromFilename(fname.c_str());
    }
  }
  style.setFromProperty(config);
  bool wantHeader = true;
  bool wantFakeHeader = false;
  if (config.check("header")) {
    int n = config.get("header").asInt();
    if (n<0) {
      wantHeader = false;
    } else {
      wantFakeHeader = true;
    }
  }
  bool markHeader = config.get("mark_header",
			       PolyValue::makeBoolean(false)).asBoolean();
  if (wantHeader) {
    CompareFlags flags;
    NameSniffer sniffer(src,flags);
    if (!sniffer.isEmbedded()) {
      if (wantFakeHeader||!sniffer.isFake()) {
	CsvSheet header;
	header.resize(src.width(),1,"");
	for (int i=0; i<src.width(); i++) {
	  header.cell(i,0) = sniffer.suggestColumnName(i);
	}
	std::string result = header.encode(style);
	if (markHeader) {
	  int len = result.length();
	  len--;
	  while (len>0 && result[len-1]=='\r') {
	    len--;
	  }
	  if (len<3) len = 3;
	  if (len>79) len = 79;
	  for (int i=0; i<len; i++) {
	    result += '-';
	  }
	  result += "\r\n";
	  style.setMarkHeader(false);
	}
	if (fp) {
	  fwrite(result.c_str(),1,result.length(),fp);
	} else {
	  *output += result;
	}
      }
    }
  }

  std::string result = src.encode(style);
  if (fp) {
    fwrite(result.c_str(),1,result.length(),fp);
    if (fp!=stdout) {
      fclose(fp);
      fp = NULL;
    }
  } else {
    *output += result;
  }
  return 0;
}


int CsvFile::write(const DataSheet& src, const Property& config) {
  return ::write(src,config,NULL);
}

string CsvFile::writeString(const DataSheet& src, const Property& config) {
  string result;
  int r = ::write(src,config,&result);
  return result;
}
