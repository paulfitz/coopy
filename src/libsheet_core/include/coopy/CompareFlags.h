#ifndef COOPY_COMPAREFLAGS
#define COOPY_COMPAREFLAGS

#include <stdio.h>
#include <vector>
#include <set>
#include <string>

namespace coopy {
  namespace cmp {
    class CompareFlags;
    class CompareOutput;
  }
  namespace store {
    // forward declarations
    class DataSheet;
    class DataBook;
  }
}

class coopy::cmp::CompareOutput {
public:
  FILE *out;

  CompareOutput() {
    out = stdout;
  }

  CompareOutput(const char *fname) {
    out = stdout;
    setOutput(fname);
  }

  ~CompareOutput() {
    setOutput(NULL);
  }

  bool setOutput(const char *fname) {
    if (out!=stdout) {
      fclose(out);
      out = stdout;
    }
    if (fname==NULL) return true;
    out = fopen(fname,"wb");
    return (out!=NULL);
  }
};

class coopy::cmp::CompareFlags {
public:
  bool head_trimmed;
  bool tail_trimmed;
  FILE *out;
  std::set<std::string> tables;
  std::vector<std::string> ids;
  std::vector<int> ids_local;
  std::vector<int> ids_remote;
  bool trust_ids;
  bool bias_ids;
  bool trust_column_names;
  coopy::store::DataSheet *mapping;
  coopy::store::DataBook *mapping_book;
  bool pivot_sides_with_local;
  bool use_order;
  bool omit_format_name;
  bool fixed_columns;
  bool ignore_case;
  std::string pivot_uri;
  std::string local_uri;
  std::string remote_uri;

  CompareFlags() {
    head_trimmed = false;
    tail_trimmed = false;
    out = stdout;
    trust_ids = false;
    bias_ids = false;
    trust_column_names = false;
    mapping = 0 /*NULL*/;
    mapping_book = 0 /*NULL*/;
    pivot_sides_with_local = false;
    use_order = true;
    omit_format_name = false;
    fixed_columns = false;
    ignore_case = false;
  }

  ~CompareFlags() {
    setOutput(NULL);
  }

  bool setOutput(const CompareOutput& o) {
    out = o.out;
  }
};

#endif

