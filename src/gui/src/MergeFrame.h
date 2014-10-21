// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright (C) 2010 Paul Fitzpatrick
 * CopyPolicy: Released under the terms of the GNU GPL v2.0.
 *
 */

#ifndef COOPY_MERGEFRAME
#define COOPY_MERGEFRAME

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


/*
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

*/

class MergeFrame: public wxFrame
{
    DECLARE_CLASS(MergeFrame)
    DECLARE_EVENT_TABLE()

private:
    wxBoxSizer *topsizer;
    wxFilePickerCtrl *files[5];

    wxString config_tags[5];

    std::string mode;

    wxButton *action_button;
    wxBoxSizer *boxes[5];

public:

    MergeFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size);

    virtual bool OnInit();

    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);

    void OnSaveJob(wxCommandEvent& event);
    void OnLoadJob(wxCommandEvent& event);

    void SetMode(const char *mode);

    void OnExit(wxCloseEvent& event) {
      //Destroy();
      //printf("Exiting ssmerge\n");
      Show(FALSE);
    }
  
    void OnMerge(wxCommandEvent& event);

    enum
    {
        TEXT_Main = wxID_HIGHEST + 101,
        TEXT_Parent,
        TEXT_Local,
        TEXT_Remote,
        TEXT_Patch,
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


#endif
