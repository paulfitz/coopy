// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

// wxwidgets library version issue
#ifdef __APPLE__
#ifdef NDEBUG
#undef NDEBUG
#endif
#endif

#include <wx/wx.h>
#include <wx/wxprec.h>
#include <wx/dcbuffer.h>
#include <wx/image.h>
#include <wx/cmdline.h>
#include <wx/dirdlg.h>
#include <wx/textdlg.h>
#include <wx/filefn.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/textctrl.h>
#include <wx/url.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/listctrl.h>
#include <wx/regex.h>

#include <string>
#include <list>
#include <map>
#include <iostream>

#include <unistd.h>

#include <coopy/Dbg.h>
#include <coopy/Options.h>

static bool verb() { coopy_is_verbose(); }

// hack to remove warning
#define static const
#include "icon/appicon.xpm"
#undef static

#define SITE_NAME "share.find.coop"
#define SITE_NAME_CREATE "http://share.find.coop/repo/new"

using namespace std;

//long int g_hinstance = 0;
//long int g_hwnd = 0;

#include "WideSheetManager.h"

static wxString conv_c(const char *s) {
    return wxString(s, wxConvUTF8);
}

static wxString conv(const std::string& s) {
    return wxString(s.c_str(), wxConvUTF8);
}

static std::string conv(const wxString& s) {
    return std::string(s.mb_str(wxConvUTF8));
} 


static void show(const wxString& view) {
#ifndef WIN32
    wxString view2 = wxT("file://") + view;
    ::wxLaunchDefaultBrowser(view2);
#else
    ::wxLaunchDefaultBrowser(view);
#endif
}

static void show(const std::string& view) {
    wxString x = conv(view);
    show(x);
}


class CoopyApp: public wxApp {
public:
    CoopyApp() {
        silent = false;
        force_happy = false;
    }

    virtual bool OnInit();

    virtual int OnRun() {
        if (!force_happy) {
            return wxApp::OnRun();
        }
        return 0;
    }

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    static string fossil_object;
    static string fossil_action;
    static string fossil_message;
    static string fossil_key;
    static string fossil_repo;
    static string fossil_target;
    static bool fossil_autoend;
    static bool silent;
    static int fossil_result;
    static bool force_happy;
    static wxFrame *store_frame;

    void OnKey(wxKeyEvent&);

    DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(CoopyApp, wxApp)
  EVT_KEY_DOWN(CoopyApp::OnKey)
END_EVENT_TABLE()

string CoopyApp::fossil_object;
string CoopyApp::fossil_action;
string CoopyApp::fossil_message;
string CoopyApp::fossil_key;
string CoopyApp::fossil_repo;
string CoopyApp::fossil_target;
bool CoopyApp::fossil_autoend = false;
bool CoopyApp::silent = false;
bool CoopyApp::force_happy = false;
int CoopyApp::fossil_result = 0;
wxFrame *CoopyApp::store_frame = NULL;

static const wxCmdLineEntryDesc g_cmdLineDesc [] = {
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("displays help on the command line parameters") },
    { wxCMD_LINE_SWITCH, wxT("H"), wxT("help-dox"), wxT("prepare doxygen help") },
    //wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, wxT("g"), wxT("gui"), wxT("force show GUI") },
    { wxCMD_LINE_SWITCH, wxT("n"), wxT("new"), wxT("new repository") },
    { wxCMD_LINE_OPTION, wxT("c"), wxT("clone"), wxT("clone repository"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_SWITCH, wxT("v"), wxT("verbose"), wxT("show debug information") },
    { wxCMD_LINE_SWITCH, wxT("l"), wxT("silent"), wxT("keep it quiet") },
    //{ wxCMD_LINE_OPTION, wxT("r"), wxT("res"), wxT("set resource location"),
    //wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_SWITCH, wxT("p"), wxT("pull"), wxT("pull in data") },
    { wxCMD_LINE_SWITCH, wxT("s"), wxT("push"), wxT("push out data") },
    { wxCMD_LINE_OPTION, wxT("k"), wxT("key"), wxT("key for adding/export"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_OPTION, wxT("r"), wxT("repo"), wxT("repository link"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_OPTION, wxT("a"), wxT("add"), wxT("add a spreadsheet/database"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_OPTION, wxT("e"), wxT("export"), wxT("export a spreadsheet/database"),
      wxCMD_LINE_VAL_STRING, 0,
    },
    { wxCMD_LINE_OPTION, wxT("m"), wxT("message"), wxT("message for log"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_PARAM, NULL, NULL, wxT("input file"), wxCMD_LINE_VAL_STRING,
      wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH, wxT("d"), wxT("delay"), wxT("add fossil delay") },
    { wxCMD_LINE_NONE },
};


void CoopyApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("--"));
}
 
bool CoopyApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    bool help = parser.Found(wxT("h"));
    bool help_dox = parser.Found(wxT("H"));
    if (help||help_dox) {
        coopy::app::Options opt("coopy");
        if (help_dox) {
            opt.setBool("help-doxygen",true);
        }
        opt.setBool("show-patch",false);
        opt.setBool("show-input-format",false);
        opt.beginHelp();
        opt.addUsage("coopy [options]");
        opt.addUsage("coopy [options] DIRECTORY");
        opt.addDescription("Manage a repository of spreadsheets and databases. Usually run without options, for a graphical interface.");
        opt.showOptions(OPTION_FOR_COOPY);
        opt.addExample("coopy",
                       "run the coopy GUI from the current directory. All the actions in these examples can be achieved from the GUI.");
        opt.addExample("coopy --new",
                       "create a new empty repository in the current directory.");
        opt.addExample("coopy --key=people --add=people.xls",
                       "add people.xls to the repository, with key 'people'.");
        opt.addExample("coopy --key=orgs --export=organizations.sqlite",
                       "export organizations.sqlite from the repository, with key 'orgs'.");
        opt.endHelp();
        force_happy = true;
        return true;
    }

    silent = parser.Found(wxT("l"));

    //wxString location;
    //if (parser.Found(wxT("r"),&location)) {
    //string loc = conv(location);
    //  printf("*** should set resource location to [%s]\n", loc.c_str());
    //}
    wxString message, key;
    if (parser.Found(wxT("k"),&key)) {
        fossil_key = conv(key);
    }
    if (parser.Found(wxT("n"))) {
        fossil_action = "new";
    }
    if (parser.Found(wxT("d"))) {
        // command-line use of coopy currently can run into trouble
        // with timestamps if commands issued in quick succession
#ifdef __WIN32__
        Sleep(1000);
#else
        sleep(1);
#endif
    }
    if (parser.Found(wxT("c"),&key)) {
        fossil_action = "clone";
        wxURL url = key;
        if (!url.HasScheme()) {
            wxFileName name = wxFileName::FileName(key);
            name.MakeAbsolute();
            fossil_target = conv(name.GetFullPath());
        } else {
            fossil_target = conv(key);
        }
    }
    if (parser.Found(wxT("r"),&key)) {
        fossil_repo = conv(key);
    }
    if (parser.Found(wxT("m"),&message)) {
        fossil_message = conv(message);
    }
    if (parser.Found(wxT("a"),&message)) {
        fossil_action = "add";
        wxFileName name = wxFileName::FileName(message);
        name.MakeAbsolute();
        fossil_message = conv(name.GetFullPath());
    }
    if (parser.Found(wxT("e"),&message)) {
        fossil_action = "export";
        wxFileName name = wxFileName::FileName(message);
        name.MakeAbsolute();
        fossil_message = conv(name.GetFullPath());
    }
    if (parser.Found(wxT("p"))) {
        fossil_action = "pull";
    }
    if (parser.Found(wxT("s"))) {
        fossil_action = "push";
    }
    if (parser.Found(wxT("v"))) {
        coopy_set_verbose(true);
    }
    fossil_autoend = false;
    if (fossil_action!="") {
        fossil_autoend = !parser.Found(wxT("g"));
        if (fossil_autoend) {
            silent = true;
        }
    }
    
    // to get at your unnamed parameters use
    wxArrayString files;
    for (size_t i = 0; i < parser.GetParamCount(); i++) {
        files.Add(parser.GetParam(i));
    }

    if (files.size()>0) {
        fossil_object = conv(files[0]);
    }
    
    // and other command line parameters

    // then do what you need with them.
    
    return true;
}

#ifdef WIN32
IMPLEMENT_APP_NO_MAIN(CoopyApp);
#else
IMPLEMENT_APP(CoopyApp);
#endif


class CoopyFrame: public wxFrame
{
    DECLARE_CLASS(CoopyFrame)
    DECLARE_EVENT_TABLE()

public:
    bool background;

private:
    wxBoxSizer *topsizer;
    //wxTextCtrl* m_textCtrl;

    wxTextCtrl *log_box;
    wxTextCtrl *src_box;
    wxTextCtrl *dest_box;
    wxDirPickerCtrl *dir_box;
    string dir_box_path;
    wxTimer *timer;
    wxListBox *list_box;
    wxInputStream *report;
    wxInputStream *reportErr;

    WideSheetManager ws;

    //UiFossilHandler handler; 

    bool askPath;
    bool askSource;
    bool askDestination;

    string path;
    string source;
    string destination;
    string commit_message;
    string fossil_path;
    ostream *stream;
    string next;
    string retry;
    bool logging;
    bool showing;
    bool writeAuthorizationFailed;
    bool writeWouldFork;
    wxString autoSyncTip;
    wxString projectCodeTip;
    list<string> results;
    list<string> fileCache;
    list<string> updateCache;

    void split(const string& str, 
               const string& delimiters, 
               list<string>& tokens) {
        size_t lastPos = str.find_first_not_of(delimiters, 0);
        size_t pos = str.find_first_of(delimiters, lastPos);
        while (string::npos != pos || string::npos != lastPos) {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }

    void beginLog() {
        results.clear();
        logging = true;
    }

    list<string> endLog() {
        list<string> r = results;
        results.clear();
        logging = false;
        return r;
    }

    void replace(std::string& str, const std::string& old, 
                 const std::string& rep) {
        size_t pos = 0;
        while((pos = str.find(old, pos)) != std::string::npos) {
            str.replace(pos, old.length(), rep);
            pos += rep.length();
        }
    }

    string safetxt(const char *txt) {
        string str(txt);
        //replace(str," ","\\ ");
#ifdef WIN32
        replace(str,"\\","/");
#endif
#ifdef WIN32
        str = string("\"") + str + "\"";
#endif
        return str;
    }

public:

    CoopyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnExit(wxCloseEvent& event) {
        Destroy();
    }

    void OnReset(wxCommandEvent& event);

    virtual bool OnInit();

    void OnSync(wxCommandEvent& event);
    void OnPush(wxCommandEvent& event);
    void OnCommit(wxCommandEvent& event);
    void OnCreate(wxCommandEvent& event);
    bool OnCreateRepo(wxCommandEvent& event);
    bool OnCloneRepo(wxCommandEvent& event);
    bool OnOpenRepo(bool back = true);
    void OnUndo(wxCommandEvent& event);

    bool havePath();
    bool haveSource();
    bool haveDestination();

    char *fossil() {
        if (fossil_path=="") {
            wxFileName name = wxFileName::FileName(wxStandardPaths::Get().GetExecutablePath());
            name.SetName(_T("ssfossil"));
            fossil_path = conv(name.GetFullPath());
        }
        return (char*)fossil_path.c_str();
    }

    int ssfossil(int argc, char *argv[], bool sync=false);

    void OnProgressTimer(wxTimerEvent& WXUNUSED(event)) {
        //printf("Tick!\n");
        processInput();
    }

    void processSubInput(wxInputStream& in) {
        wxTextInputStream tis(in);
        wxString str = tis.ReadLine();
        //printf("Got %s\n", conv(str).c_str());
        //replace(str,string("\r"),string(" * "));
        //replace(str,"Received:","\nReceived:");
        if (logging) { 
            //printf("Logging\n");
            string cp(conv(str));
            split(cp,"\n",results);
            //printf("Logged\n");
        }
        //wxString n = conv(next);
        addLog(str);
    }

    void processInput() {
        while (report->CanRead()) {
            processSubInput(*report);
        }
        while (reportErr->CanRead()) {
            processSubInput(*reportErr);
        }
    }

    void addLog(const wxString& str, bool force = false) {
        /*
        char buf[256];
        sprintf(buf,"[%d]", (int)str[0]);
        log_box->AppendText(wxT(">>>"));
        log_box->AppendText(str);
        log_box->AppendText(wxT("--"));
        log_box->AppendText(conv(string(buf)));
        log_box->AppendText(wxT("\n"));
        */

        if (str.Contains(wxT("Autosync: "))) {
            autoSyncTip = str.Mid(wxString(wxT("Autosync: ")).Len());
            autoSyncTip.Trim(false);
            autoSyncTip.Trim(true);
        }
        if (str.Contains(wxT("Server: "))) {
            autoSyncTip = str.Mid(wxString(wxT("Server: ")).Len());
            autoSyncTip.Trim(false);
            autoSyncTip.Trim(true);
        }
        if (str.Contains(wxT("project-code: "))) {
            projectCodeTip = str.Mid(wxString(wxT("project-code: ")).Len());
            projectCodeTip.Trim(false);
            projectCodeTip.Trim(true);
        }
        if (str.Contains(wxT("not authorized to write"))) {
            writeAuthorizationFailed = true;
        }
        if (str.Contains(wxT("would fork"))) {
            writeWouldFork = true;
        }
        if (str.Contains(wxT("login failed"))) {
            writeAuthorizationFailed = true;
        }

        bool needed = false;
        if (str.Contains(wxT("Error: "))) {
            needed = true;
        }

        if (showing||force||needed) {
            bool ok = true;
            if ((str[0]>='0'&&str[0]<='9')||str[0]<32) {
                ok = false;
            }
            if (force) ok = true;
            if (ok) {
                log_box->AppendText(str);
                log_box->AppendText(_T("\n"));
                Update();
                log_box->SetSelection(log_box->GetLastPosition(),-1);
                string s = conv(str);
                const char *at = s.c_str();
                while (at[0]=='\n'||at[0]==' ') at++;
                printf("%s\n", at);
            }
        } 
    }

    void addLogFile(const wxFileName& fn) {
        string sfn = conv(fn.GetFullPath());
        //printf("Adding log file %s\n", sfn.c_str());
        wxTextFile f;
        if (!f.Open(fn.GetFullPath())) return;
        for (wxString str = f.GetFirstLine(); 
             !f.Eof(); 
             str = f.GetNextLine()) {
            addLog(str,true);
        }
    }

    void CheckEnd() {
        if (CoopyApp::fossil_autoend) {
            if (CoopyApp::fossil_action!="") {
                wxCloseEvent ev;
                OnExit(ev);
            }
        }
    }

    void OnTerminate(wxProcess *process, int status) {
        //printf("OnTerminate! next is [%s]\n", next.c_str());
        processInput();
        timer->Stop();
        showing = true;
        bool fail = false;
        if (status!=0) {
            addLog(wxT("TROUBLE in CoopyTown..."));
            if (!writeWouldFork) {
                addLog(wxT("* Is repository link valid?"));
            }
            //addLog(wxString(wxT("  => ")) + conv(source));
            fail = true;
            if (retry!="") {
                next = "retryable";
            }
            if (next!="revertable"&&next!="retryable") {
                background = false;
                CoopyApp::fossil_result = 1;
                CheckEnd();
                return;
            }
        }
        //printf(">>>>> PROCESS COMPLETE: next is [%s]\n", next.c_str());
        string n = next;
        next = "";
        if (n=="view_sync") {
            updateSettings(true);
            updateListing();
            pushListing(true);
            CheckEnd();
            background = false;
        } else if (n=="view") {
            updateSettings(true);
            updateListing();
            CheckEnd();
        } else if (n == "view2") {
            wxString view = conv(path);
#ifndef WIN32
            view = wxT("file://") + view;
#endif
            ::wxLaunchDefaultBrowser(view);
        } else if (n=="sync") {
            wxCommandEvent ev;
            OnSync(ev);
        } else if (n=="push") {
            wxCommandEvent ev;
            OnPush(ev);
        } else if (n=="revertable"||n=="retryable") {
            if (fail) {
                if (n=="revertable") {
                    addLog(wxT("Reverting..."));
                    revert();
                    CheckEnd();
                }
                if (n=="retryable") {
                    addLog(wxT("Reverting..."));
                    revert();
                    string r = retry;
                    retry = "";
                    repush(r);
                }
            } else {
                retry = "";
                updatePivots(true);
                addLog(wxT("Online repository updated successfully."));
                CheckEnd();
            }
            background = false;
        }
    }

    void revert() {
        int argc = 2;
        char *argv[] = {
            fossil(),
            (char*)"revert",
            NULL };
        ssfossil(argc,argv,true);
        updatePivots(false);
    }

    list<string> getChanges() {
        beginLog();
        if (havePath()) {
            int argc = 2;
            char *argv[] = {
                fossil(),
                (char*)"changes",
                NULL };
            ssfossil(argc,argv,true);
        }
        return endLog();
    }

    list<string> getExtras() {
        beginLog();
        if (havePath()) {
            int argc = 2;
            char *argv[] = {
                fossil(),
                (char*)"extras",
                NULL };
            ssfossil(argc,argv,true);
        }
        return endLog();
    }

    list<string> getFiles() {
        beginLog();
        if (havePath()) {
            int argc = 2;
            char *argv[] = {
                fossil(),
                (char*)"ls",
                NULL };
            ssfossil(argc,argv,true);
        }
        fileCache = endLog();
        updateCache = list<string>();
        return fileCache;
    }

    list<string> getMissing(const list<string>& report) {
        list<string> missing;
        for (list<string>::const_iterator it = report.begin();
             it != report.end();
             it++) {
            if (it->find("MISSING")==0) {
                missing.push_back(it->c_str()+strlen("MISSING    "));
            }
        }
        return missing;
    }

    ostream *startStream() {
        //endStream();
        if (log_box) {
            log_box->Clear();
            //stream = new ostream(log_box);
        }
        //handler.setStream(stream);
        //return stream;
        return NULL;
    }

    void endStream() {
        /*
        if (stream) {
            delete stream;
            stream = NULL;
        }
        handler.setStream(stream);
        */
    }

    void doFiles(const list<string>& files, const char *act) {
        if (files.size()>=1) {
            if (havePath()) {
                int argc = files.size()+2;
                char **argv = new char *[argc];
                if (argv==NULL) {
                    fprintf(stderr,"Out of memory while adding files\n");
                    exit(1);
                }
                argv[0] = fossil();
                argv[1] = (char*)act;
                int i = 2;
                int items = 0;
                for (list<string>::const_iterator it = files.begin();
                     it != files.end();
                     it++) {
                    if (it->rfind(".csvs")!=string::npos) {
                        if (it->rfind(".mark")==string::npos) {
                            if (it->rfind(".pivot")==string::npos) {
                                if (it->rfind(".log")==string::npos) {
                                    argv[i] = (char*)(it->c_str());
                                    i++;
                                    items++;
                                }
                            }
                        }
                    }
                }
                argc = i;
                if (items>0) {
                    ssfossil(argc,argv,true);
                }
                delete[] argv;
            }
        }
    }

    void OnListBox(wxCommandEvent &event) {
        wxString str = list_box->GetStringSelection();
        if (str[0]!='.') {
            addLog(wxT("Selected '") + event.GetString() + wxT("' (double-click to open)"));
        } else {
            addLog(wxT("Double-click ADD option to attach a new spreadsheet/table."));
        }
    }

    void OnListBoxDoubleClick( wxCommandEvent &event ) {
        wxString str = list_box->GetStringSelection();
        if (str[0]!='.') {
            addLog(wxT("Opening '") + event.GetString() + wxT("' ..."));
            openFile(event.GetString());
        } else {
            addFile();
        }
    }

    bool updateListing();

    bool pushListing(bool reverse = false);

    bool updatePivots(bool success);

    bool updateSettings(bool create);

    bool openFile(const wxString& str, bool export_only = false);

    bool createFile(const char *local_name = NULL,
                    const char *key_name = NULL);

    bool addFile();

    bool Sync();

    void repush(const string& retry2);

    void OnFocusDbList() {
        list_box->SetFocus();
    }
enum
    {
        TEXT_Main = wxID_HIGHEST + 1,
        TEXT_Src,
        TEXT_Dest,
        TEXT_Dir,
        ID_LISTBOX,

        ID_Quit,
        ID_Commit,
        ID_Sync,
        ID_Undo,
        ID_About,
        ID_Tick,
        ID_Create,
        ID_Reset,
    };
};


class FossilProcess : public wxProcess
{
public:
    FossilProcess(CoopyFrame *parent, const wxString& cmd, bool sync)
        : wxProcess(parent), m_cmd(cmd), sync(sync)
    {
        m_parent = parent;
    }

    // instead of overriding this virtual function we might as well process the
    // event from it in the frame class - this might be more convenient in some
    // cases
    virtual void OnTerminate(int pid, int status);

protected:
    CoopyFrame *m_parent;
    wxString m_cmd;
    bool sync;
};



void FossilProcess::OnTerminate(int pid, int status)
{
    /*
    wxLogStatus(m_parent, wxT("Process %u ('%s') terminated with exit code %d."),
                pid, m_cmd.c_str(), status);
    */

    if (!sync) {
        m_parent->OnTerminate(this,status);
    }
}



bool CoopyApp::OnInit()
{
    CoopyFrame *frame = new CoopyFrame( _T("Coopy"), wxPoint(50,50), wxSize(450,340) );

    store_frame = frame;

    //g_hwnd = (long int)(frame->GetHandle());

    if (!wxApp::OnInit()) {
        return false;
    }

    if (!frame->OnInit()) {
        return false;
    }
    if (!silent) {
        frame->Show(TRUE);
        SetTopWindow(frame);
    } else {
        if (!frame->background) {
            //printf("Working in background...\n");
            //wxThread::Sleep(100);
            //}
            frame->Destroy();
            exit(fossil_result);
        }
    }
    return (fossil_result==0)?TRUE:FALSE;
};



IMPLEMENT_CLASS(CoopyFrame, wxFrame)

BEGIN_EVENT_TABLE(CoopyFrame, wxFrame)
    EVT_MENU(ID_Quit, CoopyFrame::OnQuit)
    EVT_MENU(ID_Sync, CoopyFrame::OnSync)
    EVT_MENU(ID_Commit, CoopyFrame::OnCommit)
    EVT_MENU(ID_Create, CoopyFrame::OnCreate)
    EVT_MENU(ID_About, CoopyFrame::OnAbout)
    EVT_BUTTON(ID_Reset, CoopyFrame::OnReset)
    EVT_BUTTON(ID_Quit, CoopyFrame::OnQuit)
    EVT_BUTTON(ID_Sync, CoopyFrame::OnSync)
    EVT_BUTTON(ID_Undo, CoopyFrame::OnUndo)
    EVT_BUTTON(ID_Commit, CoopyFrame::OnCommit)
    EVT_BUTTON(ID_Create, CoopyFrame::OnCreate)
    EVT_CLOSE(CoopyFrame::OnExit)
    EVT_TIMER(ID_Tick, CoopyFrame::OnProgressTimer)
    EVT_LISTBOX(ID_LISTBOX, CoopyFrame::OnListBox)
    EVT_LISTBOX_DCLICK(ID_LISTBOX, CoopyFrame::OnListBoxDoubleClick)
END_EVENT_TABLE()


int CoopyFrame::ssfossil(int argc, char *argv[], bool sync) {
    //printf("**** Calling fossil with %d arguments, %s\n", argc,
    //sync?"sync":"background");
    if (!sync) {
        background = true;
    }
    wxArrayString arr;
    wxChar *cmd[256];
    wxString op;
    wxString op1;
    for (int i=0; i<argc; i++) {
        //printf("[%s] ", argv[i]);
        wxString p = conv(safetxt(argv[i]));
        arr.Add(p);
        if (i>0) {
            op = op + p + wxT(" ");
        }
        if (i==1) {
            op1 = conv(string(argv[i]));
        }
    }
    if (verb()) {
        addLog(wxT("Command: [") + op + wxT("]"));
    }
    if (op1 == wxT("update")) {
        addLog(wxT(" \n \nUpdating repository..."));
    } else if (op1 == wxT("new")) {
        addLog(wxT(" \n \nCreating new repository..."));
    } else if (op1 == wxT("push")) {
        addLog(wxT(" \n \nPushing changes from your computer..."));
    } else if (op1 == wxT("changes")) {
        addLog(wxT(" \n \nChecking for changes on this computer..."));
    } else if (op1 == wxT("commit")) {
        addLog(wxT(" \n \nSending changes from your computer..."));
    }
    showing = (op1 == wxT("update")) || (op1 == wxT("commit")) || (op1 == wxT("push")) || (op1 == wxT("pull")) || (op1 == wxT("clone")) || (op1 == wxT("new"));
    for (int i=0; i<argc; i++) {
        //printf("[%s] ", conv(arr[i]).c_str());
        cmd[i] = (wxChar*)((const wxChar *)arr[i]);
    }
    //printf("\n");
    cmd[argc] = NULL;
    /*
    string cmd;
    for (int i=0; i<argc; i++) {
        printf("[%s] ", argv[i]);
        cmd += safetxt(argv[i]);
        cmd += " ";
    }
    */
    //printf("\n");
    //printf("Doing: %s\n", cmd.c_str());

    // Create the process string
    //wxEvtHandler *eventHandler = NULL;
    //wxProcess *proc = new wxProcess(eventHandler);
    FossilProcess *proc = new FossilProcess(this,_T("ssfossil"),sync);
    proc->Redirect();
    //if(::wxExecute(conv(cmd), wxEXEC_ASYNC, proc) == 0){
    if(::wxExecute(cmd, wxEXEC_ASYNC, proc) == 0){
        fprintf(stderr,"Fossil error (1)\n");
        exit(1);
    }
    reportErr = proc->GetErrorStream();
    report = proc->GetInputStream();
    if (report==NULL||reportErr==NULL) {
        fprintf(stderr,"Fossil error (2)\n");
        exit(1);
    }
    if (!sync) {
        timer->Start(50);
    } else {
        while (!report->Eof()) {
            processInput();
        }
        //printf("Sync done\n");
        showing = true;
    }
    
    return 0;
    
    /*
      try {
      return external_ssfossil_call(argc,argv);
      } catch (int err) {
      printf("exit(%d) called in external_ssfossil_call\n", err);
      }
      return -1;
    */
}

bool CoopyFrame::OnInit() {

    stream = NULL;
    askPath = true;
    askSource = true;
    askDestination = true;
    showing = true;

    wxPanel *panel = new wxPanel(this, wxID_ANY,
                                 wxDefaultPosition, wxSize(400,400));

    timer = new wxTimer(panel,ID_Tick);

    SetIcon(wxIcon((char**)appicon_xpm));

    topsizer = new wxBoxSizer( wxVERTICAL );

    wxSizerFlags tflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxLEFT|wxRIGHT|wxTOP, 10);
    wxSizerFlags flags = 
        wxSizerFlags(0).Align(wxALIGN_CENTER).Border(wxALL, 10);
    wxSizerFlags rflags = 
        wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10);
    wxSizerFlags lflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxALL, 10);

    
    wxBoxSizer *dir_bar = new wxBoxSizer( wxHORIZONTAL );

    /*
    dir_box = new wxDirPickerCtrl(panel,TEXT_Dir, wxT(""),
                                  wxT("Select a folder"),
                                  wxDefaultPosition,
                                  wxSize(300,-1));
    */
    const wxString choices[] = {
    };
    list_box = new wxListBox(panel,ID_LISTBOX, wxPoint(10,10), wxSize(200,100),
                             0, choices, wxLB_SINGLE | wxLB_ALWAYS_SB |
                             wxHSCROLL);

    //dir_box->SetTextCtrlProportion(0);

    if (CoopyApp::fossil_object=="") {
        if (CoopyApp::fossil_autoend) {
            wxString name = wxFileName::GetCwd();
            CoopyApp::fossil_object = conv(name);
        }
    }

    if (CoopyApp::fossil_object!="") {
        wxFileName name = wxFileName::FileName(conv(CoopyApp::fossil_object));
        name.MakeAbsolute();
        if (!wxFile::Exists(name.GetFullPath())) {
            path = conv(name.GetFullPath());
        } else {
            path = conv(name.GetPath());
        }
        //dir_box->SetPath(conv(path));
        dir_box_path = path;
        askPath = false;
    } else {
#ifdef __LINUX__
        //dir_box->SetPath(::wxGetCwd());
        dir_box_path = conv(::wxGetCwd());
#else
        wxStandardPaths sp;
        //dir_box->SetPath(sp.GetDocumentsDir());
        dir_box_path = conv(sp.GetDocumentsDir());
#endif
    }

    //dir_bar->Add(dir_box,lflags);
    dir_bar->Add(new wxButton( panel, ID_Sync, _T("Pull &in") ),
                    lflags);
    dir_bar->Add(new wxButton( panel, ID_Commit, _T("Push &out") ),
                    lflags);
    dir_bar->Add(new wxButton( panel, ID_Create, _T("&Set up repository") ),
                    lflags);
    topsizer->Add(dir_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));

    
    log_box = new wxTextCtrl(panel, TEXT_Main, conv(string("Coopy facilitates cooperative data-collection projects. \nSee http://") + SITE_NAME + " if you need a repository link.\nWarning: this is alpha software, keep backups of your data.\n"), 
                             wxDefaultPosition, wxSize(620,200),  
                             wxTE_MULTILINE | wxTE_RICH, 
                             wxDefaultValidator, wxTextCtrlNameStr);

    //handler.setCtrl(*log_box);

    topsizer->Add(log_box);
    topsizer->Add(new wxStaticText(panel,-1,_T("Spreadsheets/Tables"),
                                  wxDefaultPosition,
                                  wxSize(200,-1)),lflags);
    topsizer->Add(list_box);


    wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
    
    //create two buttons that are horizontally unstretchable, 
    // with an all-around border with a width of 10 and implicit top alignment
    button_sizer->Add(
                      new wxButton( panel, ID_Reset, _T("&Reset") ),
                      wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10));       
    
    button_sizer->Add(
                      new wxButton( panel, ID_Quit, _T("E&xit") ),
                      wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10));    



    topsizer->Add(button_sizer,wxSizerFlags(0).Align(wxALIGN_RIGHT));

    panel->SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    panel->SetFocus();

    if (!askPath) {
        updateSettings(true);
        updateListing();
    }

    if (CoopyApp::fossil_action!="") {
        if (CoopyApp::fossil_action=="push") {
            //printf("Should push\n");
            wxCommandEvent ev;
            OnCommit(ev);
        }
        if (CoopyApp::fossil_action=="new") {
            wxCommandEvent ev;
            OnCreateRepo(ev);
        }
        if (CoopyApp::fossil_action=="clone") {
            wxCommandEvent ev;
            OnCloneRepo(ev);
        }
        if (CoopyApp::fossil_action=="pull") {
            //printf("Should pull\n");
            wxCommandEvent ev;
            OnSync(ev);
        }
        if (CoopyApp::fossil_action=="add") {
            createFile(CoopyApp::fossil_message.c_str(),
                       CoopyApp::fossil_key.c_str());
            CheckEnd();
        }
        if (CoopyApp::fossil_action=="export") {
            openFile(conv(CoopyApp::fossil_key),true);
            CheckEnd();
        }
    }
    return true;
}


bool CoopyFrame::havePath() {
    //if (dir_box) 
    {
        string ref = dir_box_path; //conv(dir_box->GetPath());
        if (ref!=path) {
            path = ref;
        }
    }

    if (path=="" || askPath) {
        source = "";
        wxDirDialog dlg(NULL, wxT("Select directory to work in"), 
                        conv(path),
                        wxDD_DEFAULT_STYLE); // | wxDD_DIR_MUST_EXIST);
        while (path==""||askPath) {
            if (dlg.ShowModal()!=wxID_OK) {
                return false;
            } else {
                wxString result = dlg.GetPath();
                
                wxDir dir(result);
                int ct = 0;
                if (dir.IsOpened()) {
                    wxString filename;
                    bool cont = dir.GetFirst(&filename, wxEmptyString, 
                                             wxDIR_FILES|wxDIR_DIRS|wxDIR_HIDDEN);
                    while ( cont ) {
                        string fname = conv(filename);
                        //printf("%s\n", fname.c_str());
                        cont = dir.GetNext(&filename);
                        ct++;
                    }
                }
                if (ct>0) {
                    wxChar sep = wxFileName::GetPathSeparator();
                    wxString target = result + sep + wxT("repository.coopy");
                    if (!wxFileExists(target)) {
                        wxString target2 = result + sep + wxT("clone.fossil");
                        if (wxFileExists(target2)) {
                            target = target2;
                        }
                    }
 
                    if (!wxFileExists(target)) {
                        wxMessageDialog msg(NULL, wxT("Please select a fresh new directory to start a new repository, or a directory that already has a Coopy repository in it."),
                                            wxT("Sorry, cannot use selected directory"));
                        msg.ShowModal();
                        continue;
                    }
                }

                path = conv(result);
                //printf("Selected a directory %s\n", path.c_str());
                askPath = false;

                /*
                if (path!="") {
                    updateSettings(true);
                }
                */
            }
        }
    }
    if (path!="") {
        wxSetWorkingDirectory(conv(path));
    }

    //if (dir_box) 
    {
        string ref = dir_box_path; //conv(dir_box->GetPath());
        if (ref!=path && path!="") {
            //dir_box->SetPath(conv(path));
            dir_box_path = path;
        }
    }

    return path!="";
}

bool CoopyFrame::updateSettings(bool create) {
    wxChar sep = wxFileName::GetPathSeparator();
    wxString target = conv(path);
    if (target.length()>0) {
        target = target + sep;
    }
    target = target + wxT("local_settings.coopy");
    ws.connect(create,conv(target).c_str());
    string sep0 = conv(sep);
    ws.setDirectory(path.c_str(),sep0.c_str());
    //if (ws.isValid()) {
    //updateListing();
    //}
}


bool CoopyFrame::updateListing() {
    //printf("update listing\n");
    list<string> files = getFiles();
    map<string,int> present;
    string adder = "... Add ...";
    present[adder] = 1;
    for (list<string>::const_iterator it = files.begin();
         it != files.end();
         it++) {
        if (it->rfind(".csvs")!=string::npos) {
            string str = it->substr(0,it->rfind(".csvs"));
            //printf("file of interest %s -> %s\n", it->c_str(), str.c_str());
            wxString item = conv(str);
            int result = list_box->FindString(item);
            if (result==wxNOT_FOUND) {
                list_box->InsertItems(1,&item,0);
            }
            present[str] = 1;
        }
    }
    {
        wxString item = conv(adder);
        int result = list_box->FindString(item);
        if (result==wxNOT_FOUND) {
            list_box->InsertItems(1,&item,0);
        }
    }

    int ct = list_box->GetCount();
    int offset = 0;
    for (int i=0; i<ct; i++) {
        wxString item = list_box->GetString(i-offset);
        string str = conv(item);
        if (present.find(str)==present.end()) {
            //printf("cannot find %s\n", str.c_str());
            list_box->Delete(i-offset);
            offset++;
        }
    }
    return true;
}


bool CoopyFrame::pushListing(bool reverse) {
    //printf("push listing %d\n", reverse);
    list<string> files = fileCache;
    for (list<string>::const_iterator it = files.begin();
         it != files.end();
         it++) {
        //printf("checking %s\n", it->c_str());
        if (it->rfind(".csvs")!=string::npos) {
            string str = it->substr(0,it->rfind(".csvs"));
            //printf("checking %s -> %s\n", it->c_str(), str.c_str());
            string local = ws.getFile(str.c_str());
            if (local=="") continue;
            string remote = *it;

            wxFileName localName = wxFileName::FileName(conv(local));
            wxFileName remoteName = wxFileName::FileName(conv(remote));
            if (!localName.FileExists()) continue;
            if (!remoteName.FileExists()) continue;
            //printf("local %s remote %s %d\n", local.c_str(),
            //     remote.c_str(), reverse);
            wxFileName pivotName = wxFileName::FileName(conv(remote+".pivot"));
            wxFileName markName = wxFileName::FileName(conv(remote+".mark"));
            wxFileName logName = wxFileName::FileName(conv(remote+".log"));
            wxDateTime localTime = localName.GetModificationTime();
            wxDateTime remoteTime = remoteName.GetModificationTime();
            wxDateTime markTime;
            bool haveMark = false;
            if (markName.FileExists()) {
                markTime = markName.GetModificationTime();
                haveMark = true;
            }
            bool act = false;
            if (!reverse) {
                // local -> repository

                if (haveMark) {
                    act = markTime.IsEarlierThan(localTime);
                } else {
                    act = remoteTime.IsEarlierThan(localTime);
                }
                if (act) {
                    //printf("  local -> repository (%s, %s)\n", local.c_str(), remote.c_str());
                    bool ok = ws.importSheet(str.c_str());
                    if (ok) {
                        updateCache.push_back(str);
                        /*
                        if (!markName.FileExists()) {
                            wxFile f;
                            f.Create(markName.GetFullPath());
                        }
                        markName.Touch();
                        */
                    }
                }

            } else {
                // repository -> local

                if (haveMark) {
                    act = markTime.IsEarlierThan(remoteTime);
                } else {
                    act = localTime.IsEarlierThan(remoteTime);
                }
                if (act) {
                    //printf("  repository -> local (%s, %s)\n", remote.c_str(), local.c_str());
                    string l = conv(localName.GetFullPath());
                    string r = conv(remoteName.GetFullPath());
                    string p = conv(pivotName.GetFullPath());
                    string lg = conv(logName.GetFullPath());
                    bool pExists = pivotName.FileExists();
                    bool ok = false;
                    if (localName.FileExists()) {
                        ok = ws.mergeToLocal(l.c_str(),r.c_str(),
                                             pExists?p.c_str():NULL,
                                             lg.c_str());
                    } else {
                        ok = ws.exportSheet(str.c_str());
                    }
                    if (ok) {
                        wxCopyFile(remoteName.GetFullPath(),
                                   pivotName.GetFullPath(),
                                   true);
                        /*
                        if (!markName.FileExists()) {
                            wxFile f;
                            f.Create(markName.GetFullPath());
                        }
                        markName.Touch();
                        */
                        addLogFile(logName);
                    } else {
                        addLog(wxT("Could not merge with online version, there is a conflicting change"));
                        addLog(wxT("Not much help available for this condition yet, sorry, bug Paul about this"));
                        localName.MakeAbsolute();
                        addLog(wxT("Your file: ") + localName.GetFullPath());
                        remoteName.MakeAbsolute();
                        addLog(wxT("Online version: ") + remoteName.GetFullPath());
                        if (pExists) {
                            pivotName.MakeAbsolute();
                            addLog(wxT("Common ancestor: ") + pivotName.GetFullPath());
                        }
                    }
                }
            }
        }
    }

    return true;
}


bool CoopyFrame::updatePivots(bool success) {
    list<string> files = updateCache;
    for (list<string>::const_iterator it = files.begin();
         it != files.end();
         it++) {
        string str = (*it);

        /*
        printf("update pivot for %s: %s\n", 
               success?"success":"failure",
               str.c_str());
        */

        string remote = str + ".csvs";
        wxCopyFile(conv(remote),conv(remote+".pivot"),true);

        if (success) {
            wxFileName markName(conv(remote+".mark"));
            if (!markName.FileExists()) {
                wxFile f;
                f.Create(markName.GetFullPath());
            }
            markName.Touch();
        }
    }
    return true;
}


bool CoopyFrame::createFile(const char *local_name, const char *key_name) {
    wxString meat = wxT("example_name");
    wxRegEx re(wxT("[^a-zA-Z0-9]"));
    if (local_name!=NULL) {
        std::string ll(local_name);
        if (key_name!=NULL) {
            ll = key_name;
        }
        wxString n(conv(ll));
        wxFileName f = wxFileName::FileName(n);
        meat = f.GetName();
        re.ReplaceAll(&meat,wxT("_"));
    }
    if (CoopyApp::fossil_action=="") {
        wxTextEntryDialog dlg(NULL, 
                              wxT("Enter a simple name for the file in the repository.\nAny spaces or punctuation will be replaced by '_' characters.\nYou'll be able to save with a different name on your computer."),
                              wxT("Set name"),
                              meat);
        if (dlg.ShowModal()!=wxID_OK) {
            return false;
        }
        meat = dlg.GetValue();
        re.ReplaceAll(&meat,wxT("_"));
    }
    wxString actName = meat + wxT(".csvs");
    string key = conv(meat);
    wxFileName name = wxFileName::FileName(actName);
    if (!name.FileExists()) {
        if (local_name==NULL) {
            wxFile f;
            f.Create(name.GetFullPath());
            f.Write(wxT("== Main ==\n"));
            f.Write(wxT("Name,Number\n"));
            f.Write(wxT("-----------\n"));
            f.Write(wxT("One,1\n"));
            f.Write(wxT("Two,2\n"));
            f.Write(wxT("Three,3\n"));
            f.Write(wxT("Four,4\n"));
            f.Write(wxT("Five,5\n"));
            f.Close();
        } else {
            wxFile f;
            f.Create(name.GetFullPath());
            f.Close();
            ws.setFile(key.c_str(),local_name);
            ws.exportSheet(key.c_str(),true);
        }
    }
    list<string> files;
    files.push_back(conv(actName));
    doFiles(files,"add");
    updateListing();
    CheckEnd();
    return true;
}

bool CoopyFrame::addFile() {
    wxFileDialog LoadDialog(this, _("Add File"), wxEmptyString, wxEmptyString,
                            _("Excel files (*.xls)|*.xls|Sqlite files (*.sqlite)|*.sqlite|CSV files (*.csv)|*.csv"),
                            wxFD_OPEN | wxFD_FILE_MUST_EXIST, wxDefaultPosition);
 
    if (LoadDialog.ShowModal() == wxID_OK) {
        wxFileName name = wxFileName::FileName(LoadDialog.GetPath());
        name.MakeAbsolute();
        string fname = conv(name.GetFullPath());
        return createFile(fname.c_str());
    }

    return false;
}


bool CoopyFrame::openFile(const wxString& str, bool export_only) {
    string key = conv(str);
    string fname = ws.getFile(key.c_str());
    bool exists = false;
    if (fname!="") {
        wxFileName name = wxFileName::FileName(conv(fname));
        exists = name.FileExists();
    }
    if (fname==""||!exists) {
        if (CoopyApp::fossil_message!="") {
            fname = CoopyApp::fossil_message;
        } else {
            wxFileDialog SaveDialog(this, _("Save File As _?"), wxEmptyString, conv(fname),
                                    _("Excel files (*.xls)|*.xls|Sqlite files (*.sqlite)|*.sqlite|CSV files (*.csv)|*.csv"),
                                    wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
            
            if (SaveDialog.ShowModal() == wxID_OK) {
                fname = conv(SaveDialog.GetFilename());
            } else {
                return false;
            }
        }
        if (fname!="") {
            ws.setFile(key.c_str(),fname.c_str());
            ws.exportSheet(key.c_str());
            updateCache.push_back(key);
            updatePivots(true);
        } else {
            return false;
        }
    }
    if (fname=="") return false;
    addLog(wxT("... local file is ") + conv(fname));

    wxFileName name = wxFileName::FileName(conv(fname));
    if (!name.FileExists()) {
        ws.exportSheet(key.c_str());
    }
    name.MakeAbsolute();
    fname = conv(name.GetFullPath());

    if (!export_only) {
        show(fname);
    }

    return true;
}


bool CoopyFrame::haveSource() {
    if (src_box) {
        string ref = conv(src_box->GetValue());
        if (ref!=source) {
            source = ref;
        }
    }
    if (CoopyApp::fossil_repo.length()>0) {
        source = CoopyApp::fossil_repo;
        askSource = false;
    }

    if (source==""||askSource) {
        string suggest = source;
        if (suggest=="") {
            suggest = ""; 
            //suggest = "http://coopy.sourceforge.net/cgi-bin/wiki/home";
        }
        if (CoopyApp::silent) {
            addLog(wxT("No repository found"));
            CoopyApp::fossil_result = 1;
            CheckEnd();
            return false;
        }
        wxTextEntryDialog dlg(NULL, 
                              wxT("Enter the link for the repository to pull in to your computer.\nIf you do not have a link, try creating a repository first."),
                              wxT("Enter repository link"),
                              conv(suggest));
        if (dlg.ShowModal()==wxID_OK) {
            source = conv(dlg.GetValue());
            /*
string("http://") +
                FOSSIL_USERNAME + ":" + conv(dlg.GetValue()) + "@" +
                FOSSIL_ROOT + FOSSIL_REPO;
            */
            //printf("Source set to %s\n", source.c_str());
            askSource = false;
        } else {
            source = "";
        }
    }

    if (src_box) {
        string ref = conv(src_box->GetValue());
        if (ref!=source && source!="") {
            src_box->ChangeValue(conv(source));
        }
    }

    //printf("Source is %s\n", source.c_str());

    return source!="";
}

bool CoopyFrame::haveDestination() {
    if (dest_box) {
        string ref = conv(dest_box->GetValue());
        if (ref!=destination) {
            destination = ref;
        }
    }

    if (destination==""||askDestination) {
        string suggest = destination;
        if (suggest=="") {
            suggest = source;
        }
        wxTextEntryDialog dlg(NULL, wxT("Enter destination URL"),
                              wxT("Enter destination URL"),
                              conv(suggest));
        if (dlg.ShowModal()==wxID_OK) {
            destination = conv(dlg.GetValue());
            askDestination = false;
        } else {
            destination = "";
        }
        /*
        wxPasswordEntryDialog dlg(NULL, wxT("Enter password"));
        if (dlg.ShowModal()==wxID_OK) {
            source = string("http://") +
                FOSSIL_USERNAME + ":" + conv(dlg.GetValue()) + "@" +
                FOSSIL_ROOT + FOSSIL_REPO;
            printf("Source set to %s\n", source.c_str());
        }
        */
    }

    if (dest_box) {
        string ref = conv(dest_box->GetValue());
        if (ref!=destination && destination!="") {
            dest_box->ChangeValue(conv(destination));
        }
    }

    return destination!="";
}

void CoopyFrame::OnReset(wxCommandEvent& ev) {
    askPath = true;
    askSource = true;
    askDestination = true;
    if (src_box) { src_box->ChangeValue(wxT("")); }
    if (dest_box) { dest_box->ChangeValue(wxT("")); }
}

bool CoopyFrame::OnCloneRepo(wxCommandEvent& event) {
    next = "";
    retry = "";
    string src = CoopyApp::fossil_target;
    wxURL url = conv(src);
    if (!url.HasScheme()) {
        wxChar sep = wxFileName::GetPathSeparator();
        wxFileName name = wxFileName::FileName(conv(src));
        if (!wxFile::Exists(name.GetFullPath())) {
            src = src + conv(sep) + "repository.coopy";
        }
    }
    if (havePath()) {
        wxChar sep = wxFileName::GetPathSeparator();
        wxString target = conv(path) + sep + wxT("repository.coopy");
        if (!wxFileExists(target)) {
            int argc = 4;
            string t = conv(target);
            char *argv[] = {
                fossil(),
                (char*)"clone",
                (char*)src.c_str(),
                (char*)t.c_str(),
                NULL };
            ssfossil(argc,argv,true);
        }
        if (!wxFileExists(target)) {
            exit(1);
        }
        return OnOpenRepo(false);
    }
    return false;
}

bool CoopyFrame::OnCreateRepo(wxCommandEvent& event) {
    next = "";
    retry = "";
    if (havePath()) {
        wxChar sep = wxFileName::GetPathSeparator();
        wxString target = conv(path) + sep + wxT("repository.coopy");
        if (!wxFileExists(target)) {
            int argc = 3;
            string t = conv(target);
            char *argv[] = {
                fossil(),
                (char*)"new",
                (char*)t.c_str(),
                NULL };
            ssfossil(argc,argv,true);
        }
        if (!wxFileExists(target)) {
            exit(1);
        }
        /*
        if (wxFileExists(target)) {
            Sync();
        }
        */
        next = "";
        return OnOpenRepo(false);
    }
    return false;
}


bool CoopyFrame::OnOpenRepo(bool back) {
    if (havePath()) {
        //printf("Should open %s\n", path.c_str());
        wxChar sep = wxFileName::GetPathSeparator();
        wxString target = conv(path) + sep + wxT("repository.coopy");
        string ctarget = conv(target);
        if (wxFileExists(target)) {
            wxString view_target = conv(path) + sep + wxT("_FOSSIL_");
            if (!wxFileExists(view_target)) {
                //printf("No view yet %s\n", conv(view_target).c_str());
                int argc = 3;
                char *argv[] = {
                    fossil(),
                    (char*)"open",
                    (char*)ctarget.c_str(),
                    NULL };
                if (back) {
                    next = "sync";
                }
                ssfossil(argc,argv,true);
                //if (back) {
                //return true;
                //}
            }
            if (wxFileExists(view_target)) {
                // make sure we have autosync
                int argc = 4;
                char *argv[] = {
                    fossil(),
                    (char*)"setting",
                    (char*)"autosync",
                    (char*)"1",
                    NULL };
                ssfossil(argc,argv,true);
            }
            if (wxFileExists(view_target)) {
                //printf("Simple sync\n");
                int argc = 2;
                char *argv[] = {
                    fossil(),
                    (char*)"update",
                    NULL };
                if (back) {
                    next = "view_sync";
                }
                ssfossil(argc,argv,!back);
            }
        }
        if (!back) {
            updateSettings(true);
            updateListing();
            pushListing(true);
        }
    }
}

bool CoopyFrame::Sync() {
    //printf("Syncing...\n");
    retry = "clone";
    next = "retryable";
    if (havePath()) {
        //printf("Should pull %s\n", path.c_str());
        wxChar sep = wxFileName::GetPathSeparator();
        wxString target = conv(path) + sep + wxT("repository.coopy");
        string ctarget = conv(target);
        if (!wxFileExists(target)) {
            //printf("Could not find %s\n", ctarget.c_str());
            if (haveSource()) {
                //printf("Need to clone %s\n", ctarget.c_str());
                int argc = 4;
                char *argv[] = {
                    fossil(),
                    (char*)"clone",
                    (char*)source.c_str(),
                    (char*)ctarget.c_str(),
                    NULL };
                next = "sync";
                ssfossil(argc,argv);
                return true;
            } else {
                return false;
            }
        }
        next = "sync";
        return OnOpenRepo(true);
    }
    return false;
}

void CoopyFrame::OnSync(wxCommandEvent& event) {
    retry = "pull";
    Sync();
}


void CoopyFrame::OnUndo(wxCommandEvent& event) {
    //printf("Should undo\n");
    startStream();
    if (havePath()) {
        int argc = 2;
        char *argv[] = {
            fossil(),
            (char*)"undo",
            NULL };
        ssfossil(argc,argv);
    }
    endStream();
}

void CoopyFrame::OnCreate(wxCommandEvent& event) {
    //printf("Create!\n");
    //::wxLaunchDefaultBrowser(wxT(SITE_NAME_CREATE));
    retry = "";
    repush("");
}


void CoopyFrame::repush(const string& retry2) {
    if (writeWouldFork) {
        wxMessageDialog dlg(NULL, wxT("There have been updates to the repository.\nPlease pull those updates in before pushing yours out."), wxT("Cannot push"), 
                            wxOK|wxICON_INFORMATION);
        dlg.ShowModal();
        CheckEnd();
        return;
    }
    CheckEnd();

    bool has_dir = (dir_box_path!="" && !askPath);

    wxString choices[] = { 
        wxT("Connect to an existing repository"), 
        wxT("Set username and password"), 
        wxT("Create empty new repository"), 
        wxT("Fork current repository"), 
        wxT("Change directory"),
        wxT("Show project code"),
    };

    int CHOICE_CONNECT=0;
    int CHOICE_USERNAME=1;
    int CHOICE_NEW=2;
    int CHOICE_FORK=3;
    int CHOICE_DIR=4;
    int CHOICE_CODE=5;

    wxSingleChoiceDialog dlg(this, (retry2!="")?wxT("Access denied.  What would you like to do?"):wxT("What would you like to do?"),
                             (retry2!="")?wxT("Access denied"):wxT("Set up repository"), has_dir?6:4, choices);
    
    if (dlg.ShowModal() != wxID_OK) {
        return;
    }
 
    {
        int argc = 2;
        char *argv[] = {
            fossil(),
            (char*)"info",
            NULL };
        ssfossil(argc,argv,true);
    }

    int choice = dlg.GetSelection();

    if (source=="") {
        source = conv(autoSyncTip);
    }
    wxURL url = conv(source);
    //printf("source is %s\n", source.c_str());

    if (choice==CHOICE_DIR) {
        askPath = true;
        havePath();
        return;
    }

    if (choice==CHOICE_CODE||choice==CHOICE_FORK) {
        if (projectCodeTip.IsEmpty()) {
            wxMessageDialog dlg(NULL, wxT("Please connect to an existing repository first"), wxT("No repository"), 
                                wxOK|wxICON_ERROR);
            dlg.ShowModal();
            return;
        }
    }

    if (choice==CHOICE_CODE) {
        wxTextEntryDialog dlg(NULL, wxT("Here is the project code.\nThis may be needed when forking a repository."), wxT("Project code"), projectCodeTip);
        dlg.ShowModal();
        return;
    }

    wxString target = wxT("");
    if (choice==CHOICE_NEW||choice==CHOICE_FORK) {
        wxString choices[] = { 
            wxT("chiselapp.com"), 
            wxT("share.find.coop"), 
            wxT("I've already made the new repository"), 
        };

        wxString full_choices[] = {
            wxT("http://chiselapp.com"),
            wxT("http://share.find.coop/repo/new/"),
        };
        
        wxSingleChoiceDialog 
            dlg(this, wxT("Please select a host for your new repository."),
                wxT("Select host"), (choice==CHOICE_FORK)?3:2, choices);

        if (dlg.ShowModal() != wxID_OK) {
            return;
        }

        int choice2 = dlg.GetSelection();
        if (choice2<2) {
            target = full_choices[choice2];
            ::wxLaunchDefaultBrowser(target);
        }
        if (choice!=CHOICE_FORK) {
            return;
        }
    }

    if (choice==CHOICE_FORK) {
        wxTextEntryDialog dlg4(NULL, wxT("If asked, here is the project code:"), wxT("Project code"), projectCodeTip);
        if (dlg4.ShowModal()!=wxID_OK) return;

        wxTextEntryDialog dlg3(NULL, wxT("Repository link"), wxT("Enter repository"), conv(source));
        if (dlg3.ShowModal()!=wxID_OK) return;
        url = dlg3.GetValue();

    }
    if (choice==CHOICE_CONNECT) {
        /*
        if (has_dir) {
        } else {
        */
        Sync();
        return;
    }

    wxString user = url.GetUser();
    wxTextEntryDialog dlg1(NULL, wxT("Username needed to access the repository"), wxT("Enter username"), user);
    if (dlg1.ShowModal()!=wxID_OK) return;
    wxPasswordEntryDialog dlg2(NULL, wxT("Password needed to access the repository"), wxT("Enter password"), url.GetPassword());
    if (dlg2.ShowModal()!=wxID_OK) return;

    wxString username = dlg1.GetValue();
    wxString pword = dlg2.GetValue();
    wxString protocol = url.GetScheme();
    if (protocol.IsEmpty()) {
        protocol = wxT("http");
    }
    wxString current = url.BuildURI();
    int firstAt = current.Find('@');
    if (firstAt!=wxNOT_FOUND) {
        current = current.Mid(firstAt+1);
    }
    firstAt = current.Find(wxT("://"));
    if (firstAt!=wxNOT_FOUND) {
        current = current.Mid(firstAt+3);
    }
    if (!username.IsEmpty()) {
        current = protocol + wxT("://") + username + wxT(":") + pword + 
            wxT("@") + current;
    } else {
        current = protocol + wxT("://") + current;
    }
    source = conv(current);
    //printf("Source set to %s\n", source.c_str());

    /*
        source = string("http://") +
            FOSSIL_USERNAME + ":" + conv(dlg.GetValue()) + "@" +
            FOSSIL_ROOT + FOSSIL_REPO;
        printf("Source set to %s\n", source.c_str());
    */

    {
        next = "retryable";
        retry = retry2;
        if (retry2!="clone") {
            int argc = 3;
            char *argv[] = {
                fossil(),
                (char*)retry2.c_str(),
                (char*)source.c_str(),
                NULL };
            ssfossil(argc,argv);
        } else {
            Sync();
        }
    }
}

void CoopyFrame::OnPush(wxCommandEvent& event) {
    int argc = 3;
    char *argv[] = {
        fossil(),
        (char*)"push",
        (char*)destination.c_str(),
        NULL };
    ssfossil(argc,argv);
}

void CoopyFrame::OnCommit(wxCommandEvent& event) {
    retry = "push";
    writeWouldFork = false;
    writeAuthorizationFailed = false;
    //printf("Should commit\n");
    //startStream();
    if (havePath()) {
        updateListing();
        pushListing();
        //if (haveDestination()) {
            doFiles(getExtras(),"add");
            list<string> changes = getChanges();
            list<string> missing = getMissing(changes);
            if (missing.size()>0) {
                string msg = "";
                for (list<string>::const_iterator it = missing.begin();
                     it != missing.end();
                     it++) {
                    msg += it->c_str();
                    msg += " is missing";
                    msg += "\n";
                }
                msg += "Should this/these be removed in the repository?";
                //printf("Message is %s\n", msg.c_str());
                wxMessageDialog dlg(NULL, conv(msg), wxT(""), 
                                    wxYES_NO|wxCANCEL);
                if (dlg.ShowModal()!=wxID_YES) {
                    endStream();
                    return;
                }
                doFiles(missing,"rm");
                changes = getChanges();
            }
            string msg = "";
            for (list<string>::const_iterator it = changes.begin();
                 it != changes.end();
                 it++) {
                msg += it->c_str();
                msg += "\n";
            }
            if (msg!="") {
                msg += "Enter brief description of changes";
                //printf("Message is %s\n", msg.c_str());
                wxString message = conv(CoopyApp::fossil_message);
                bool ok = true;
                if (message == wxT("")) {
                    wxTextEntryDialog dlg(NULL, conv(msg));
                    ok = (dlg.ShowModal()==wxID_OK);
                    message = dlg.GetValue();
                } else {
                    CoopyApp::fossil_message = "";
                }
                if (ok) {
                    commit_message = conv(message);
                    if (commit_message == "") {
                        commit_message = "[from coopy]";
                    }
                    int argc = 4;
                    // commit currently fails if called twice, unless username
                    // specified (need to remove some global state from fossil)
                    char *argv[] = {
                        fossil(),
                        (char*)"commit",
                        //(char*)"--user",
                        //(char*)FOSSIL_USERNAME,
                        (char*)"-m",
                        (char*)commit_message.c_str(),
                        NULL };
                    next = "revertable";
                    ssfossil(argc,argv);
                    return;
                } else {
                    return;
                }
            } else {
                int argc = 2;
                // commit currently fails if called twice, unless username
                // specified (need to remove some global state from fossil)
                char *argv[] = {
                    fossil(),
                    (char*)"push",
                    NULL };
                next = "revertable";
                ssfossil(argc,argv);
                //msg = "No changes!";
                //addLog(wxT("No changes to push"));
                //CheckEnd();
            }
            //}
    }
    //endStream();
    //OnPush(event);
}

CoopyFrame::CoopyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    //m_textCtrl = NULL;
    background = false;

    logging = false;
    path = "";
    source = "";
    commit_message = "";

    writeWouldFork = false;
    writeAuthorizationFailed = false;

    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_Sync, _T("Pull &in...") );
    menuFile->Append( ID_Commit, _T("Push &out...") );
    menuFile->Append( ID_Create, _T("Set up &repository...") );
    menuFile->Append( ID_About, _T("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("E&xit") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _T("&File") );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _T("Welcome to Coopy!") );
}

void CoopyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    //wxCloseEvent ev;
    //wxPostEvent(this,ev);
    Close(TRUE);
}

void CoopyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Welcome to coopy!\n\
Now go back to work."),
                 _T("About coopy"), wxOK | wxICON_INFORMATION, this);
}

void CoopyApp::OnKey(wxKeyEvent& e) {
    if(e.GetModifiers() == wxMOD_ALT) {
        switch(e.GetKeyCode()) {
        case 'T':
            ((CoopyFrame *)store_frame)->OnFocusDbList();
            break;
        default:
            e.Skip();
            break;
        }
    } else {
        e.Skip();
    }
}


#ifdef _WIN32

FILE *FOUT = NULL;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR m_lpCmdLine, int nCmdShow) {

    //g_hinstance = (int)(hInstance);    
    return wxEntry(hInstance,hPrevInstance,m_lpCmdLine,nCmdShow);
}

#endif

