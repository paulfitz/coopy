/* This file was automatically generated.  Do not edit! */
typedef struct Bag Bag;
int bag_count(Bag *p);
int bag_next(Bag *p,int e);
int bag_first(Bag *p);
void bag_remove(Bag *p,int e);
int bag_find(Bag *p,int e);
int bag_insert(Bag *p,int e);
void bag_clear(Bag *p);
void bag_init(Bag *p);
struct Bag {
  int cnt;   /* Number of integers in the bag */
  int sz;    /* Number of slots in a[] */
  int used;  /* Number of used slots in a[] */
  int *a;    /* Hash table of integers that are in the bag */
};
#define INTERFACE 0
