/* This file was automatically generated.  Do not edit! */
struct tm *fossil_localtime(const time_t *clock);
typedef struct Blob Blob;
int name_to_uuid(Blob *pName,int iErrPriority);
void fossil_fatal(const char *zFormat,...);
void usage(const char *zFormat);
const char *find_option(const char *zLong,const char *zShort,int hasArg);
void db_find_and_open_repository(int errIfNotFound);
void timeline_cmd(void);
const char *timeline_query_for_tty(void);
int comment_print(const char *zText,int indent,int lineLength);
int db_lget_int(const char *zName,int dflt);
typedef struct Stmt Stmt;
void print_timeline(Stmt *q,int mxLine);
void style_footer(void);
int db_finalize(Stmt *pStmt);
int db_prepare(Stmt *pStmt,const char *zFormat,...);
void cgi_redirect(const char *zURL);
void blob_init(Blob *pBlob,const char *zData,int size);
double db_double(double rDflt,const char *zSql,...);
typedef struct HQuery HQuery;
void url_add_parameter(HQuery *p,const char *zName,const char *zValue);
void url_initialize(HQuery *p,const char *zBase);
void compute_ancestors(int rid,int N);
char *blob_str(Blob *p);
void compute_descendants(int rid,int N);
void blob_appendf(Blob *pBlob,const char *zFormat,...);
char *db_text(char *zDefault,const char *zSql,...);
void login_anonymous_available(void);
void style_header(const char *zTitleFormat,...);
void login_needed(void);
void login_check_credentials(void);
const char *cgi_parameter(const char *zName,const char *zDefault);
#define P(x)        cgi_parameter((x),0)
#define PD(x,y)     cgi_parameter((x),(y))
void page_timeline(void);
char *url_render(HQuery *p,const char *zName1,const char *zValue1,const char *zName2,const char *zValue2);
void style_submenu_element(const char *zLabel,const char *zTitle,const char *zLink,...);
struct Blob {
  unsigned int nUsed;            /* Number of bytes used in aData[] */
  unsigned int nAlloc;           /* Number of bytes allocated for aData[] */
  unsigned int iCursor;          /* Next character of input to parse */
  char *aData;                   /* Where the information is stored */
  void (*xRealloc)(Blob*, unsigned int); /* Function to reallocate the buffer */
};
struct HQuery {
  Blob url;                  /* The URL */
  const char *zBase;         /* The base URL */
  int nParam;                /* Number of parameters.  Max 10 */
  const char *azName[10];    /* Parameter names */
  const char *azValue[10];   /* Parameter values */
};
char *mprintf(const char *zFormat,...);
const char *timeline_query_for_www(void);
#define GR_MAX_RAIL   32
typedef struct GraphRow GraphRow;
#define GR_MAX_PARENT 10
typedef unsigned int u32;
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
typedef struct GraphContext GraphContext;
void graph_free(GraphContext *p);
void graph_finish(GraphContext *p,int omitDescenders);
void blob_reset(Blob *pBlob);
void wiki_convert(Blob *pIn,Blob *pOut,int flags);
#define blob_buffer(X)  ((X)->aData)
void blob_append(Blob *pBlob,const char *aData,int nData);
#define blob_size(X)  ((X)->nUsed)
void db_column_blob(Stmt *pStmt,int N,Blob *pBlob);
void graph_add_row(GraphContext *p,int rid,int isLeaf,int nParent,int *aParent,const char *zBranch);
int db_reset(Stmt *pStmt);
int db_bind_int(Stmt *pStmt,const char *zParamName,int iValue);
int db_static_prepare(Stmt *pStmt,const char *zFormat,...);
# define TAG_CLOSED     9     /* Do not display this check-in as a leaf */
int db_exists(const char *zSql,...);
const char *db_column_text(Stmt *pStmt,int N);
int db_column_int(Stmt *pStmt,int N);
int db_step(Stmt *pStmt);
void blob_zero(Blob *pBlob);
int db_multi_exec(const char *zSql,...);
GraphContext *graph_init(void);
#define WIKI_NOBLOCK        0x008  /* No block markup of any kind */
#define WIKI_INLINE         0x004  /* Do not surround with <p>..</p> */
int db_get_boolean(const char *zName,int dflt);
int db_get_int(const char *zName,int dflt);
struct GraphContext {
  int nErr;             /* Number of errors encountered */
  int mxRail;           /* Number of rails required to render the graph */
  GraphRow *pFirst;     /* First row in the list */
  GraphRow *pLast;      /* Last row in the list */
  int nBranch;          /* Number of distinct branches */
  char **azBranch;      /* Names of the branches */
  int railMap[GR_MAX_RAIL];  /* Rail order mapping */
};
struct Stmt {
  Blob sql;               /* The SQL for this statement */
  sqlite3_stmt *pStmt;    /* The results of sqlite3_prepare() */
  Stmt *pNext, *pPrev;    /* List of all unfinalized statements */

  Stmt *proxy;
  int src;
  int prep_count;
};
void www_print_timeline(Stmt *pQuery,int tmFlags,void(*xExtra)(int));
#define TIMELINE_DISJOINT 0x0010  /* Elements are not contiguous */
#define TIMELINE_GRAPH    0x0008  /* Compute a graph */
#define TIMELINE_BRIEF    0x0004  /* Combine adjacent elements of same object */
#define TIMELINE_LEAFONLY 0x0002  /* Show "Leaf", but not "Merge", "Fork" etc */
#define TIMELINE_ARTID    0x0001  /* Show artifact IDs on non-check-in lines */
#define INTERFACE 0
# define TAG_BRANCH     8     /* Value is name of the current branch */
int db_int(int iDflt,const char *zSql,...);
int count_nonbranch_children(int pid);
void hyperlink_to_user(const char *zU,const char *zD,const char *zSuf);
void hyperlink_to_date(const char *zDate,const char *zSuffix);
void hyperlink_to_diff(const char *zV1,const char *zV2);
void hyperlink_to_uuid_with_mouseover(const char *zUuid,const char *zIn,const char *zOut,int id);
void cgi_printf(const char *zFormat,...);
typedef struct Global Global;
typedef struct Th_Interp Th_Interp;
#define MX_AUX  5
struct Global {
  int argc; char **argv;  /* Command-line arguments to the program */
  int isConst;            /* True if the output is unchanging */
  sqlite3 *db;            /* The connection to the databases */
  sqlite3 *dbConfig;      /* Separate connection for global_config table */
  int useAttach;          /* True if global_config is attached to repository */
  int configOpen;         /* True if the config database is open */
  long long int now;      /* Seconds since 1970 */
  int repositoryOpen;     /* True if the main repository database is open */
  char *zRepositoryName;  /* Name of the repository database */
  const char *zHome;      /* Name of user home directory */
  int localOpen;          /* True if the local database is open */
  char *zLocalRoot;       /* The directory holding the  local database */
  int minPrefix;          /* Number of digits needed for a distinct UUID */
  int fSqlTrace;          /* True if -sqltrace flag is present */
  int fSqlPrint;          /* True if -sqlprint flag is present */
  int fQuiet;             /* True if -quiet flag is present */
  int fHttpTrace;         /* Trace outbound HTTP requests */
  int fNoSync;            /* Do not do an autosync even.  --nosync */
  char *zPath;            /* Name of webpage being served */
  char *zExtra;           /* Extra path information past the webpage name */
  char *zBaseURL;         /* Full text of the URL being served */
  char *zTop;             /* Parent directory of zPath */
  const char *zContentType;  /* The content type of the input HTTP request */
  int iErrPriority;       /* Priority of current error message */
  char *zErrMsg;          /* Text of an error message */
  Blob cgiIn;             /* Input to an xfer www method */
  int cgiPanic;           /* Write error messages to CGI */
  int xferPanic;          /* Write error messages in XFER protocol */
  int fullHttpReply;      /* True for full HTTP reply.  False for CGI reply */
  Th_Interp *interp;      /* The TH1 interpreter */
  FILE *httpIn;           /* Accept HTTP input from here */
  FILE *httpOut;          /* Send HTTP output here */
  int xlinkClusterOnly;   /* Set when cloning.  Only process clusters */
  int fTimeFormat;        /* 1 for UTC.  2 for localtime.  0 not yet selected */
  int *aCommitFile;       /* Array of files to be committed */
  int markPrivate;        /* All new artifacts are private if true */

  int urlIsFile;          /* True if a "file:" url */
  int urlIsHttps;         /* True if a "https:" url */
  char *urlName;          /* Hostname for http: or filename for file: */
  char *urlHostname;      /* The HOST: parameter on http headers */
  char *urlProtocol;      /* "http" or "https" */
  int urlPort;            /* TCP port number for http: or https: */
  int urlDfltPort;        /* The default port for the given protocol */
  char *urlPath;          /* Pathname for http: */
  char *urlUser;          /* User id for http: */
  char *urlPasswd;        /* Password for http: */
  char *urlCanonical;     /* Canonical representation of the URL */
  char *urlProxyAuth;     /* Proxy-Authorizer: string */
  int dontKeepUrl;        /* Do not persist the URL */

  const char *zLogin;     /* Login name.  "" if not logged in. */
  int noPswd;             /* Logged in without password (on 127.0.0.1) */
  int userUid;            /* Integer user id */

  /* Information used to populate the RCVFROM table */
  int rcvid;              /* The rcvid.  0 if not yet defined. */
  char *zIpAddr;          /* The remote IP address */
  char *zNonce;           /* The nonce used for login */
  
  /* permissions used by the server */
  int okSetup;            /* s: use Setup screens on web interface */
  int okAdmin;            /* a: administrative permission */
  int okDelete;           /* d: delete wiki or tickets */
  int okPassword;         /* p: change password */
  int okQuery;            /* q: create new reports */
  int okWrite;            /* i: xfer inbound. checkin */
  int okRead;             /* o: xfer outbound. checkout */
  int okHistory;          /* h: access historical information. */
  int okClone;            /* g: clone */
  int okRdWiki;           /* j: view wiki via web */
  int okNewWiki;          /* f: create new wiki via web */
  int okApndWiki;         /* m: append to wiki via web */
  int okWrWiki;           /* k: edit wiki via web */
  int okRdTkt;            /* r: view tickets via web */
  int okNewTkt;           /* n: create new tickets */
  int okApndTkt;          /* c: append to tickets via the web */
  int okWrTkt;            /* w: make changes to tickets via web */
  int okTktFmt;           /* t: create new ticket report formats */
  int okRdAddr;           /* e: read email addresses or other private data */
  int okZip;              /* z: download zipped artifact via /zip URL */

  /* For defense against Cross-site Request Forgery attacks */
  char zCsrfToken[12];    /* Value of the anti-CSRF token */
  int okCsrf;             /* Anti-CSRF token is present and valid */

  FILE *fDebug;           /* Write debug information here, if the file exists */
  int thTrace;            /* True to enable TH1 debugging output */
  Blob thLog;             /* Text of the TH1 debugging output */

  int isHome;             /* True if rendering the "home" page */

  /* Storage for the aux() and/or option() SQL function arguments */
  int nAux;                    /* Number of distinct aux() or option() values */
  const char *azAuxName[MX_AUX]; /* Name of each aux() or option() value */
  char *azAuxParam[MX_AUX];      /* Param of each aux() or option() value */
  const char *azAuxVal[MX_AUX];  /* Value of each aux() or option() value */
  const char **azAuxOpt[MX_AUX]; /* Options of each option() value */
  int anAuxCols[MX_AUX];         /* Number of columns for option() values */
};
extern Global g;
#define UUID_SIZE 40
void hyperlink_to_uuid(const char *zUuid);
