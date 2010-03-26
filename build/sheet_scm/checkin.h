/* This file was automatically generated.  Do not edit! */
typedef struct Blob Blob;
char *md5sum_finish(Blob *pOut);
void md5sum_step_blob(Blob *p);
int content_get(int rid,Blob *pBlob);
void md5sum_step_text(const char *zText,int nBytes);
void md5sum_init(void);
int name_to_rid(const char *zName);
void remove_from_argv(int i,int n);
void usage(const char *zFormat);
void import_manifest_cmd(void);
int count_nonbranch_children(int pid);
#define AUTOSYNC_PUSH  1
void undo_reset(void);
void vfile_aggregate_checksum_manifest(int vid,Blob *pOut,Blob *pManOut);
int blob_compare(Blob *pA,Blob *pB);
void vfile_aggregate_checksum_repository(int vid,Blob *pOut);
void db_lset_int(const char *zName,int value);
int manifest_crosslink(int rid,Blob *pContent);
int clearsign(Blob *pIn,Blob *pOut);
int md5sum_blob(const Blob *pIn,Blob *pCksum);
typedef struct Stmt Stmt;
int db_reset(Stmt *pStmt);
int db_bind_int(Stmt *pStmt,const char *zParamName,int iValue);
void content_make_public(int rid);
int file_isexe(const char *zFilename);
void content_deltify(int rid,int srcid,int force);
int content_put(Blob *pBlob,const char *zUuid,int srcId);
void db_end_transaction(int rollbackFlag);
void vfile_aggregate_checksum_disk(int vid,Blob *pOut);
# define TAG_CLOSED     9     /* Do not display this check-in as a leaf */
void db_blob(Blob *pResult,const char *zSql,...);
int unsaved_changes(void);
void db_record_repository_filename(const char *zName);
void db_begin_transaction(void);
void user_select(void);
#define AUTOSYNC_PULL  2
void autosync(int flags);
int content_is_private(int rid);
void verify_all_options(void);
int db_get_boolean(const char *zName,int dflt);
void url_proxy_options(void);
void commit_cmd(void);
int db_exists(const char *zSql,...);
# define TAG_BRANCH     8     /* Value is name of the current branch */
int is_a_leaf(int rid);
int db_int(int iDflt,const char *zSql,...);
void select_commit_files(void);
void blob_resize(Blob *pBlob,unsigned int newSize);
int blob_line(Blob *pFrom,Blob *pTo);
void blob_remove_cr(Blob *p);
int blob_read_from_file(Blob *pBlob,const char *zFilename);
void blob_reset(Blob *pBlob);
void fossil_panic(const char *zFormat,...);
int portable_system(char *zOrigCmd);
int blob_write_to_file(Blob *pBlob,const char *zFilename);
#if defined(__MINGW32__)
void blob_add_cr(Blob *p);
#endif
char *db_text(char *zDefault,const char *zSql,...);
char *info_tags_of_checkin(int rid,int propagatingOnly);
void prompt_user(const char *zPrompt,Blob *pIn);
void clean_cmd(void);
int file_tree_name(const char *zOrigName,Blob *pOut,int errFatal);
#define blob_size(X)  ((X)->nUsed)
void vfile_scan(int vid,Blob *pPath,int nPrefix,int allFlag);
void blob_init(Blob *pBlob,const char *zData,int size);
int db_multi_exec(const char *zSql,...);
void extra_cmd(void);
char *blob_str(Blob *p);
char *glob_expr(const char *zVal,const char *zGlobList);
const char *find_option(const char *zLong,const char *zShort,int hasArg);
void ls_cmd(void);
void show_common_info(int rid,const char *zUuidName,int showComment);
char *db_get(const char *zName,char *zDefault);
char *db_lget(const char *zName,char *zDefault);
void status_cmd(void);
#define blob_buffer(X)  ((X)->aData)
void vfile_check_signature(int vid,int notFileIsFatal);
int db_lget_int(const char *zName,int dflt);
void blob_zero(Blob *pBlob);
void db_must_be_within_tree(void);
void changes_cmd(void);
void fossil_fatal(const char *zFormat,...);
int db_finalize(Stmt *pStmt);
void fossil_warning(const char *zFormat,...);
int file_isfile(const char *zFilename);
void blob_appendf(Blob *pBlob,const char *zFormat,...);
void blob_append(Blob *pBlob,const char *aData,int nData);
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
char *mprintf(const char *zFormat,...);
int db_column_int(Stmt *pStmt,int N);
const char *db_column_text(Stmt *pStmt,int N);
int db_step(Stmt *pStmt);
int db_prepare(Stmt *pStmt,const char *zFormat,...);
struct Stmt {
  Blob sql;               /* The SQL for this statement */
  sqlite3_stmt *pStmt;    /* The results of sqlite3_prepare() */
  Stmt *pNext, *pPrev;    /* List of all unfinalized statements */

  Stmt *proxy;
  int src;
  int prep_count;
};
