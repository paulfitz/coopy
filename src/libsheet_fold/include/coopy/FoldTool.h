#ifndef COOPY_FOLDTOOL
#define COOPY_FOLDTOOL

#include <coopy/PolyBook.h>

#include <string>
#include <set>

namespace coopy {
  namespace fold {
    class FoldOptions;
    class FoldTool;
  }
}

class coopy::fold::FoldOptions {
public:
  std::string tableName;
  coopy::store::PolyBook recipe;
  std::set<std::string> drops;
  bool fold;

  FoldOptions() {
    fold = true;
  }
};

class coopy::fold::FoldTool {
public:
  FoldTool();

  bool fold(coopy::store::PolyBook& src,
	    coopy::store::PolyBook& dest,
	    FoldOptions& options);

  bool unfold(coopy::store::PolyBook& src,
	      coopy::store::PolyBook& dest,
	      FoldOptions& options);
};

#endif
