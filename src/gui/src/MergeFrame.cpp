
#include "MergeFrame.h"

//#include <wx/base64.h>

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

// we count on UTF-8 build
static wxString toReg(wxString str) {
  return str;
  //wxScopedCharBuffer buf = str.ToUTF8();
  //return wxString("coopy_v1_") + wxBase64Encode(buf.data(),buf.length());
}

static wxString fromReg(wxString str) {
  return str;
  /*
  //printf("Reading %s\n", conv(str).c_str());
  if (str.Mid(0,9)!="coopy_v1_") {
    return "";
  }
  str = str.Mid(9);
  //printf("Part is %s\n", conv(str).c_str());
  wxMemoryBuffer buf = wxBase64Decode(str.c_str(),str.Length());
  //printf("Buffer length is %d\n", buf.GetDataLen());
  wxString result = wxString::FromUTF8((const char *)buf.GetData(),
				       (size_t)buf.GetDataLen());
  //printf("Outcome is %s\n", conv(result).c_str());
  return result;
  */
}

static void show(const wxString& view) {
#ifndef WIN32
    wxString view2 = wxT("file://") + view;
    ::wxLaunchDefaultBrowser(view2);
#else
    ::wxLaunchDefaultBrowser(view);
    //::wxLaunchDefaultApplication(view);
#endif
}

static void show(const std::string& view) {
    wxString x = conv(view);
    show(x);
}


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

  //SetIcon(wxIcon((char**)appicon_xpm));

    topsizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *dir_bar = new wxBoxSizer( wxHORIZONTAL );

    wxSizerFlags lflags = 
        wxSizerFlags(0).Align(wxALIGN_LEFT).Border(wxALL, 10);

    action_button = new wxButton( this, ID_Merge, _T("&Merge"));
    dir_bar->Add(action_button,
		 lflags);

    dir_bar->Add(new wxButton( this, ID_Quit, _T("&Cancel") ),
                 lflags);    
    const char *tips[5] = { 
        "Select a common ancestor",
        "Select main version",
        "Select other version",
        "Select patch file",
        "Select output"
    };
    const char *labels[5] = {
        "Common ancestor",
        "Main version",
        "Other version",
        "Patch file",
        "Output"
    };
    const char *tags[5] = {
        "Ancestor",
        "LocalVersion",
        "RemoteVersion",
        "Patch",
        "Output"
    };


    wxConfigBase *pConfig = wxConfigBase::Get();
    for (int i=0; i<5; i++) {
        wxBoxSizer *fbar = new wxBoxSizer( wxHORIZONTAL );
	boxes[i] = fbar;
        config_tags[i] = conv_c(tags[i]);
        wxString path = fromReg(pConfig->Read(config_tags[i],wxT("")));
        files[i] = new wxFilePickerCtrl(this,TEXT_Parent+i, 
                                        path,
                                        conv_c(tips[i]),
                                        wxT("*.*"),
                                        wxDefaultPosition,
                                        wxSize(400,-1),
					(i==4)?(wxFLP_USE_TEXTCTRL|wxFLP_SAVE|wxFLP_OVERWRITE_PROMPT):(wxFLP_DEFAULT_STYLE));
        if (files[i]->GetTextCtrl()) {
	  // there's a strange wxmsw crash otherwise
	  //files[i]->GetTextCtrl()->AutoComplete(NULL);
	  //files[i]->GetTextCtrl()->SetEditable(false);
	  //files[i]->GetTextCtrl()->SetValue(path);
        }
        files[i]->SetPath(path);
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
    wxString patch = files[3]->GetPath();
    wxString output = files[4]->GetPath();
    wxConfigBase *pConfig = wxConfigBase::Get();
    //printf("CONFIG %ld\n", (long)pConfig);
    for (int i=0; i<5; i++) {
        wxString key = config_tags[i];
        wxString val = files[i]->GetPath();
        //printf("i is %d :  %s -> %s\n", i, conv(key).c_str(), conv(val).c_str());
	//printf("Writing %s for %s\n", conv(toReg(val)).c_str(),conv(val).c_str());
        pConfig->Write(key,toReg(val));
    }
    pConfig->Flush();
    Options opt((string("ss")+mode).c_str());
    vector<string>& core = opt.core;
    if (mode=="merge") {
      core.push_back(conv(parent));
      core.push_back(conv(local));
      core.push_back(conv(remote));
    } else if (mode=="diff") {
      core.push_back(conv(local));
      core.push_back(conv(remote));
      opt.option_string["mode"] = "hilite";
    } else if (mode=="patch") {
      core.push_back(conv(local));
      core.push_back(conv(patch));
    }
    opt.option_string["output"] = conv(output);

    Diff app;
    int result = app.apply(opt);

    if (result==0) {
        wxMessageDialog msg(NULL, wxT("Done! View result?"),
                            wxT("Done!"),wxYES_NO|wxCANCEL);
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
    printf("Output %s\n", conv(output).c_str());
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


MergeFrame::MergeFrame(wxWindow *parent, const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(parent, -1, title, pos, size)
{

    wxMenu *menuFile = new wxMenu;

    //menuFile->Append( ID_LoadJob, _T("&Load Job...") );
    //menuFile->Append( ID_SaveJob, _T("&Save Job...") );
    //menuFile->Append( ID_About, _T("&About...") );
    menuFile->AppendSeparator();
    menuFile->Append( ID_Quit, _T("&Finish") );

    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, _T("&Job") );

    SetMenuBar( menuBar );

    CreateStatusBar();
    SetStatusText( _T("Compare and contrast") );
}

void MergeFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    printf("Quit!\n");
    //wxCloseEvent ev;
    //wxPostEvent(this,ev);
    Show(FALSE);
    //Close(TRUE);
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

void MergeFrame::SetMode(const char *mode) {
  this->mode = mode;
  string action = "&Merge";
  bool shows[5] = { true, true, true, true, true };
  if (this->mode=="merge") {
    action = "&Merge";
    shows[3] = false;
  }
  if (this->mode=="diff") {
    action = "&Compare";
    shows[0] = false;
    shows[3] = false;
  }
  if (this->mode=="patch") {
    action = "&Patch";
    shows[0] = false;
    shows[2] = false;
  }
  for (int i=0; i<5; i++) {
    wxSizerItemList& lst = boxes[i]->GetChildren();
    for (wxSizerItemList::iterator iter = lst.begin(); 
	 iter != lst.end(); iter++) {
      (*iter)->Show(shows[i]);
    }
    boxes[i]->Layout();
  }
  action_button->SetLabel(conv(action));
  topsizer->Layout();
}
