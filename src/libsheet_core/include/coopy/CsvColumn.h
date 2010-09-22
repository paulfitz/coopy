#ifndef SSFOSSIL_CSVCOLUMN
#define SSFOSSIL_CSVCOLUMN

#include <coopy/CsvSheet.h>

namespace coopy {
  namespace cmp {
    class Vote;
    class Nature;
    class CsvColumn;
    class CsvColumnPair;
  }
}

class coopy::cmp::Vote {
public:
  int votes;
  float confidence;
  float yes;
  float no;

  Vote() {
    clear();
  }

  void clear() {
    votes = 0;
    confidence = 0;
    yes = no = 0;
  }

  void vote(float vote, float confidence) {
    if (confidence>1) confidence = 1;
    if (confidence<0) confidence = 0;
    if (vote>0) yes += vote*confidence;
    if (vote<0) no += vote*confidence;
    this->confidence += confidence;
    votes++;
  }

  float result() {
    float v = yes+no;
    if (votes>0) {
      v /= votes;
    }
    return v;
  }
};

class coopy::cmp::Nature {
public:
  Vote web;
  Vote email;
  Vote text;
  Vote number;

  void evaluate(const char *txt);  

  float compare(const char *txt);

  float confidence();

  void clear() {
    web.clear();
    email.clear();
    text.clear();
    number.clear();
  }
};

class coopy::cmp::CsvColumn {
private:
  coopy::store::CsvSheet *sheet;
  int index;
  Nature nmean;
public:
  CsvColumn(coopy::store::CsvSheet& owner, int index) {
    sheet = &owner;
    this->index = index;
  }

  void evaluate();
  
  Nature getNature() { return nmean; }

  int height() {
    return sheet->height();
  }

  int size() {
    return sheet->height();
  }

  std::string cell(int x) {
    return sheet->cell(index,x);
  }
};

class coopy::cmp::CsvColumnPair {
private:
public:
  void compare(CsvColumn& a, CsvColumn& b);
};


#endif
