// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

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
#include <wx/textctrl.h>
#include <wx/url.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/listctrl.h>

#include <string>
#include <list>
#include <map>
#include <iostream>

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
private:
    bool silent;
public:
    CoopyApp() {
        silent = false;
    }

    virtual bool OnInit();

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);

    static string fossil_object;
};

string CoopyApp::fossil_object;

static const wxCmdLineEntryDesc g_cmdLineDesc [] = {
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("displays help on the command line parameters"),
      wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, wxT("s"), wxT("silent"), wxT("disables the GUI") },
    { wxCMD_LINE_OPTION, wxT("r"), wxT("res"), wxT("set resource location"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_PARAM, NULL, NULL, wxT("input file"), wxCMD_LINE_VAL_STRING,
      wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_NONE },
};


void CoopyApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("--"));
}
 
bool CoopyApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    silent = parser.Found(wxT("s"));

    wxString location;
    if (parser.Found(wxT("r"),&location)) {
        string loc = conv(location);
        printf("*** should set location to [%s]\n", loc.c_str());
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


/*
class UiFossilHandler : public FossilHandler {
private:
    bool logging;
    list<string> results;
    ostream *stream;
    wxWindow *owner;
    wxTextCtrl *ctrl;
public:
    UiFossilHandler() {
        logging = false;
        stream = NULL;
        owner = NULL;
        ctrl = NULL;
    }

    void setStream(ostream *stream) {
        this->stream = stream;
    }

    void setOwner(wxWindow& owner) {
        this->owner = &owner;
    }

    void setCtrl(wxTextCtrl& ctrl) {
        this->ctrl = &ctrl;
    }

    virtual int exit(int result) {
        throw result;
    }


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


    void replace(std::string& str, const std::string& old, 
                 const std::string& rep) {
        size_t pos = 0;
        while((pos = str.find(old, pos)) != std::string::npos) {
            str.replace(pos, old.length(), rep);
            pos += rep.length();
        }
    }

    virtual int printf(const char *txt) {
        ::printf(">>> %s", txt);
        if (logging) {
            string cp(txt);
            split(cp,"\n",results);
        } else {
            if (stream) {
                string str(txt);
                replace(str,string("\r"),string(" * "));
                replace(str,"Received:","\nReceived:");
                (*stream) << str;
                (*stream).flush();
                if (owner!=NULL) {
                    owner->Update();
                    ctrl->SetSelection(ctrl->GetLastPosition(),-1);
                }
            }
        }
        return 0;
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
};
*/


class CoopyFrame: public wxFrame
{
    DECLARE_CLASS(CoopyFrame)
    DECLARE_EVENT_TABLE()

private:
    wxBoxSizer *topsizer;
    //wxTextCtrl* m_textCtrl;

    wxTextCtrl *log_box;
    wxTextCtrl *src_box;
    wxTextCtrl *dest_box;
    wxDirPickerCtrl *dir_box;
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
    bool logging;
    bool showing;
    list<string> results;
    list<string> fileCache;

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
        printf("Exiting Coopy\n");
    }

    void OnOK(wxCommandEvent& event);

    virtual bool OnInit();

    void OnSync(wxCommandEvent& event);
    void OnPush(wxCommandEvent& event);
    void OnCommit(wxCommandEvent& event);
    void OnCreate(wxCommandEvent& event);
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
        printf("Tick!\n");
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

    void addLog(const wxString& str) {
        if (showing) {
            bool ok = true;
            if ((str[0]>='0'&&str[0]<='9')||str[0]<32) {
                ok = false;
            }
            if (ok) {
                log_box->AppendText(str);
                log_box->AppendText(_T("\n"));
                Update();
                log_box->SetSelection(log_box->GetLastPosition(),-1);
            }
        } 
        string s = conv(str);
        printf("   // %s\n", s.c_str());
    }

    void OnTerminate(wxProcess *process, int status) {
        processInput();
        timer->Stop();
        showing = true;
        if (status!=0) {
            printf(">>>>> PROCESS FAILED: next ignored [%s]\n", next.c_str());
            addLog(_T("\nSomething went wrong..."));
            return;
        }
        printf(">>>>> PROCESS COMPLETE: next is [%s]\n", next.c_str());
        string n = next;
        next = "";
        if (n=="view_sync") {
            updateSettings(true);
            updateListing();
            pushListing(true);
        } else if (n=="view") {
            updateSettings(true);
            updateListing();
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
        }
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
                    printf("Out of memory while adding files\n");
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
                            argv[i] = (char*)(it->c_str());
                            i++;
                            items++;
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
        }
    }

    void OnListBoxDoubleClick( wxCommandEvent &event ) {
        wxString str = list_box->GetStringSelection();
        if (str[0]!='.') {
            addLog(wxT("Opening '") + event.GetString() + wxT("' ..."));
            openFile(event.GetString());
        } else {
            createFile();
        }
    }

    bool updateListing();

    bool pushListing(bool reverse = false);

    bool updateSettings(bool create);

    bool openFile(const wxString& str);

    bool createFile();

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

    //g_hwnd = (long int)(frame->GetHandle());

    if (!wxApp::OnInit()) {
        return false;
    }

    if (!silent) {
        if (!frame->OnInit()) {
            return false;
        }
        frame->Show(TRUE);
        SetTopWindow(frame);
        return TRUE;
    } else {
        return false;
    }
};



IMPLEMENT_CLASS(CoopyFrame, wxFrame)

BEGIN_EVENT_TABLE(CoopyFrame, wxFrame)
    EVT_MENU(ID_Quit, CoopyFrame::OnQuit)
    EVT_MENU(ID_Sync, CoopyFrame::OnSync)
    EVT_MENU(ID_Commit, CoopyFrame::OnCommit)
    EVT_MENU(ID_Create, CoopyFrame::OnCreate)
    EVT_MENU(ID_About, CoopyFrame::OnAbout)
    EVT_BUTTON(wxID_OK, CoopyFrame::OnOK)
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
    printf("**** Calling fossil with %d arguments\n", argc);
    wxArrayString arr;
    wxChar *cmd[256];
    wxString op;
    wxString op1;
    for (int i=0; i<argc; i++) {
        printf("[%s] ", argv[i]);
        wxString p = conv(safetxt(argv[i]));
        arr.Add(p);
        if (i>0) {
            op = op + p + wxT(" ");
        }
        if (i==1) {
            op1 = p;
        }
    }
    //addLog(wxT("Command: ") + op);
    if (op1 == wxT("update")) {
        addLog(wxT(" \n \nLooking for changes online..."));
    } else if (op1 == wxT("changes")) {
        addLog(wxT(" \n \nChecking for changes on this computer..."));
    } else if (op1 == wxT("commit")) {
        addLog(wxT(" \n \nSending changes from your computer..."));
    }
    showing = (op1 == wxT("update")) || (op1 == wxT("commit"));
    for (int i=0; i<argc; i++) {
        //printf("[%s] ", conv(arr[i]).c_str());
        cmd[i] = (wxChar*)((const wxChar *)arr[i]);
    }
    printf("\n");
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
        cerr<<"Process launch error"<<endl;
        exit(1);
    }
    reportErr = proc->GetErrorStream();
    report = proc->GetInputStream();
    if (report==NULL||reportErr==NULL) {
        cerr<<"Process stream error"<<endl;
        exit(1);
    }
    if (!sync) {
        timer->Start(50);
    } else {
        while (!report->Eof()) {
            processInput();
        }
        printf("Sync done\n");
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

    timer = new wxTimer(this,ID_Tick);

    //ssfossil_set_handler(&handler);
    //handler.setOwner(*this);

    SetIcon(wxIcon((char**)appicon_xpm));

    topsizer = new wxBoxSizer( wxVERTICAL );

    /*
    m_textCtrl = new wxTextCtrl(this, -1, wxEmptyString,
                                wxDefaultPosition, wxSize(320,30));
    */

    wxBoxSizer *button_sizer = new wxBoxSizer( wxHORIZONTAL );
    
    //create two buttons that are horizontally unstretchable, 
    // with an all-around border with a width of 10 and implicit top alignment
    button_sizer->Add(
                      new wxButton( this, wxID_OK, _T("&Reset") ),
                      wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10));       
    
    button_sizer->Add(
                      new wxButton( this, ID_Quit, _T("E&xit") ),
                      wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10));    


    wxSizerFlags tflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxLEFT|wxRIGHT|wxTOP, 10);
    wxSizerFlags flags = 
        wxSizerFlags(0).Align(wxALIGN_CENTER).Border(wxALL, 10);
    wxSizerFlags rflags = 
        wxSizerFlags(0).Align(wxALIGN_RIGHT).Border(wxALL, 10);
    wxSizerFlags lflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxALL, 10);

    /*
    wxBoxSizer *source_bar = new wxBoxSizer( wxHORIZONTAL );
    source_bar->Add(new wxStaticText(this,-1,_T("Repo"),
                                     wxDefaultPosition,
                                     wxSize(60,-1)),lflags);
    src_box = new wxTextCtrl(this,TEXT_Src, wxT(""),
                             wxDefaultPosition,
                             wxSize(300,-1));
    source_bar->Add(src_box,lflags);
    */
    //source_bar->Add(new wxButton( this, ID_Undo, _T("&Undo") ),
    //lflags);
    //topsizer->Add(source_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));


    /*
    wxBoxSizer *dest_bar = new wxBoxSizer( wxHORIZONTAL );
    dest_bar->Add(new wxStaticText(this,-1,_T("Push to"),
                                   wxDefaultPosition,
                                   wxSize(70,-1)),lflags);
    dest_box = new wxTextCtrl(this,TEXT_Src, wxT(""),
                              wxDefaultPosition,
                              wxSize(300,-1));
    dest_bar->Add(dest_box,lflags);
    dest_bar->Add(new wxButton( this, ID_Commit, _T("Push &out") ),
                  lflags);
    topsizer->Add(dest_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));
    */

    
    wxBoxSizer *dir_bar = new wxBoxSizer( wxHORIZONTAL );
    //dir_bar->Add(new wxStaticText(this,-1,_T("Store"),
    //                             wxDefaultPosition,
    //                             wxSize(60,-1)),lflags);
    dir_box = new wxDirPickerCtrl(this,TEXT_Dir, wxT(""),
                                  wxT("Select a folder"),
                                  wxDefaultPosition,
                                  wxSize(300,-1));
                                  //wxDIRP_USE_TEXTCTRL);

    const wxString choices[] = {
        wxT("... Add ..."),
    };
    list_box = new wxListBox(this,ID_LISTBOX, wxPoint(10,10), wxSize(250,70),
                             1, choices, wxLB_SINGLE | wxLB_ALWAYS_SB |
                             wxHSCROLL);

    //dir_box->SetTextCtrlProportion(0);

    if (CoopyApp::fossil_object!="") {
        wxFileName name = wxFileName::FileName(conv(CoopyApp::fossil_object));
        name.MakeAbsolute();
        path = conv(name.GetPath());
        dir_box->SetPath(name.GetPath());
        askPath = false;
    } else {
#ifdef __LINUX__
        dir_box->SetPath(::wxGetCwd());
#else
        wxStandardPaths sp;
        dir_box->SetPath(sp.GetDocumentsDir());
#endif
    }

    dir_bar->Add(dir_box,lflags);
    dir_bar->Add(new wxButton( this, ID_Sync, _T("Pull &in") ),
                    lflags);
    dir_bar->Add(new wxButton( this, ID_Commit, _T("Push &out") ),
                    lflags);
    dir_bar->Add(new wxButton( this, ID_Create, _T("Create &repository") ),
                    lflags);
    topsizer->Add(dir_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));

    
    log_box = new wxTextCtrl(this, TEXT_Main, conv(string("Welcome to Coopy!\n\nThe purpose of Coopy is to facilitate cooperative data-collection projects. \n\nSee http://") + SITE_NAME + " to get a repository link.\n\nWarning: this is pre-alpha software, keep backups of your data.\n"), 
                             wxDefaultPosition, wxSize(620,200),  
                             wxTE_MULTILINE | wxTE_RICH, 
                             wxDefaultValidator, wxTextCtrlNameStr);

    //handler.setCtrl(*log_box);

    topsizer->Add(log_box);
    topsizer->Add(list_box);
    topsizer->Add(button_sizer,wxSizerFlags(0).Align(wxALIGN_RIGHT));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    /*
    if (m_textCtrl!=NULL) {
        m_textCtrl->Clear();
        m_textCtrl->AppendText(wxString(FOSSIL_REPO,
                                        wxConvUTF8));
    }
    */

    updateSettings(false);
    updateListing();

    return true;
}


bool CoopyFrame::havePath() {
    if (dir_box) {
        string ref = conv(dir_box->GetPath());
        if (ref!=path) {
            path = ref;
        }
    }

    if (path=="" || askPath) {
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
                        printf("%s\n", fname.c_str());
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
                printf("Selected a directory %s\n", path.c_str());
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

    if (dir_box) {
        string ref = conv(dir_box->GetPath());
        if (ref!=path && path!="") {
            dir_box->SetPath(conv(path));
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
    printf("update listing\n");
    list<string> files = getFiles();
    map<string,int> present;
    present["... Add ..."] = 1;
    for (list<string>::const_iterator it = files.begin();
         it != files.end();
         it++) {
        if (it->rfind(".csvs")!=string::npos) {
            string str = it->substr(0,it->rfind(".csvs"));
            printf("file of interest %s -> %s\n", it->c_str(), str.c_str());
            wxString item = conv(str);
            int result = list_box->FindString(item);
            if (result==wxNOT_FOUND) {
                list_box->InsertItems(1,&item,0);
            }
            present[str] = 1;
        }
    }

    int ct = list_box->GetCount();
    int offset = 0;
    for (int i=0; i<ct; i++) {
        wxString item = list_box->GetString(i-offset);
        string str = conv(item);
        if (present.find(str)==present.end()) {
            printf("cannot find %s\n", str.c_str());
            list_box->Delete(i-offset);
            offset++;
        }
    }
    return true;
}


bool CoopyFrame::pushListing(bool reverse) {
    printf("push listing %d\n", reverse);
    list<string> files = fileCache;
    for (list<string>::const_iterator it = files.begin();
         it != files.end();
         it++) {
        printf("checking %s\n", it->c_str());
        if (it->rfind(".csvs")!=string::npos) {
            string str = it->substr(0,it->rfind(".csvs"));
            printf("checking %s -> %s\n", it->c_str(), str.c_str());
            string local = ws.getFile(str.c_str());
            if (local=="") continue;
            string remote = *it;

            wxFileName localName = wxFileName::FileName(conv(local));
            wxFileName remoteName = wxFileName::FileName(conv(remote));
            if (!localName.FileExists()) continue;
            wxFileName markName = wxFileName::FileName(conv(remote+".mark"));
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
                act = remoteTime.IsEarlierThan(localTime);
                if (haveMark) {
                    act = act && markTime.IsEarlierThan(localTime);
                }
            } else {
                act = localTime.IsEarlierThan(remoteTime);
                if (haveMark) {
                    act = act && markTime.IsEarlierThan(remoteTime);
                }
            }
            if (act) {
                printf("  time to update (%s, %s)\n", local.c_str(), remote.c_str());
                bool ok = false;
                if (reverse) {
                    ok = ws.exportSheet(str.c_str()); // should be patch based
                } else {
                    ok = ws.importSheet(str.c_str());
                }
                if (ok) {
                    if (!haveMark) {
                        wxFile f;
                        f.Create(markName.GetFullPath());
                    }
                    markName.Touch();
                }
            }
        }
    }

    return true;
}


bool CoopyFrame::createFile() {
    string msg = "Sorry, you need to make new files manually just now.  Just add a blank file called demo.csvs in the same directory as repository.coopy";

    wxTextEntryDialog dlg(NULL, 
                          wxT("Enter a simple name for the file in the repository.\nAny spaces or punctuation will be replaced by '_' characters.\nYou'll be able to save with a different name on your computer."),
                          wxT("Set name"),
                          wxT("example_name"));
    if (dlg.ShowModal()!=wxID_OK) {
        return false;
    }
    wxString actName = dlg.GetValue() + wxT(".csvs");
    wxFileName name = wxFileName::FileName(actName);
    if (!name.FileExists()) {
        wxFile f;
        f.Create(name.GetFullPath());
        f.Write(wxT("== Main Sheet ==\n"));
        f.Write(wxT("Name,Number\n"));
        f.Write(wxT("-----------\n"));
        f.Write(wxT("One,1\n"));
        f.Write(wxT("Two,2\n"));
        f.Write(wxT("Three,3\n"));
        f.Write(wxT("Four,4\n"));
        f.Write(wxT("Five,5\n"));
        f.Close();
    }
    list<string> files;
    files.push_back(conv(actName));
    doFiles(files,"add");
    updateListing();
    return true;
}


bool CoopyFrame::openFile(const wxString& str) {
    string key = conv(str);
    string fname = ws.getFile(key.c_str());
    bool exists = false;
    if (fname!="") {
        wxFileName name = wxFileName::FileName(conv(fname));
        exists = name.FileExists();
    }
    if (fname==""||!exists) {
        wxFileDialog SaveDialog(this, _("Save File As _?"), wxEmptyString, conv(fname),
                                _("Excel files (*.xls)|*.xls|Sqlite files (*.sqlite)|*.sqlite|CSV files (*.csv)|*.csv"),
                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
        if (SaveDialog.ShowModal() == wxID_OK) {
            //CurrentDocPath = SaveDialog.GetPath();
            //MainEditBox->SaveFile(CurrentDocPath); // Save the file to the selected path
            // Set the Title to reflect the file open
            //SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
            fname = conv(SaveDialog.GetFilename());
            ws.setFile(key.c_str(),fname.c_str());
            ws.exportSheet(key.c_str());
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

    show(fname);

    return true;
}


bool CoopyFrame::haveSource() {
    if (src_box) {
        string ref = conv(src_box->GetValue());
        if (ref!=source) {
            source = ref;
        }
    }

    if (source==""||askSource) {
        string suggest = source;
        if (suggest=="") {
            suggest = ""; 
            //suggest = "http://coopy.sourceforge.net/cgi-bin/wiki/home";
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
            printf("Source set to %s\n", source.c_str());
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

void CoopyFrame::OnOK(wxCommandEvent& ev) {
    askPath = true;
    askSource = true;
    askDestination = true;
    if (src_box) { src_box->ChangeValue(wxT("")); }
    if (dest_box) { dest_box->ChangeValue(wxT("")); }
}

void CoopyFrame::OnSync(wxCommandEvent& event) {
    printf("Syncing...\n");
    next = "";
    //startStream();
    if (havePath()) {
        printf("Should pull %s\n", path.c_str());
        wxChar sep = wxFileName::GetPathSeparator();
        wxString target = conv(path) + sep + wxT("repository.coopy");
        if (!wxFileExists(target)) {
            wxString target2 = conv(path) + sep + wxT("clone.fossil");
            if (wxFileExists(target2)) {
                target = target2;
            }
        }
        string ctarget = conv(target);
        if (!wxFileExists(target)) {
            printf("Could not find %s\n", ctarget.c_str());
            if (haveSource()) {
                printf("Need to clone %s\n", ctarget.c_str());
                int argc = 4;
                char *argv[] = {
                    fossil(),
                    (char*)"clone",
                    (char*)source.c_str(),
                    (char*)ctarget.c_str(),
                    NULL };
                for (int i=0; i<argc; i++) {
                    printf("HAVE %s\n", argv[i]);
                }
                next = "sync";
                ssfossil(argc,argv);
                return;
            } else {
                return;
            }
        }
        if (wxFileExists(target)) {
            wxString view_target = conv(path) + sep + wxT("_FOSSIL_");
            if (!wxFileExists(view_target)) {
                printf("No view yet %s\n", conv(view_target).c_str());
                int argc = 3;
                char *argv[] = {
                    fossil(),
                    (char*)"open",
                    (char*)ctarget.c_str(),
                    NULL };
                next = "sync";
                ssfossil(argc,argv);
                return;
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
                printf("Simple sync\n");
                int argc = 2;
                char *argv[] = {
                    fossil(),
                    (char*)"update",
                    NULL };
                next = "view_sync";
                ssfossil(argc,argv);
            }
        }
    }
    //updateSettings(true);
    //endStream();
}


void CoopyFrame::OnUndo(wxCommandEvent& event) {
    printf("Should undo\n");
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
    printf("Create!\n");
    ::wxLaunchDefaultBrowser(wxT(SITE_NAME_CREATE));
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
    printf("Should commit\n");
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
                printf("Message is %s\n", msg.c_str());
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
                printf("Message is %s\n", msg.c_str());
                wxTextEntryDialog dlg(NULL, conv(msg));
                if (dlg.ShowModal()==wxID_OK) {
                    commit_message = conv(dlg.GetValue());
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
                    //next = "push";
                    ssfossil(argc,argv);
                    return;
                } else {
                    return;
                }
            } else {
                msg = "No changes!";
                addLog(_T("No changes to push"));
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

    logging = false;
    path = "";
    source = "";
    commit_message = "";

    wxMenu *menuFile = new wxMenu;

    menuFile->Append( ID_Sync, _T("Pull &in...") );
    menuFile->Append( ID_Commit, _T("Push &out...") );
    menuFile->Append( ID_Create, _T("Create &repository...") );
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


#ifdef _WIN32

FILE *FOUT = NULL;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR m_lpCmdLine, int nCmdShow) {

    //g_hinstance = (int)(hInstance);    
    return wxEntry(hInstance,hPrevInstance,m_lpCmdLine,nCmdShow);
}

#endif

