/* This file was automatically generated.  Do not edit! */
#if defined(__MINGW32__           /* This code is for win32 only */)
void win32_http_server(int mnPort,int mxPort,char *zBrowser,char *zStopper,char *zNotFound);
#endif
int cgi_http_server(int mnPort,int mxPort,char *zBrowser);
int db_get_int(const char *zName,int dflt);
typedef struct Blob Blob;
void blob_zero(Blob *pBlob);
void cmd_webserver(void);
void login_set_capabilities(const char *zCap);
void cmd_test_http(void);
void cgi_handle_http_request(const char *zIpAddr);
void cmd_http(void);
int file_simplify_name(char *z,int n);
void db_must_be_within_tree(void);
void cgi_init(void);
void cgi_panic(const char *zFormat,...);
void db_close(void);
void cgi_setenv(const char *zName,const char *zValue);
void blob_reset(Blob *pBlob);
# define blob_eq(B,S) \
     ((B)->nUsed==sizeof(S)-1 && memcmp((B)->aData,S,sizeof(S)-1)==0)
#define blob_buffer(X)  ((X)->aData)
int blob_token(Blob *pFrom,Blob *pTo);
int blob_line(Blob *pFrom,Blob *pTo);
int blob_read_from_file(Blob *pBlob,const char *zFilename);
void cmd_cgi(void);
void cgi_set_parameter_nocopy(const char *zName,const char *zValue);
int dehttpize(char *z);
void db_open_repository(const char *zDbName);
void cgi_replace_parameter(const char *zName,const char *zValue);
void cgi_set_status(int iStat,const char *zStat);
void cgi_redirect(const char *zURL);
i64 file_size(const char *zFilename);
const char *cgi_parameter(const char *zName,const char *zDefault);
#define P(x)        cgi_parameter((x),0)
int file_isdir(const char *zFilename);
char *blob_str(Blob *p);
void file_canonical_name(const char *zOrigName,Blob *pOut);
char *db_get(const char *zName,char *zDefault);
void cgi_redirectf(const char *zFormat,...);
void fossil_redirect_home(void);
char *mprintf(const char *zFormat,...);
#define PD(x,y)     cgi_parameter((x),(y))
void set_base_url(void);
void help_cmd(void);
#define MANIFEST_DATE "2010/03/25"
#define MANIFEST_VERSION "coopy_011"
void version_cmd(void);
void cmd_test_cmd_list(void);
void cmd_cmd_list(void);
void verify_all_options(void);
void remove_from_argv(int i,int n);
void usage(const char *zFormat);
void fossil_warning(const char *zFormat,...);
void fossil_fatal_recursive(const char *zFormat,...);
void fossil_fatal(const char *zFormat,...);
void db_force_rollback(void);
void cgi_reply(void);
void cgi_printf(const char *zFormat,...);
char *vmprintf(const char *zFormat,va_list ap);
void fossil_panic(const char *zFormat,...);
const char *find_option(const char *zLong,const char *zShort,int hasArg);
int _manifest_setup();
int _verify_setup();
int db_reset_all();
int main(int argc,char **argv);
void cgi_debug(const char *zFormat,...);
typedef struct Global Global;
struct Blob {
  unsigned int nUsed;            /* Number of bytes used in aData[] */
  unsigned int nAlloc;           /* Number of bytes allocated for aData[] */
  unsigned int iCursor;          /* Next character of input to parse */
  char *aData;                   /* Where the information is stored */
  void (*xRealloc)(Blob*, unsigned int); /* Function to reallocate the buffer */
};
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
#define CGIDEBUG(X)  if( g.fDebug ) cgi_debug X
#define UUID_SIZE 40
#define count(X)  (sizeof(X)/sizeof(X[0]))
#define INTERFACE 0
