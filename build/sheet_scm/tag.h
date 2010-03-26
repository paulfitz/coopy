/* This file was automatically generated.  Do not edit! */
typedef struct Stmt Stmt;
void www_print_timeline(Stmt *pQuery,int tmFlags,void(*xExtra)(int));
const char *timeline_query_for_www(void);
void tagtimeline_page(void);
void style_footer(void);
void cgi_printf(const char *zFormat,...);
void style_submenu_element(const char *zLabel,const char *zTitle,const char *zLink,...);
void style_header(const char *zTitleFormat,...);
void login_anonymous_available(void);
void login_needed(void);
void login_check_credentials(void);
void taglist_page(void);
void print_timeline(Stmt *q,int mxLine);
const char *timeline_query_for_tty(void);
void db_find_and_open_repository(int errIfNotFound);
const char *find_option(const char *zLong,const char *zShort,int hasArg);
void tag_cmd(void);
typedef struct Blob Blob;
int manifest_crosslink(int rid,Blob *pContent);
int content_put(Blob *pBlob,const char *zUuid,int srcId);
int md5sum_blob(const Blob *pIn,Blob *pCksum);
void blob_appendf(Blob *pBlob,const char *zFormat,...);
char *db_text(char *zDefault,const char *zSql,...);
char *blob_str(Blob *p);
int name_to_uuid(Blob *pName,int iErrPriority);
void blob_append(Blob *pBlob,const char *aData,int nData);
void blob_zero(Blob *pBlob);
void user_select(void);
struct Blob {
  unsigned int nUsed;            /* Number of bytes used in aData[] */
  unsigned int nAlloc;           /* Number of bytes allocated for aData[] */
  unsigned int iCursor;          /* Next character of input to parse */
  char *aData;                   /* Where the information is stored */
  void (*xRealloc)(Blob*, unsigned int); /* Function to reallocate the buffer */
};
void tag_add_artifact(const char *zPrefix,const char *zTagname,const char *zObjName,const char *zValue,int tagtype);
void db_end_transaction(int rollbackFlag);
void db_begin_transaction(void);
int content_is_private(int rid);
int name_to_rid(const char *zName);
void fossil_fatal(const char *zFormat,...);
void usage(const char *zFormat);
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
void db_must_be_within_tree(void);
void testtag_cmd(void);
# define TAG_DATE       4     /* The date of a check-in */
#define WIKI_INLINE         0x004  /* Do not surround with <p>..</p> */
void wiki_extract_links(char *z,int srcid,int srctype,double mtime,int replaceFlag,int flags);
char *mprintf(const char *zFormat,...);
# define TAG_USER       3     /* User who made a checking */
# define TAG_COMMENT    2     /* The check-in comment */
double db_double(double rDflt,const char *zSql,...);
int tag_insert(const char *zTag,int tagtype,const char *zValue,int srcId,double mtime,int rid);
i64 db_last_insert_rowid(void);
int db_multi_exec(const char *zSql,...);
int db_int(int iDflt,const char *zSql,...);
int tag_findid(const char *zTag,int createFlag);
const char *db_column_text(Stmt *pStmt,int N);
void tag_propagate_all(int pid);
int db_finalize(Stmt *pStmt);
typedef struct PQueue PQueue;
void pqueue_clear(PQueue *p);
int db_reset(Stmt *pStmt);
double db_column_double(Stmt *pStmt,int N);
int db_column_int(Stmt *pStmt,int N);
int db_step(Stmt *pStmt);
int db_bind_int(Stmt *pStmt,const char *zParamName,int iValue);
int pqueue_extract(PQueue *p);
# define TAG_BGCOLOR    1     /* Set the background color for display */
int db_bind_double(Stmt *pStmt,const char *zParamName,double rValue);
int db_prepare(Stmt *pStmt,const char *zFormat,...);
void pqueue_insert(PQueue *p,int e,double v);
void pqueue_init(PQueue *p);
struct Stmt {
  Blob sql;               /* The SQL for this statement */
  sqlite3_stmt *pStmt;    /* The results of sqlite3_prepare() */
  Stmt *pNext, *pPrev;    /* List of all unfinalized statements */

  Stmt *proxy;
  int src;
  int prep_count;
};
struct PQueue {
  int cnt;   /* Number of entries in the queue */
  int sz;    /* Number of slots in a[] */
  struct QueueElement {
    int id;          /* ID of the element */
    double value;    /* Value of element.  Kept in ascending order */
  } *a;
};
void tag_propagate(int pid,int tagid,int tagType,int origId,const char *zValue,double mtime);
