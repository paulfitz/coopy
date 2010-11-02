#ifndef SSFOSSIL_DATACOLUMN
#define SSFOSSIL_DATACOLUMN

#include <coopy/DataSheet.h>

namespace coopy {
  namespace cmp {
    class Vote;
    class Nature;
    class DataColumn;
    class DataColumnPair;
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

class coopy::cmp::DataColumn {
private:
  coopy::store::DataSheet *sheet;
  int index;
  Nature nmean;
public:
  DataColumn(coopy::store::DataSheet& owner, int index) {
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
    return sheet->cellString(index,x);
  }
};

class coopy::cmp::DataColumnPair {
private:
public:
  void compare(DataColumn& a, DataColumn& b);
};


#endif
