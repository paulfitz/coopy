#ifndef COOPY_LOGDIFFRENDER
#define COOPY_LOGDIFFRENDER

#include <coopy/DiffRender.h>

#include <list>

#include <stdio.h>

namespace coopy {
  namespace light {
    class LogRow;
    class LogCell;
    class LogDiffRender;
  }
}

class coopy::light::LogRow {
public:
  int row;
  std::string mode;
};

class coopy::light::LogCell {
public:
  int col;
  int row;
  Cell cell;
  std::string cell_mode;
  std::string row_mode;
  std::string separator;
};

class coopy::light::LogDiffRender : public DiffRender {
private:
  int row, col;
  std::string row_mode;
public:

  std::list<LogRow> row_log;
  std::list<LogCell> cell_log;

  LogDiffRender() {
    row = col = 0;
  }

  virtual bool begin_table() {
    return true;
  }

  virtual bool begin_row(const std::string& mode) {
    row_mode = mode;
    LogRow item = { row, mode };
    row_log.push_back(item);
    col = 0;
    return true;
  }

  virtual bool insert_cell(const Cell& cell,
			   const std::string& mode,
			   const std::string& separator) {
    LogCell item = { col, row, cell, mode, row_mode, separator };
    cell_log.push_back(item);
    col++;
    return true;
  }

  virtual bool end_row() {
    row++;
    return true;
  }

  virtual bool end_table() {
    return true;
  }

  virtual std::string to_string() const {
    std::string result = "";
    char buf[1000]; // finite length since this is a test class
    for (std::list<LogCell>::const_iterator it = cell_log.begin();
	 it != cell_log.end(); it++) {
      sprintf(buf,"%d,%d [%s/%s] %s : %s\n", it->col, it->row,
	      it->cell_mode.c_str(),
	      it->row_mode.c_str(),
	      it->separator.c_str(),
	      it->cell.txt.c_str());
      result += buf;
    }
    return result;
  }

};

#endif
