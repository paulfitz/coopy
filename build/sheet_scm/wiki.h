/* This file was automatically generated.  Do not edit! */
typedef struct Blob Blob;
int blob_read_from_file(Blob *pBlob,const char *zFilename);
int blob_read_from_channel(Blob *pBlob,FILE *in,int nToRead);
void usage(const char *zFormat);
void db_find_and_open_repository(int errIfNotFound);
void wiki_cmd(void);
#define AUTOSYNC_PUSH  1
void autosync(int flags);
void user_select(void);
void fossil_fatal(const char *zFormat,...);
int wiki_cmd_commit(char const *zPageName,int isNew,Blob *pContent);
void wikirules_page(void);
void wfind_page(void);
typedef struct Stmt Stmt;
const char *db_column_text(Stmt *pStmt,int N);
int db_step(Stmt *pStmt);
void wcontent_page(void);
int *text_diff(Blob *pA_Blob,Blob *pB_Blob,Blob *pOut,int nContext);
void wdiff_page(void);
int db_finalize(Stmt *pStmt);
#define TIMELINE_ARTID    0x0001  /* Show artifact IDs on non-check-in lines */
void www_print_timeline(Stmt *pQuery,int tmFlags,void(*xExtra)(int));
int db_prepare(Stmt *pStmt,const char *zFormat,...);
const char *timeline_query_for_www(void);
struct Blob {
  unsigned int nUsed;            /* Number of bytes used in aData[] */
  unsigned int nAlloc;           /* Number of bytes allocated for aData[] */
  unsigned int iCursor;          /* Next character of input to parse */
  char *aData;                   /* Where the information is stored */
  void (*xRealloc)(Blob*, unsigned int); /* Function to reallocate the buffer */
};
struct Stmt {
  Blob sql;               /* The SQL for this statement */
  sqlite3_stmt *pStmt;    /* The results of sqlite3_prepare() */
  Stmt *pNext, *pPrev;    /* List of all unfinalized statements */

  Stmt *proxy;
  int src;
  int prep_count;
};
void whistory_page(void);
#define blob_size(X)  ((X)->nUsed)
char *blob_str(Blob *p);
void fossil_redirect_home(void);
void wikiappend_page(void);
void wikinew_page(void);
void login_insert_csrf_secret(void);
void blob_append(Blob *pBlob,const char *aData,int nData);
void db_end_transaction(int rollbackFlag);
void content_deltify(int rid,int srcid,int force);
int manifest_crosslink(int rid,Blob *pContent);
int db_multi_exec(const char *zSql,...);
int content_put(Blob *pBlob,const char *zUuid,int srcId);
int md5sum_blob(const Blob *pIn,Blob *pCksum);
void blob_appendf(Blob *pBlob,const char *zFormat,...);
char *db_text(char *zDefault,const char *zSql,...);
void login_verify_csrf_secret(void);
void db_set(const char *zName,const char *zValue,int globalFlag);
void db_begin_transaction(void);
const char *cgi_parameter(const char *zName,const char *zDefault);
#define PD(x,y)     cgi_parameter((x),(y))
void wikiedit_page(void);
typedef struct Manifest Manifest;
void manifest_clear(Manifest *p);
void blob_reset(Blob *pBlob);
void wiki_convert(Blob *pIn,Blob *pOut,int flags);
void blob_init(Blob *pBlob,const char *zData,int size);
void style_submenu_element(const char *zLabel,const char *zTitle,const char *zLink,...);
#define CFTYPE_WIKI       4
int manifest_parse(Manifest *p,Blob *pContent);
int content_get(int rid,Blob *pBlob);
void blob_zero(Blob *pBlob);
int db_int(int iDflt,const char *zSql,...);
#define P(x)        cgi_parameter((x),0)
void login_needed(void);
char *mprintf(const char *zFormat,...);
struct Manifest {
  Blob content;         /* The original content blob */
  int type;             /* Type of file */
  int mode;             /* Access mode */
  char *zComment;       /* Decoded comment */
  double rDate;         /* Time in the "D" line */
  char *zUser;          /* Name of the user */
  char *zRepoCksum;     /* MD5 checksum of the baseline content */
  char *zWiki;          /* Text of the wiki page */
  char *zWikiTitle;     /* Name of the wiki page */
  char *zTicketUuid;    /* UUID for a ticket */
  int nFile;            /* Number of F lines */
  int nFileAlloc;       /* Slots allocated in aFile[] */
  struct { 
    char *zName;           /* Name of a file */
    char *zUuid;           /* UUID of the file */
    char *zPerm;           /* File permissions */
    char *zPrior;          /* Prior name if the name was changed */
    int iRename;           /* index of renamed name in prior/next manifest */
  } *aFile;
  int nParent;          /* Number of parents */
  int nParentAlloc;     /* Slots allocated in azParent[] */
  char **azParent;      /* UUIDs of parents */
  int nCChild;          /* Number of cluster children */
  int nCChildAlloc;     /* Number of closts allocated in azCChild[] */
  char **azCChild;      /* UUIDs of referenced objects in a cluster */
  int nTag;             /* Number of T lines */
  int nTagAlloc;        /* Slots allocated in aTag[] */
  struct { 
    char *zName;           /* Name of the tag */
    char *zUuid;           /* UUID that the tag is applied to */
    char *zValue;          /* Value if the tag is really a property */
  } *aTag;
  int nField;           /* Number of J lines */
  int nFieldAlloc;      /* Slots allocated in aField[] */
  struct { 
    char *zName;           /* Key or field name */
    char *zValue;          /* Value of the field */
  } *aField;
  int nAttach;          /* Number of A lines */
  int nAttachAlloc;     /* Slots allocated in aAttach[] */
  struct { 
    char *zUuid;           /* UUID of the attachment */
    char *zName;           /* Name of the attachment */
    char *zDesc;           /* Description of the attachment */
  } *aAttach;
};
void wiki_page(void);
void cgi_set_parameter_nocopy(const char *zName,const char *zValue);
void cgi_redirectf(const char *zFormat,...);
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
void login_check_credentials(void);
char *db_get(const char *zName,char *zDefault);
void home_page(void);
void style_footer(void);
void style_header(const char *zTitleFormat,...);
void cgi_printf(const char *zFormat,...);
int wiki_name_is_wellformed(const unsigned char *z);
