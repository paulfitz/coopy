#ifndef COOPY_TYPEDSHEET
#define COOPY_TYPEDSHEET

#include <coopy/DataSheet.h>

#include <vector>
#include <string>

#include <stdio.h>

namespace coopy {
  namespace store {
    template <class T> class TypedSheet;
    template <class T> class EscapedTypedSheet;
  }
}

template <class T>
class coopy::store::TypedSheet : public DataSheet {
public:
  using DataSheet::insertRow;
  std::vector<std::vector<T> > arr;
  int h, w;
  T zero;

  TypedSheet() {
    h = w = 0;
  }

  void clear() {
    arr.clear();
    h = w = 0;
  }
  
  void resize(int w, int h, const T& zero) {
    arr.clear();
    for (int i=0; i<h; i++) {
      arr.push_back(std::vector<T>());
      std::vector<T>& lst = arr.back();
      for (int j=0; j<w; j++) {
	lst.push_back(zero);
      }
    }
    this->h = h;
    this->w = w;
    this->zero = zero;
  }

  int width() const {
    return w;
  }

  int height() const {
    return h;
  }

  T& cell(int x, int y) {
    return arr[y][x];
  }

  const T& cell(int x, int y) const {
    return arr[y][x];
  }

  // still need to define how T is serialized 
  virtual std::string cellString(int x, int y) const {
    return "IMPLEMENT SERIALIZATION";
  }

  virtual bool deleteColumn(const ColumnRef& column) {
    int offset = column.getIndex();
    if (offset<0) return false;
    if (offset>=w) return false;
    for (int i=0; i<(int)arr.size(); i++) {
      arr[i].erase(arr[i].begin()+offset);
    }
    w--;
    return true;
  }

  virtual ColumnRef insertColumn(const ColumnRef& base) {
    int offset = base.getIndex();
    if (offset>=w) return ColumnRef();
    for (int i=0; i<(int)arr.size(); i++) {
      if (offset>=0) {
	arr[i].insert(arr[i].begin()+offset,zero);
      } else {
	arr[i].push_back(zero);
      }
    }
    w++;
    return ColumnRef((offset>=0)?offset:ColumnRef(w-1));
  }

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    int offset = base.getIndex();
    if (offset>=(int)w) return ColumnRef();
    int offset_src = src.getIndex();
    int offset_del = offset_src;
    if (offset_del<0||offset_del>=w) return ColumnRef();
    int final = offset;
    if (offset<=offset_del&&offset!=-1) {
      offset_del++;
    } else {
      final--;
    }
    //printf("Move col: insert %d from %d offset_del %d final %d\n", 
    //offset, offset_src, offset_del, final);
    for (int i=0; i<(int)arr.size(); i++) {
      std::vector<T>& row = arr[i];
      if (offset>=0) {
	row.insert(row.begin()+offset,row[offset_src]);
      } else {
	row.push_back(row[offset_src]);
      }
      row.erase(row.begin()+offset_del);
    }
    if (offset<0) {
      return ColumnRef(w-1);
    }
    return ColumnRef(final);
  }

  virtual bool deleteRow(const RowRef& src) {
    int offset = src.getIndex();
    if (offset<0||offset>=h) return false;
    arr.erase(arr.begin()+offset);
    h--;
    return true;
  }

  virtual RowRef insertRow(const RowRef& base) {
    int offset = base.getIndex();
    if (offset>=h) return RowRef();
    if (offset<0) {
      arr.push_back(std::vector<T>());
      offset = h;
    } else {
      arr.insert(arr.begin()+offset,std::vector<T>());
    }
    h++;
    for (int i=0; i<w; i++) {
      arr[offset].push_back(zero);
    }
    return RowRef(offset);
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    int offset1 = src.getIndex();
    if (offset1>=h) return RowRef();
    int offset2 = base.getIndex();
    if (offset2>=h) return RowRef();
    //printf("Moving %d to %d\n", offset1, offset2);
    if (offset2!=-1) {
      int orig = offset1+((offset2<offset1)?1:0);
      arr.insert(arr.begin()+offset2, arr[offset1]);
      arr.erase(arr.begin()+orig);
      if (offset2>offset1) offset2--;
    } else {
      arr.push_back(arr[offset1]);
      arr.erase(arr.begin()+offset1);
      offset2 = (int)arr.size()-1;
    }
    //printf("Move resulted in %d\n", offset2);
    return RowRef(offset2);
  }

  virtual Poly<SheetRow> insertRow() {
    Poly<SheetRow> pRow = DataSheet::insertRow();
    arr.push_back(std::vector<T>());
    std::vector<T>& lst = arr.back();
    for (int j=0; j<w; j++) {
      lst.push_back(zero);
    }
    h++;
    return pRow;
  }

  void insertRowHelper() {
    arr.push_back(std::vector<T>());
    std::vector<T>& lst = arr.back();
    for (int j=0; j<w; j++) {
      lst.push_back(zero);
    }
    h++;
  }

  virtual bool applySchema(const SheetSchema& ss) {
    arr.clear();
    h = 0;
    w = ss.getColumnCount();
    return true;
  }

  virtual std::string getDescription() const {
    return "typed";
  }
};



template <class T>
class coopy::store::EscapedTypedSheet : public DataSheet {
public:
  using DataSheet::insertRow;
  typedef std::pair<T,bool> pairCellType;
  typedef T cellType;

  TypedSheet<std::pair<T,bool> > s;
  //std::vector<std::vector<pairCellType> >& arr;
  //int& h;
  //int& w;
  
  //EscapedTypedSheet() : arr(s.arr), h(s.h), w(s.w) {
  EscapedTypedSheet() {
  }

  void clear() {
    s.clear();
  }
  
  void resize(int w, int h, const T& zero) {
    std::pair<T,bool> pzero(zero,true);
    s.resize(w,h,pzero);
  }

  int width() const {
    return s.width();
  }

  int height() const {
    return s.height();
  }

  T& cell(int x, int y) {
    return s.cell(x,y).first;
  }

  const T& cell(int x, int y) const {
    return s.cell(x,y).first;
  }

  std::pair<T,bool>& pcell(int x, int y) {
    return s.cell(x,y);
  }

  const std::pair<T,bool>& pcell(int x, int y) const {
    return s.cell(x,y);
  }

  // still need to define how T is serialized 
  //virtual std::string serializeCell(const T& cell) const = 0;

  virtual std::string cellString(int x, int y) const = 0;
  //return serializeCell(s.cell(x,y).first);
  //}

  virtual std::string cellString(int x, int y, bool& escaped) const = 0;


  virtual bool cellString(int x, int y, const std::string& str, 
			  bool escaped) = 0;


  /*
    const std::pair<T,bool>& p = s.cell(x,y);
    escaped = p.second;
    return serializeCell(p.first);
  }
  */

  virtual bool deleteColumn(const ColumnRef& column) {
    return s.deleteColumn(column);
  }

  virtual ColumnRef insertColumn(const ColumnRef& base) {
    return s.insertColumn(base);
  }

  virtual ColumnRef moveColumn(const ColumnRef& src, const ColumnRef& base) {
    return s.moveColumn(src,base);
  }

  virtual bool deleteRow(const RowRef& src) {
    return s.deleteRow(src);
  }

  virtual RowRef insertRow(const RowRef& base) {
    return s.insertRow(base);
  }

  // move a row before base; if base is invalid move after all rows
  virtual RowRef moveRow(const RowRef& src, const RowRef& base) {
    return s.moveRow(src,base);
  }

  virtual Poly<SheetRow> insertRow() {
    Poly<SheetRow> pRow = DataSheet::insertRow();
    s.insertRowHelper();
    return pRow;
  }

  virtual bool applySchema(const SheetSchema& ss) {
    return s.applySchema(ss);
  }

  virtual std::string getDescription() const {
    return "escaped";
  }
};


#endif
