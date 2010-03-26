/* This file was automatically generated.  Do not edit! */
typedef struct Bag Bag;
int bag_find(Bag *p,int e);
int bag_insert(Bag *p,int e);
void bag_init(Bag *p);
struct Bag {
  int cnt;   /* Number of integers in the bag */
  int sz;    /* Number of slots in a[] */
  int used;  /* Number of used slots in a[] */
  int *a;    /* Hash table of integers that are in the bag */
};
typedef struct GraphContext GraphContext;
void graph_finish(GraphContext *p,int omitDescenders);
void graph_add_row(GraphContext *p,int rid,int isLeaf,int nParent,int *aParent,const char *zBranch);
char *mprintf(const char *zFormat,...);
void graph_free(GraphContext *p);
GraphContext *graph_init(void);
void fossil_panic(const char *zFormat,...);
void *safeMalloc(int nByte);
typedef struct GraphRow GraphRow;
#define GR_MAX_RAIL   32
struct GraphContext {
  int nErr;             /* Number of errors encountered */
  int mxRail;           /* Number of rails required to render the graph */
  GraphRow *pFirst;     /* First row in the list */
  GraphRow *pLast;      /* Last row in the list */
  int nBranch;          /* Number of distinct branches */
  char **azBranch;      /* Names of the branches */
  int railMap[GR_MAX_RAIL];  /* Rail order mapping */
};
typedef unsigned int u32;
#define GR_MAX_PARENT 10
struct GraphRow {
  int rid;                    /* The rid for the check-in */
  int isLeaf;                 /* True if the check-in is an open leaf */
  int nParent;                /* Number of parents */
  int aParent[GR_MAX_PARENT]; /* Array of parents.  0 element is primary .*/
  char *zBranch;              /* Branch name */

  GraphRow *pNext;            /* Next row down in the list of all rows */
  GraphRow *pPrev;            /* Previous row */
  
  int idx;                    /* Row index.  First is 1.  0 used for "none" */
  int iRail;                  /* Which rail this check-in appears on. 0-based.*/
  int aiRaiser[GR_MAX_RAIL];  /* Raisers from this node to a higher row. */
  int bDescender;             /* Raiser from bottom of graph to here. */
  u32 mergeIn;                /* Merge in from other rails */
  int mergeOut;               /* Merge out to this rail */
  int mergeUpto;              /* Draw the merge rail up to this level */

  u32 railInUse;              /* Mask of occupied rails */
};
#define INTERFACE 0
