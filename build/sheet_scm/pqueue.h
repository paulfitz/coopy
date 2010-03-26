/* This file was automatically generated.  Do not edit! */
typedef struct PQueue PQueue;
int pqueue_extract(PQueue *p);
void pqueue_insert(PQueue *p,int e,double v);
void pqueue_clear(PQueue *p);
void pqueue_init(PQueue *p);
struct PQueue {
  int cnt;   /* Number of entries in the queue */
  int sz;    /* Number of slots in a[] */
  struct QueueElement {
    int id;          /* ID of the element */
    double value;    /* Value of element.  Kept in ascending order */
  } *a;
};
#define INTERFACE 0
