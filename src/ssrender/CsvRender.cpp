
#include <coopy/CsvRender.h>

using namespace std;
using namespace coopy::store;

string csv2html_css();

static void replace(string& str, const string& old, const string& rep) {
  size_t pos = 0;
  while((pos = str.find(old, pos)) != std::string::npos) {
    str.replace(pos, old.length(), rep);
    pos += rep.length();
  }
}

// assume text is in, and should remain, utf8
string ml_encode(string x, bool dox) {
  replace(x,"&amp;","&");
  replace(x,"&lt;","<");
  replace(x,"&rt;",">");
  replace(x,"&","&amp;");
  replace(x,"<","&lt;");
  if (!dox) {
    replace(x,">","&rt;");
  }

  if (dox) {
    replace(x,"@","\\@");
  }

  if (x.find("http://")==0) {
    replace(x,"\"","%22");
    x = string("<a href=\"") + x +"\">" + x + "</a>";
  }

  return x;
}

string CsvRender::renderHtml(const DataSheet& sheet, const std::string& title) {
  string result = "";

  if (full) {
    result += "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
    result += "<html>\n";
    result += "<head>\n";
    result += "<title>csv2html</title>\n";
    result += "<style>\n";
    result += csv2html_css();
    result += "</style>\n";
    result += "</head>\n";
    result += "<body>\n";
  }
  if (sheet.width()>=1 && sheet.height()>=1) {
    result += "<table";
    if (decorate) {
      result += " class=\"csv_sheet\"";
    }
    result += ">\n";

    //if (decorate) {
    //if (sheet.cellString(0,0)[0]!='[') {
    //	decorate = false;
    //}
    //}

    if (header) {
      SheetSchema *schema = sheet.getSchema();
      if (title!="sheet") {
	result += "  <caption>";
	result += title; //schema->getSheetName();
	result += "</caption>\n";
      }
      if (schema!=NULL) {
	result += "  <tr>";
	for (int i=0; i<schema->getColumnCount(); i++) {
	  result += "<th>";
	  result += ml_encode(schema->getColumnInfo(i).getName(),dox);
	  result += "</th>";
	}
	result += "</tr>\n";
      }
    }

    for (int i=0; i<sheet.height(); i++) {
      string row_mode = "";
      string txt = sheet.cellString(0,i);
      string bit = "td";
      string row_color = "";
      if (header) {
	if (txt=="@"||txt=="@@") {
	  bit = "th";
	} else if (txt=="+++") {
	  row_color = "#7fff7f";
	} else if (txt=="---") {
	  row_color = "#ff7f7f";
	}
      }
      if (txt == "[---]" || txt == "---") {
	row_mode = "csv_row_mmm";
      } else if (txt == "[+++]" || txt == "+++") {
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
      if (row_color!="") {
	row_decorate = string(" style=\"background-color: ") + row_color + ";\"";
      }
      result += "  <tr";
      result += row_decorate;
      result += ">";
      for (int j=0; j<sheet.width(); j++) {
	string txt = sheet.cellString(j,i);
	if (j==0&&header) {
	  if (txt=="NULL") txt = "";
	}
	string cell_decorate = "";
	if (header) {
	  if (txt.find("->")!=string::npos) {
	    cell_decorate += " style=\"background-color: #7f7fff;\"";
	  }
	}
	txt = ml_encode(txt,dox);
	if (decorate) {
	  if (txt!="") {
	    txt = string("&nbsp;") + txt + "&nbsp;";
	  }
	  if (j==0) {
	    cell_decorate += " class=\"csv_cmd\"";
	  }
	}
	result += "<";
	result += bit;
	result += cell_decorate;
	result += ">";
	result += txt;
	result += "</";
	result += bit;
	result += ">";
      }
      result += "</tr>\n";
    }
    result += "</table>\n";
  }
  if (full) {
    result += "</body>\n";
    result += "</html>\n";
  }

  return result;
}


