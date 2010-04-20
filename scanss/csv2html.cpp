#include <stdio.h>
#include "CsvSheet.h"
#include "CsvFile.h"

#include <string>
using namespace std;

string csv2html_css();

static void replace(string& str, const string& old, const string& rep) {
  int pos = 0;
  while((pos = str.find(old, pos)) != std::string::npos) {
    str.replace(pos, old.length(), rep);
    pos += rep.length();
  }
}

// assume text is in, and should remain, utf8
string ml_encode(string x) {
  replace(x,"&amp;","&");
  replace(x,"&lt;","<");
  replace(x,"&rt;",">");
  replace(x,"&","&amp;");
  replace(x,"<","&lt;");
  replace(x,">","&rt;");
  return x;
}

int main(int argc, char *argv[]) {
  bool full = false;
  bool decorate = false;
  const char *fname = "-";
  while (argc>1) {
    string arg = argv[1];
    if (arg=="--full" || arg=="-f") {
      full = true;
      decorate = true;
    } else if (arg=="--decorate" || arg=="-d") {
      decorate = true;
    } else {
      fname = argv[1];
    }
    argv++;
    argc--;
  }
  CsvSheet sheet;
  if (CsvFile::read(fname,sheet)!=0) {
    printf("Failed to read input\n");
    return 1;
  }

  if (full) {
    printf("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n");
    printf("<html>\n");
    printf("<head>\n");
    printf("<title>csv2html</title>\n");
    printf("<style>\n");
    printf("%s",csv2html_css().c_str());
    printf("</style>\n");
    printf("</head>\n");
    printf("<body>\n");
    printf("<div class=\"csv2html\">\n");
  }
  if (sheet.width()>=1 && sheet.height()>=1) {
    printf("<table>\n");
    for (int i=0; i<sheet.height(); i++) {
      string row_mode = "";
      string txt = sheet.cell(0,i);
      if (txt == "[---]") {
	row_mode = "csv_row_mmm";
      } else if (txt == "[+++]") {
	row_mode = "csv_row_ppp";
      } else if (txt == "[-]") {
	row_mode = "csv_row_m";
      } else if (txt == "[+]") {
	row_mode = "csv_row_p";
      } else if (txt == "[for]") {
	row_mode = "csv_row_for";
      } else if (txt == "[do]") {
	row_mode = "csv_row_do";
      } else if (txt == "[conflict]") {
	row_mode = "csv_row_err";
      } else if (txt == "[local]") {
	row_mode = "csv_row_local";
      } else if (txt == "[conflicting]") {
	row_mode = "csv_row_conflicting";
      }
      string row_decorate = "";
      if (decorate&&row_mode!="") {
	row_decorate = string(" class=\"") + row_mode + "\"";
      }
      printf("  <tr%s>", row_decorate.c_str());
      for (int j=0; j<sheet.width(); j++) {
	string txt = sheet.cell(j,i);
	  /*
	vector<string> modes;
	string mode_txt;
	if (decorate) {
	  if (row_mode!="" && j>0) {
	    modes.push_back(row_mode);
	  }
	}
	if (modes.size()>0) {
	  mode_txt = " class=\"";
	  for (int k=0; k<modes.size(); k++) {
	    if (k!=0) {
	      mode_txt += " ";
	    }
	    mode_txt += modes[k];
	  }
	  mode_txt += "\"";
	}
	  */
	txt = ml_encode(txt);
	string cell_decorate = "";
	if (decorate) {
	  if (txt!="") {
	    txt = string("&nbsp;") + txt + "&nbsp;";
	  }
	  if (j==0) {
	    cell_decorate = " class=\"csv_cmd\"";
	  }
	}
	printf("<td%s>%s</td>", cell_decorate.c_str(), txt.c_str());
      }
      printf("</tr>\n");
    }
    printf("</table>\n");
  }
  if (full) {
    printf("</div>\n");
    printf("</body>\n");
    printf("</html>\n");
  }

  return 0;
}
