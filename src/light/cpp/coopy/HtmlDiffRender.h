#ifndef COOPY_HTMLDIFFRENDER
#define COOPY_HTMLDIFFRENDER

#include <coopy/DiffRender.h>

namespace coopy {
  namespace light {
    class HtmlDiffRender;
  }
}

class coopy::light::HtmlDiffRender : public DiffRender {
public:
  HtmlDiffRender() {
    open = false;
  }

  virtual bool begin_table() {
    insert("<table class='csv_sheet'>\n");
    return true;
  }

  virtual bool begin_row(const std::string& mode) {
    td_open = "<td";
    td_close = "</td>";
    row_color = "";
    open = false;
    if (mode=="@@") {
      td_open = "<th";
	td_close = "</th>";
    } else if (mode=="!") {
      row_color = "#aaaaaa";
    } else if (mode=="+++") {
      row_color = "#7fff7f";
    } else if (mode=="---") {
      row_color = "#ff7f7f";
    } else {
      open = true;
    }
    std::string tr = "<tr>";
    std::string row_decorate = "";
    if (row_color!="") {
      row_decorate = " bgcolor=\"" + row_color + "\" style=\"background-color: " + row_color + ";\"";
      tr = "<tr" + row_decorate + ">";
    }
    insert(tr);
    return true;
  }


  virtual bool insert_cell(const Cell& cell,
			   const std::string& mode,
			   const std::string& separator) {
    std::string cell_decorate = "";
    if (mode=="+++") {
      cell_decorate += " bgcolor=\"#7fff7f\" style=\"background-color: #7fff7f;\"";
    } else if (mode=="---") {
      cell_decorate += " bgcolor=\"#ff7f7f\" style=\"background-color: #ff7f7f;\"";
    } else if (mode=="->") {
      cell_decorate += " bgcolor=\"#7f7fff\" style=\"background-color: #7f7fff;\"";
    }
    insert(td_open+cell_decorate+">");
    insert(cell.is_null?"":cell.txt);
    insert(td_close);
    return true;
  }

  virtual bool end_row() {
    insert("</tr>\n");
    return true;
  }

  virtual bool end_table() {
    insert("</table>\n");
    return true;
  }

  std::string html() const {
    return accum;
  }

  virtual std::string to_string() const {
    return html();
  }

private:

  void insert(const std::string& txt) {
    accum += txt;
  }

  std::string accum;
  std::string td_open, td_close, row_color;
  bool open;
};

#endif

