// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#include <coopy/CsvFile.h>
#include <coopy/BookCompare.h>
//#include <coopy/MergeOutputAccum.h>
#include <coopy/Options.h>
#include <coopy/Diff.h>
#include <coopy/PolyBook.h>

using namespace coopy::store;
using namespace coopy::cmp;
using namespace coopy::app;

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
#include <wx/textctrl.h>
#include <wx/url.h>
#include <wx/filepicker.h>
#include <wx/process.h>
#include <wx/stdpaths.h>
#include <wx/txtstrm.h>
#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/config.h>
#include <wx/log.h>

#include <string>
#include <list>
#include <iostream>

// hack to remove warning
#define static const
#include "icon/appicon.xpm"
#undef static

using namespace std;

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


class MergeApp: public wxApp {
private:
    bool silent;
public:
    MergeApp() {
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
    //{ wxCMD_LINE_PARAM, NULL, NULL, wxT("input file"), wxCMD_LINE_VAL_STRING,
    //wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_NONE },
};


void MergeApp::OnInitCmdLine(wxCmdLineParser& parser) {
    parser.SetDesc (g_cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars (wxT("--"));
}
 
bool MergeApp::OnCmdLineParsed(wxCmdLineParser& parser) {
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

    // and other command line parameters

    // then do what you need with them.
    
    return true;
}

#ifdef WIN32
IMPLEMENT_APP_NO_MAIN(MergeApp);
#else
IMPLEMENT_APP(MergeApp);
#endif


class MergeFrame: public wxFrame
{
    DECLARE_CLASS(MergeFrame)
    DECLARE_EVENT_TABLE()

private:
    wxBoxSizer *topsizer;
    wxFilePickerCtrl *files[4];

    wxString config_tags[4];

public:

    MergeFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

    virtual bool OnInit();

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSaveJob(wxCommandEvent& event);
    void OnLoadJob(wxCommandEvent& event);

    void OnExit(wxCloseEvent& event) {
        Destroy();
        printf("Exiting ssmerge\n");
    }
  
    void OnMerge(wxCommandEvent& event);

    enum
    {
        TEXT_Main = wxID_HIGHEST + 1,
        TEXT_Parent,
        TEXT_Local,
        TEXT_Remote,
        TEXT_Output,
        TEXT_MORE1,
        TEXT_MORE2,
        TEXT_MORE3,
        ID_Quit,
        ID_Merge,
        ID_About,
        ID_SaveJob,
        ID_LoadJob,
    };
};


bool MergeApp::OnInit()
{
    MergeFrame *frame = new MergeFrame( _T("ssmerge"), wxPoint(50,50), wxSize(450,340) );

    if (!wxApp::OnInit()) {
        return false;
    }


    SetVendorName(wxT("DataCommonsCoop"));
    SetAppName(wxT("ssmerge_gui"));

    wxConfigBase *pConfig = wxConfigBase::Get();

    // uncomment this to force writing back of the defaults for all values
    // if they're not present in the config - this can give the user an idea
    // of all possible settings for this program
    pConfig->SetRecordDefaults();

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



IMPLEMENT_CLASS(MergeFrame, wxFrame)

BEGIN_EVENT_TABLE(MergeFrame, wxFrame)
    EVT_MENU(ID_Quit, MergeFrame::OnQuit)
    EVT_MENU(ID_About, MergeFrame::OnAbout)
    EVT_MENU(ID_SaveJob, MergeFrame::OnSaveJob)
    EVT_MENU(ID_LoadJob, MergeFrame::OnLoadJob)
    EVT_BUTTON(ID_Merge, MergeFrame::OnMerge)
    EVT_BUTTON(ID_Quit, MergeFrame::OnQuit)
    EVT_CLOSE(MergeFrame::OnExit)
END_EVENT_TABLE()


bool MergeFrame::OnInit() {

    SetIcon(wxIcon((char**)appicon_xpm));

    topsizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *dir_bar = new wxBoxSizer( wxHORIZONTAL );

    wxSizerFlags lflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxALL, 10);

    dir_bar->Add(new wxButton( this, ID_Merge, _T("&Merge") ),
                    lflags);

    dir_bar->Add(new wxButton( this, ID_Quit, _T("E&xit") ),
                 lflags);    
    const char *tips[4] = { 
        "Select a common ancestor",
        "Select local version",
        "Select remote version",
        "Select output"
    };
    const char *labels[4] = {
        "Ancestor",
        "Local version",
        "Remote version",
        "Output"
    };
    const char *tags[4] = {
        "Ancestor",
        "LocalVersion",
        "RemoteVersion",
        "Output"
    };
        
    wxConfigBase *pConfig = wxConfigBase::Get();
    for (int i=0; i<4; i++) {
        wxBoxSizer *fbar = new wxBoxSizer( wxHORIZONTAL );
        config_tags[i] = conv_c(tags[i]);
        //printf("--> %s %s\n", conv(config_tags[i]).c_str(), tags[i]);
        wxString path = pConfig->Read(config_tags[i],wxT(""));
        //printf("  %s\n", conv(path).c_str());
        files[i] = new wxFilePickerCtrl(this,TEXT_Parent+i, 
                                        path,
                                        conv_c(tips[i]),
                                        wxT("*.*"),
                                        wxDefaultPosition,
                                        wxSize(400,-1),
                                        (i==3)?(wxFLP_USE_TEXTCTRL|wxFLP_SAVE|wxFLP_OVERWRITE_PROMPT):(wxFLP_DEFAULT_STYLE|wxFLP_CHANGE_DIR));
        files[i]->SetPath(path);
        if (files[i]->GetTextCtrl()) {
            files[i]->GetTextCtrl()->SetValue(path);
        }
        fbar->Add(new wxStaticText(this,-1,conv_c(labels[i]),
                                   wxDefaultPosition,
                                   wxSize(150,-1)),lflags);
        fbar->Add(files[i],1,wxGROW|wxALIGN_CENTER_VERTICAL|wxALL);
        fbar->SetItemMinSize(files[i],400,50);
        topsizer->Add(fbar,1,wxGROW|wxALIGN_CENTER_VERTICAL|wxALL);
    }

    topsizer->Add(dir_bar,wxSizerFlags(0).Align(wxALIGN_RIGHT));

    SetSizer(topsizer);
    topsizer->SetSizeHints(this);

    return true;
}


void MergeFrame::OnMerge(wxCommandEvent& event) {
    //printf("Merging...\n");
    wxString parent = files[0]->GetPath();
    wxString local = files[1]->GetPath();
    wxString remote = files[2]->GetPath();
    wxString output = files[3]->GetPath();
    wxConfigBase *pConfig = wxConfigBase::Get();
    //printf("CONFIG %ld\n", (long)pConfig);
    for (int i=0; i<4; i++) {
        wxString key = config_tags[i];
        wxString val = files[i]->GetPath();
        //printf("i is %d :  %s -> %s\n", i, conv(key).c_str(), conv(val).c_str());
        pConfig->Write(key,val);
    }
    pConfig->Flush();
    Options opt("ssmerge");
    vector<string>& core = opt.core;
    core.push_back(conv(parent));
    core.push_back(conv(local));
    core.push_back(conv(remote));
    opt.option_string["output"] = conv(output);

    Diff app;
    int result = app.apply(opt);

    if (result==0) {
        wxMessageDialog msg(NULL, wxT("Merged. View result?"),
                            wxT("Merged."),wxYES_NO|wxCANCEL);
        if (msg.ShowModal()==wxID_YES) {
            show(output);
        }
    } else {
        wxMessageDialog msg(NULL, wxT("That did not work out."),
                            wxT("Sorry!"));
        msg.ShowModal();
    }

    /*
    printf("Parent %s\n", conv(parent).c_str());
    printf("Local %s\n", conv(local).c_str());
    printf("Remote %s\n", conv(remote).c_str());
    PolyBook parent_sheet, local_sheet, remote_sheet;
    parent_sheet.read(conv(parent).c_str());
    local_sheet.read(conv(local).c_str());
    remote_sheet.read(conv(remote).c_str());
    CompareFlags flags;


    BookCompare cmp;
    MergeOutputAccum accum;
    cmp.compare(parent_sheet,local_sheet,remote_sheet,accum,flags);
    const DataSheet& result = accum.getSheet();
    printf("Result of dimensions %dx%d\n", result.width(), result.height());

    if (result.width()==0||result.height()==0) {
        wxMessageDialog msg(NULL, wxT("No merge took place.\nAre all files present and readable?"),
                            wxT("Sorry, no merge"));
        msg.ShowModal();
        return;
    }

	wxFileDialog *SaveDialog = new wxFileDialog(this, _("Save Merged Result As _?"), wxEmptyString, wxEmptyString,
		_("*.*"),
                                                wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);
 
	if (SaveDialog->ShowModal() == wxID_OK) {
		SetTitle(wxString(wxT("Merge - ")) << SaveDialog->GetFilename());
	} else {
        return;
    }

    if (CsvFile::write(result,conv(SaveDialog->GetPath()).c_str())!=0) {
        wxMessageDialog msg(NULL, wxT("Could not save file."),
                            wxT("Sorry, no save"));
        msg.ShowModal();
        return;
    }
    */
}


MergeFrame::MergeFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame((wxFrame *)NULL, -1, title, pos, size)
{

    wxMenu *menuFile = new wxMenu;

    //menuFile->Append( ID_LoadJob, _T("&Load Job...") );
    //menuFile->Append( ID_SaveJob, _T("&Save Job...") );
    menuFile->Append( ID_About, _T("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("D&one") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _T("&File") );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _T("Welcome to ssmerge!") );
}

void MergeFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    printf("Quit!\n");
    //wxCloseEvent ev;
    //wxPostEvent(this,ev);
    // Close(TRUE);
    Show(false);
}

void MergeFrame::OnLoadJob(wxCommandEvent& WXUNUSED(event)) 
{
    //wxFileInputStream stream("config.ini");
    //wxFileConfig config(stream);
    //wxString a = config.Read("a", "");
}

void MergeFrame::OnSaveJob(wxCommandEvent& WXUNUSED(event)) 
{
    //wxFileOutputStream stream("config.ini");
    //wxFileConfig
}

void MergeFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox(_T("Welcome to ssmerge!\n\
Now go back to work."),
                 _T("About ssmerge"), wxOK | wxICON_INFORMATION, this);
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
