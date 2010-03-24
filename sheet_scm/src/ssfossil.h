#ifndef SSFOSSIL_INC
#define SSFOSSIL_INC

int ssfossil_debug();

int ssfossil_call(int argc, char *argv[]);

class FossilHandler {
public:
  virtual int exit(int result) = 0;
  virtual int printf(const char *txt) = 0;
};

void ssfossil_set_handler(FossilHandler *handler);

#endif
