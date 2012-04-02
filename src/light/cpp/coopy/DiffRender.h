#ifndef COOPY_DIFFRENDER
#define COOPY_DIFFRENDER

#include <string>

namespace coopy {
  namespace light {
    class Cell;
    class Table;
    class DiffRender;
  }
}

class coopy::light::Cell {
public:
  std::string txt;
  bool is_null;
};

class coopy::light::Table {
public:
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual Cell get_cell(int x, int y) const = 0;
};

class coopy::light::DiffRender {
public:
  virtual bool begin_table() = 0;
  virtual bool begin_row(const std::string& mode) = 0;
  virtual bool insert_cell(const Cell& cell,
			   const std::string& mode,
			   const std::string& separator) = 0;
  virtual bool end_row() = 0;
  virtual bool end_table() = 0;

  virtual std::string to_string() const = 0;

  virtual bool render(const Table& table);
};

#endif
