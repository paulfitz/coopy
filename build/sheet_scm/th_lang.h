/* This file was automatically generated.  Do not edit! */
typedef struct Th_Interp Th_Interp;
typedef int(*Th_CommandProc)(Th_Interp *,void *,int,const char **,int *);
int th_register_language(Th_Interp *interp);
int th_register_language(Th_Interp *interp);
int Th_LinkVar(Th_Interp *,const char *,int,int,const char *,int);
int th_isdigit(char);
int th_strlen(const char *);
typedef struct Th_SubCommand Th_SubCommand;
struct Th_SubCommand {char *zName; Th_CommandProc xProc;};
int Th_CallSubCommand(Th_Interp *interp,void *ctx,int argc,const char **argv,int *argl,Th_SubCommand *aSub);
int Th_CallSubCommand(Th_Interp *,void *,int,const char **,int *,Th_SubCommand *);
int Th_UnsetVar(Th_Interp *,const char *,int);
int th_isalnum(char);
int Th_RenameCommand(Th_Interp *,const char *,int,const char *,int);
int Th_CreateCommand(Th_Interp *interp,const char *zName,Th_CommandProc xProc,void *pContext,void(*xDel)(Th_Interp *,void *));
void *Th_Malloc(Th_Interp *,int);
#define TH_RETURN   3
int Th_InFrame(Th_Interp *interp,int(*xCall)(Th_Interp *,void *pContext1,void *pContext2),void *pContext1,void *pContext2);
int Th_StringAppend(Th_Interp *,char **,int *,const char *,int);
int Th_GetVar(Th_Interp *,const char *,int);
int Th_SplitList(Th_Interp *,const char *,int,char ***,int **,int *);
void Th_Free(Th_Interp *,void *);
int Th_SetResult(Th_Interp *,const char *,int);
int Th_ListAppend(Th_Interp *,char **,int *,const char *,int);
#define TH_BREAK    2
#define TH_CONTINUE 4
int Th_ToInt(Th_Interp *,const char *,int,int *);
int Th_Expr(Th_Interp *interp,const char *,int);
#define TH_OK       0
int Th_SetResultInt(Th_Interp *,int);
int Th_SetVar(Th_Interp *,const char *,int,const char *,int);
const char *Th_GetResult(Th_Interp *,int *);
int Th_Eval(Th_Interp *interp,int iFrame,const char *zProg,int nProg);
#define TH_ERROR    1
int Th_ErrorMessage(Th_Interp *,const char *,const char *,int);
int Th_WrongNumArgs(Th_Interp *interp,const char *zMsg);
int Th_WrongNumArgs(Th_Interp *interp,const char *zMsg);
