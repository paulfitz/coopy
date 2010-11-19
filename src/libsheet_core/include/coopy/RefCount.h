#ifndef COOPY_REFCOUNT
#define COOPY_REFCOUNT

namespace coopy {
  namespace store {
    class RefCount;
    template <class T> class Poly;
  }
}

class coopy::store::RefCount {
private:
  int ref_ct;
public:
  RefCount() {
    ref_ct = 0;
  }

  virtual ~RefCount() {
  }

  int addReference() {
    ref_ct++;
    return ref_ct;
  }

  int removeReference() {
    ref_ct--;
    return ref_ct;
  }

  int getReferenceCount() {
    return ref_ct;
  }
};

template <class T>
class coopy::store::Poly {
private:
  T *ref;
  bool owned;
public:
  Poly() {
    ref = 0/*NULL*/;
    owned = false;
  }

  Poly(T *ref, bool owned) : ref(ref), owned(owned) {
    if (ref!=0/*NULL*/&&owned) {
      ref->addReference();
    }
  }

  Poly(const Poly& alt) {
    owned = alt.owned;
    ref = alt.ref;
    if (ref!=0/*NULL*/&&owned) {
      ref->addReference();
    }
  }

  virtual ~Poly() {
    clear();
  }

  void clear() {
    if (ref==0/*NULL*/) return;
    if (owned) {
      int ct = ref->removeReference();
      if (ct==0) {
	delete ref;
      }
    }
    ref = 0/*NULL*/;
  }

  bool isValid() const {
    return ref!=0/*NULL*/;
  }

  T& operator *() {
    return *ref;
  }

  T *operator ->() {
    return ref;
  }

  T *getContent() {
    return ref;
  }


};

#endif
