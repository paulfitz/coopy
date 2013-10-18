#ifndef COOPY_COOPYHXLINK_INC
#define COOPY_COOPYHXLINK_INC

#include <coopy/CoopyhxTable.h>

class CoopyhxResult {
public:
  virtual void order(int a, int b, bool row, bool p2l) = 0;
};

class CoopyhxLink {
public:
  void init();
  void probe();
  void compare(CoopyhxTable& pivot,CoopyhxTable& local,CoopyhxTable& remote,
	       CoopyhxResult& result);
};

#endif
