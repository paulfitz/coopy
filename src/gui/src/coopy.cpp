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
#include <wx/textctrl.h>
#include <wx/url.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/arrstr.h>
#include <wx/dir.h>

#include <string>
#include <list>
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



static wxString conv_c(const char *s) {
    return wxString(s, wxConvUTF8);
}

static wxString conv(const std::string& s) {
    return wxString(s.c_str(), wxConvUTF8);
}

static std::string conv(const wxString& s) {
    return std::string(s.mb_str(wxConvUTF8));
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
    wxInputStream *report;
    wxInputStream *reportErr;

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
    list<string> results;

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
        printf("Got %s\n", conv(str).c_str());
        //replace(str,string("\r"),string(" * "));
        //replace(str,"Received:","\nReceived:");
        if (logging) { 
            printf("Logging\n");
            string cp(conv(str));
            split(cp,"\n",results);
            printf("Logged\n");
        }
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
        log_box->AppendText(str);
        log_box->AppendText(_T("\n"));
        Update();
        log_box->SetSelection(log_box->GetLastPosition(),-1);
   }

    void OnTerminate(wxProcess *process, int status) {
        processInput();
        timer->Stop();
        if (status!=0) {
            printf(">>>>> PROCESS FAILED: next ignored [%s]\n", next.c_str());
            addLog(_T("\nSomething went wrong..."));
            return;
        }
        printf(">>>>> PROCESS COMPLETE: next is [%s]\n", next.c_str());
        string n = next;
        next = "";
        if (n=="view") {
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
                for (list<string>::const_iterator it = files.begin();
                     it != files.end();
                     it++) {
                    argv[i] = (char*)(it->c_str());
                    i++;
                }
                ssfossil(argc,argv,true);
                delete[] argv;
            }
        }
    }
enum
    {
        TEXT_Main = wxID_HIGHEST + 1,
        TEXT_Src,
        TEXT_Dest,
        TEXT_Dir,
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
    FossilProcess(CoopyFrame *parent, const wxString& cmd)
        : wxProcess(parent), m_cmd(cmd)
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
};



void FossilProcess::OnTerminate(int pid, int status)
{
    /*
    wxLogStatus(m_parent, wxT("Process %u ('%s') terminated with exit code %d."),
                pid, m_cmd.c_str(), status);
    */

    m_parent->OnTerminate(this,status);
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
    EVT_MENU(ID_About, CoopyFrame::OnAbout)
    EVT_BUTTON(wxID_OK, CoopyFrame::OnOK)
    EVT_BUTTON(ID_Quit, CoopyFrame::OnQuit)
    EVT_BUTTON(ID_Sync, CoopyFrame::OnSync)
    EVT_BUTTON(ID_Undo, CoopyFrame::OnUndo)
    EVT_BUTTON(ID_Commit, CoopyFrame::OnCommit)
    EVT_BUTTON(ID_Create, CoopyFrame::OnCreate)
    EVT_CLOSE(CoopyFrame::OnExit)
    EVT_TIMER(ID_Tick, CoopyFrame::OnProgressTimer)
END_EVENT_TABLE()


int CoopyFrame::ssfossil(int argc, char *argv[], bool sync) {
    printf("Calling fossil with %d arguments\n", argc);
    wxArrayString arr;
    wxChar *cmd[256];
    for (int i=0; i<argc; i++) {
        printf("Have %s\n", argv[i]);
        arr.Add(conv(safetxt(argv[i])));
    }
    for (int i=0; i<argc; i++) {
        printf("* Have %s\n", conv(arr[i]).c_str());
        cmd[i] = (wxChar*)((const wxChar *)arr[i]);
    }
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
    FossilProcess *proc = new FossilProcess(this,_T("ssfossil"));
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
                next = "view";
                ssfossil(argc,argv);
            }
        }
    }
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

