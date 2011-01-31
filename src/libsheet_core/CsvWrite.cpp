#include <stdio.h>
#include <stdlib.h>

#include <coopy/CsvWrite.h>
#include <coopy/CsvSheet.h>
#include <coopy/NameSniffer.h>

using namespace coopy::store;

int CsvFile::write(const DataSheet& src, const char *fname) {
  Property p;
  p.put("file",fname);
  return write(src,p);
}

int CsvFile::write(const DataSheet& src, const Property& config) {
  dbg_printf("Writing a %dx%d csv file\n", src.width(), src.height());
  std::string fname = config.get("file",PolyValue::makeString("-")).asString();
  SheetStyle style;
  FILE *fp = NULL;
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
      fprintf(stderr,"could not open %s\n", fname.c_str());
      exit(1);
    }
    style.setFromFilename(fname.c_str());
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
  if (wantHeader) {
    NameSniffer sniffer(src);
    if (!sniffer.isEmbedded()) {
      if (wantFakeHeader||!sniffer.isFake()) {
	CsvSheet header;
	header.resize(src.width(),1,"");
	for (int i=0; i<src.width(); i++) {
	  header.cell(i,0) = sniffer.suggestColumnName(i);
	}
	std::string result = header.encode(style);
	fwrite(result.c_str(),1,result.length(),fp);
      }
    }
  }

  std::string result = src.encode(style);
  fwrite(result.c_str(),1,result.length(),fp);
  if (fp!=stdout) {
    fclose(fp);
    fp = NULL;
  }
  return 0;
}


