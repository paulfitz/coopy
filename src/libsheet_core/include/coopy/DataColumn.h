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

  void vote(float vote, float confidence, bool forward=true) {
    if (confidence>1) confidence = 1;
    if (confidence<0) confidence = 0;
    float vote2 = vote;
    if (!forward) vote2*=-1;
    if (vote>0) yes += vote2*confidence;
    if (vote<0) no += vote2*confidence;
    this->confidence += confidence;
    if (forward) votes++; else votes--;
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
  Vote type_integer;
  Vote nully;

  void evaluate(const char *txt, bool forward = true);  

  float compare(const char *txt, bool forward = true);

  float uncompare(const char *txt) {
    return compare(txt,false);
  }

  float confidence();

  void clear() {
    web.clear();
    email.clear();
    text.clear();
    number.clear();
    type_integer.clear();
    nully.clear();
  }

  void show();

  bool couldBeInteger() {
    return (type_integer.yes>0.5) && (type_integer.no>-0.5);
  }
};

class coopy::cmp::DataColumn {
private:
  const coopy::store::DataSheet *sheet;
  int index;
  Nature nmean;
  int hh;
public:
  DataColumn(const coopy::store::DataSheet& owner, int index, int height) {
    sheet = &owner;
    this->index = index;
    hh = height;
  }

  void evaluate();

  void unevaluate(int top);
  
  Nature getNature() { return nmean; }

  int height() {
    return hh;
  }

  int size() {
    return hh;
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
