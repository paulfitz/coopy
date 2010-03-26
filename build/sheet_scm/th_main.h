/* This file was automatically generated.  Do not edit! */
typedef struct Blob Blob;
int blob_read_from_file(Blob *pBlob,const char *zFilename);
void blob_zero(Blob *pBlob);
void usage(const char *zFormat);
void test_th_render(void);
typedef struct Th_Interp Th_Interp;
int Th_Eval(Th_Interp *interp,int iFrame,const char *zProg,int nProg);
int Th_Render(const char *z);
const char *Th_GetResult(Th_Interp *,int *);
int Th_GetVar(Th_Interp *,const char *,int);
int Th_UnsetVar(Th_Interp *,const char *,int);
void Th_Unstore(const char *zName);
int Th_SetVar(Th_Interp *,const char *,int,const char *,int);
void Th_Store(const char *zName,const char *zValue);
typedef int(*Th_CommandProc)(Th_Interp *,void *,int,const char **,int *);
int Th_CreateCommand(Th_Interp *interp,const char *zName,Th_CommandProc xProc,void *pContext,void(*xDel)(Th_Interp *,void *));
int th_register_language(Th_Interp *interp);
int th_register_language(Th_Interp *interp);
typedef struct Th_Vtab Th_Vtab;
struct Th_Vtab {
  void *(*xMalloc)(unsigned int);
  void (*xFree)(void *);
};
Th_Interp *Th_CreateInterp(Th_Vtab *pVtab);
void Th_FossilInit(void);
void Th_Free(Th_Interp *,void *);
#define blob_size(X)  ((X)->nUsed)
#define blob_buffer(X)  ((X)->aData)
char *mprintf(const char *zFormat,...);
char *blob_str(Blob *p);
char *Th_Fetch(const char *zName,int *pSize);
int Th_SplitList(Th_Interp *,const char *,int,char ***,int **,int *);
#define TH_ERROR    1
int Th_SetResultInt(Th_Interp *,int);
int login_has_capability(const char *zCap,int nCap);
char *db_text(char *zDefault,const char *zSql,...);
int Th_SetResult(Th_Interp *,const char *,int);
void blob_reset(Blob *pBlob);
#define WIKI_INLINE         0x004  /* Do not surround with <p>..</p> */
void wiki_convert(Blob *pIn,Blob *pOut,int flags);
void blob_init(Blob *pBlob,const char *zData,int size);
struct Blob {
  unsigned int nUsed;            /* Number of bytes used in aData[] */
  unsigned int nAlloc;           /* Number of bytes allocated for aData[] */
  unsigned int iCursor;          /* Next character of input to parse */
  char *aData;                   /* Where the information is stored */
  void (*xRealloc)(Blob*, unsigned int); /* Function to reallocate the buffer */
};
#define TH_OK       0
void cgi_append_content(const char *zData,int nAmt);
char *htmlize(const char *zIn,int n);
int Th_ToInt(Th_Interp *,const char *,int,int *);
int Th_WrongNumArgs(Th_Interp *interp,const char *zMsg);
int Th_WrongNumArgs(Th_Interp *interp,const char *zMsg);
typedef struct Global Global;
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
void blob_vappendf(Blob *pBlob,const char *zFormat,va_list ap);
void Th_Trace(const char *zFormat,...);
