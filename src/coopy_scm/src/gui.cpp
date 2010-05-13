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

#include <string>
#include <list>
#include <iostream>

#include "ssfossil.h"

// hack to remove warning
#define static const
#include "icon/appicon.xpm"
#undef static

using namespace std;

int g_hinstance = 0;
int g_hwnd = 0;

static wxString conv(const std::string& s) {
    return wxString(s.c_str(), wxConvUTF8);
}

static std::string conv(const wxString& s) {
    return std::string(s.mb_str(wxConvUTF8));
} 

class MyApp: public wxApp {
private:
    bool silent;
public:
    MyApp() {
        silent = false;
    }

    virtual bool OnInit();

    virtual void OnInitCmdLine(wxCmdLineParser& parser);
    virtual bool OnCmdLineParsed(wxCmdLineParser& parser);
};

static const wxCmdLineEntryDesc g_cmdLineDesc [] = {
    { wxCMD_LINE_SWITCH, wxT("h"), wxT("help"), wxT("displays help on the command line parameters"),
      wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_SWITCH, wxT("s"), wxT("silent"), wxT("disables the GUI") },
    { wxCMD_LINE_OPTION, wxT("r"), wxT("res"), wxT("set resource location"),
      wxCMD_LINE_VAL_STRING, 0  },
    { wxCMD_LINE_NONE },
};


void MyApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("--"));
}
 
bool MyApp::OnCmdLineParsed(wxCmdLineParser& parser) {
    silent = parser.Found(wxT("s"));

    wxString location;
    if (parser.Found(wxT("r"),&location)) {
        string loc = conv(location);
        printf("*** should set location to [%s]\n", loc.c_str());
    }
    
    // to get at your unnamed parameters use
    wxArrayString files;
    for (int i = 0; i < parser.GetParamCount(); i++) {
        files.Add(parser.GetParam(i));
    }
    
    // and other command line parameters
    
    // then do what you need with them.
    
    return true;
}

#ifdef WIN32
IMPLEMENT_APP_NO_MAIN(MyApp);
#else
IMPLEMENT_APP(MyApp);
#endif



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
        int lastPos = str.find_first_not_of(delimiters, 0);
        int pos = str.find_first_of(delimiters, lastPos);
        while (string::npos != pos || string::npos != lastPos) {
            tokens.push_back(str.substr(lastPos, pos - lastPos));
            lastPos = str.find_first_not_of(delimiters, pos);
            pos = str.find_first_of(delimiters, lastPos);
        }
    }


    void replace(std::string& str, const std::string& old, 
                 const std::string& rep) {
        int pos = 0;
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
                /*
                for (int i=0; i<strlen(txt); i++) {
                    ::printf("[%d:%c] ", txt[i], (txt[i]>=32)?txt[i]:'*');
                }
                ::printf("\n");
                */
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


class MyFrame: public wxFrame
{
    DECLARE_CLASS(MyFrame)
    DECLARE_EVENT_TABLE()

private:
    wxBoxSizer *topsizer;
    //wxTextCtrl* m_textCtrl;

    wxTextCtrl *log_box;
    wxTextCtrl *src_box;
    wxTextCtrl *dest_box;
    wxDirPickerCtrl *dir_box;

    UiFossilHandler handler; 

    bool askPath;
    bool askSource;
    bool askDestination;

    string path;
    string source;
    string destination;
    string commit_message;
    ostream *stream;

public:

    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnExit(wxCloseEvent& event) {
        Destroy();
        printf("Exiting Coopy\n");
    }

    void OnOK(wxCommandEvent& event);

    virtual bool OnInit();

    void OnSync(wxCommandEvent& event);
    void OnCommit(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);

    bool havePath();
    bool haveSource();
    bool haveDestination();

    int ssfossil(int argc, char *argv[]) {
        try {
            return ssfossil_call(argc,argv);
        } catch (int err) {
            printf("exit(%d) called in ssfossil_call\n", err);
        }
        return -1;
    }

    list<string> getChanges() {
        handler.beginLog();
        if (havePath()) {
            int argc = 2;
            char *argv[] = {
                (char*)"fossil",
                (char*)"changes",
                NULL };
            ssfossil(argc,argv);
        }
        return handler.endLog();
    }

    list<string> getExtras() {
        handler.beginLog();
        if (havePath()) {
            int argc = 2;
            char *argv[] = {
                (char*)"fossil",
                (char*)"extras",
                NULL };
            ssfossil(argc,argv);
        }
        return handler.endLog();
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
        endStream();
        if (log_box) {
            log_box->Clear();
            stream = new ostream(log_box);
        }
        handler.setStream(stream);
        return stream;
    }

    void endStream() {
        if (stream) {
            delete stream;
            stream = NULL;
        }
        handler.setStream(stream);
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
                argv[0] = (char*)"fossil";
                argv[1] = (char*)act;
                int i = 2;
                for (list<string>::const_iterator it = files.begin();
                     it != files.end();
                     it++) {
                    argv[i] = (char*)(it->c_str());
                    i++;
                }
                ssfossil(argc,argv);
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
    };
};




bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame( _T("Coopy"), wxPoint(50,50), wxSize(450,340) );

    g_hwnd = (int)(frame->GetHandle());

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



IMPLEMENT_CLASS(MyFrame, wxFrame)

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Quit, MyFrame::OnQuit)
    EVT_MENU(ID_About, MyFrame::OnAbout)
    EVT_BUTTON(wxID_OK, MyFrame::OnOK)
    EVT_BUTTON(ID_Quit, MyFrame::OnQuit)
    EVT_BUTTON(ID_Sync, MyFrame::OnSync)
    EVT_BUTTON(ID_Undo, MyFrame::OnUndo)
    EVT_BUTTON(ID_Commit, MyFrame::OnCommit)
    EVT_CLOSE(MyFrame::OnExit)
END_EVENT_TABLE()


bool MyFrame::OnInit() {

    stream = NULL;
    askPath = true;
    askSource = true;
    askDestination = true;

    ssfossil_set_handler(&handler);
    handler.setOwner(*this);

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

    wxBoxSizer *source_bar = new wxBoxSizer( wxHORIZONTAL );
    source_bar->Add(new wxStaticText(this,-1,_T("Pull from"),
                                     wxDefaultPosition,
                                     wxSize(70,-1)),lflags);
    src_box = new wxTextCtrl(this,TEXT_Src, wxT(""),
                             wxDefaultPosition,
                             wxSize(300,-1));
    source_bar->Add(src_box,lflags);
    source_bar->Add(new wxButton( this, ID_Sync, _T("Pull &in") ),
                    lflags);
    source_bar->Add(new wxButton( this, ID_Undo, _T("&Undo") ),
                    lflags);
    topsizer->Add(source_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));


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

    
    wxBoxSizer *dir_bar = new wxBoxSizer( wxHORIZONTAL );
    dir_bar->Add(new wxStaticText(this,-1,_T("Directory"),
                                   wxDefaultPosition,
                                   wxSize(70,-1)),lflags);
    dir_box = new wxDirPickerCtrl(this,TEXT_Dir, wxT(""),
                                  wxT("Select a folder"),
                                  wxDefaultPosition,
                                  wxSize(300,-1));
                                  //wxDIRP_USE_TEXTCTRL);
    //dir_box->SetTextCtrlProportion(0);
    dir_box->SetPath(::wxGetCwd());
    dir_bar->Add(dir_box,lflags);
    topsizer->Add(dir_bar,wxSizerFlags(0).Align(wxALIGN_LEFT));

    
    log_box = new wxTextCtrl(this, TEXT_Main, wxT("Welcome to Coopy!\n\nThe purpose of Coopy is to facilitate cooperative data-collection projects. It uses fossil (www.fossil-scm.org) to share files between computers, and works to merge spreadsheets intelligently.\n\nWarning: this is pre-alpha software, keep backups of your data.\n\n[Status messages appear here during actions]\n"), 
                             wxDefaultPosition, wxSize(620,200),  
                             wxTE_MULTILINE | wxTE_RICH, 
                             wxDefaultValidator, wxTextCtrlNameStr);

    handler.setCtrl(*log_box);

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


bool MyFrame::havePath() {
    if (dir_box) {
        string ref = conv(dir_box->GetPath());
        if (ref!=path) {
            path = ref;
        }
    }

    if (path=="" || askPath) {
        wxDirDialog dlg(NULL, wxT("Choose input directory"), 
                        conv(path),
                        wxDD_DEFAULT_STYLE); // | wxDD_DIR_MUST_EXIST);
        if (dlg.ShowModal()==wxID_OK) {
            wxString result = dlg.GetPath();
            path = conv(result);
            printf("Selected a directory %s\n", result.c_str());
            askPath = false;
        } else {
            path = "";
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


bool MyFrame::haveSource() {
    if (src_box) {
        string ref = conv(src_box->GetValue());
        if (ref!=source) {
            source = ref;
        }
    }

    if (source==""||askSource) {
        string suggest = source;
        if (suggest=="") {
            suggest = "http://coopy.sourceforge.net/cgi-bin/wiki/home";
        }
        wxTextEntryDialog dlg(NULL, wxT("Enter source URL"),
                              wxT("Enter source URL"),
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

bool MyFrame::haveDestination() {
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

void MyFrame::OnOK(wxCommandEvent& ev) {
    askPath = true;
    askSource = true;
    askDestination = true;
    if (src_box) { src_box->ChangeValue(wxT("")); }
    if (dest_box) { dest_box->ChangeValue(wxT("")); }
}

void MyFrame::OnSync(wxCommandEvent& event) {
    startStream();
    if (haveSource()) {
        if (havePath()) {
            printf("Should pull %s\n", path.c_str());
            wxChar sep = wxFileName::GetPathSeparator();
            wxString target = conv(path) + sep + wxT("clone.fossil");
            if (!wxFileExists(target)) {
                printf("Need to clone %s\n", conv(target).c_str());
                if (haveSource()) {
                    int argc = 4;
                    char *argv[] = {
                        (char*)"fossil",
                        (char*)"clone",
                        (char*)source.c_str(),
                        (char*)target.c_str(),
                        NULL };
                    ssfossil(argc,argv);
                }
            }
            if (wxFileExists(target)) {
                wxString view_target = conv(path) + sep + wxT("_FOSSIL_");
                if (!wxFileExists(view_target)) {
                    printf("No view yet %s\n", conv(view_target).c_str());
                    int argc = 3;
                    char *argv[] = {
                        (char*)"fossil",
                        (char*)"open",
                        (char*)target.c_str(),
                        NULL };
                    ssfossil(argc,argv);

                    if (wxFileExists(view_target)) {
                        // get rid of autosync
                        int argc = 4;
                        char *argv[] = {
                            (char*)"fossil",
                            (char*)"setting",
                            (char*)"autosync",
                            (char*)"0",
                            NULL };
                        ssfossil(argc,argv);
                    }
                }
                if (wxFileExists(view_target)) {
                    printf("Simple sync\n");
                    int argc = 2;
                    char *argv[] = {
                        (char*)"fossil",
                        (char*)"update",
                        NULL };
                    ssfossil(argc,argv);
                }
            }
        }
    }
    endStream();
}


void MyFrame::OnUndo(wxCommandEvent& event) {
    printf("Should undo\n");
    startStream();
    if (havePath()) {
        int argc = 2;
        char *argv[] = {
            (char*)"fossil",
            (char*)"undo",
            NULL };
        ssfossil(argc,argv);
    }
    endStream();
}

void MyFrame::OnCommit(wxCommandEvent& event) {
    printf("Should commit\n");
    startStream();
    if (havePath()) {
        if (haveDestination()) {
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
            } else {
                msg = "No changes!";
                (*stream) << "No changes to push" << endl;
                endStream();
                return;
            }
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
                    (char*)"fossil",
                    (char*)"commit",
                    //(char*)"--user",
                    //(char*)FOSSIL_USERNAME,
                    (char*)"-m",
                    (char*)commit_message.c_str(),
                    NULL };
                ssfossil(argc,argv);

                if (true) {
                    int argc = 3;
                    char *argv[] = {
                        (char*)"fossil",
                        (char*)"push",
                        (char*)destination.c_str(),
                        NULL };
                    ssfossil(argc,argv);
                }
            }
        }
    }
    endStream();
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{
    //m_textCtrl = NULL;

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

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    //wxCloseEvent ev;
    //wxPostEvent(this,ev);
    Close(TRUE);
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Welcome to coopy!\n\
Now go back to work."),
                 _T("About coopy"), wxOK | wxICON_INFORMATION, this);
}


#ifdef WIN32

FILE *FOUT = NULL;

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR m_lpCmdLine, int nCmdShow) {

    g_hinstance = (int)(hInstance);    
    return wxEntry(hInstance,hPrevInstance,m_lpCmdLine,nCmdShow);
}

#endif

