// wxWidgets "Hello World" Program
 
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
 
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
 
class Prawnstar : public wxApp
{
public:
    virtual bool OnInit();
};
 
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
 
private:
    void OnHello(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
};
 
enum
{
    ID_Hello = 1
};
 
wxIMPLEMENT_APP(Prawnstar);
 
bool Prawnstar::OnInit()
{
    MainFrame *frame = new MainFrame("Prawnstar Blockchain", wxPoint(0, 0), wxSize(990, 640));
    frame->Center();
    frame->Show(true);
    return true;
}
 
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
 
    wxMenu *menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
 
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
 
    SetMenuBar( menuBar );

    wxColour color1, color2;
    color1.Set(wxT("#ffffff"));
    color2.Set(wxT("#555555"));

    wxFrame* wxFrame = this;
    wxGridSizer* itemGridSizer1 = new wxGridSizer(0, 3, 0, 0);
    SetSizer(itemGridSizer1);

    wxPanel* leftPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize );
    leftPanel->SetBackgroundColour(color2);
    leftPanel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemGridSizer1->Add(leftPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxVERTICAL);
    leftPanel->SetSizer(itemBoxSizer3);

    wxPanel* itemPanel4 = new wxPanel( leftPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel4->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel4->SetBackgroundColour(color1);
    itemBoxSizer3->Add(itemPanel4, 3, wxGROW|wxALL, 5);

    wxPanel* itemPanel5 = new wxPanel( leftPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel5->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel5->SetBackgroundColour(color1);
    itemBoxSizer3->Add(itemPanel5, 6, wxGROW|wxALL, 5);

    wxPanel* centerPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize);
    centerPanel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    centerPanel->SetBackgroundColour(color2);
    itemGridSizer1->Add(centerPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer7 = new wxBoxSizer(wxVERTICAL);
    centerPanel->SetSizer(itemBoxSizer7);

    wxPanel* itemPanel8 = new wxPanel( centerPanel, -1, wxDefaultPosition, wxDefaultSize);
    itemPanel8->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel8->SetBackgroundColour(color1);
    itemBoxSizer7->Add(itemPanel8, 6, wxGROW|wxALL, 5);

    wxPanel* itemPanel9 = new wxPanel( centerPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel9->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel9->SetBackgroundColour(color1);
    itemBoxSizer7->Add(itemPanel9, 3, wxGROW|wxALL, 5);

    wxPanel* rightPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize );
    rightPanel->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    rightPanel->SetBackgroundColour(color2);
    itemGridSizer1->Add(rightPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer11 = new wxBoxSizer(wxVERTICAL);
    rightPanel->SetSizer(itemBoxSizer11);

    wxPanel* itemPanel12 = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel12->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel12->SetBackgroundColour(color1);
    itemBoxSizer11->Add(itemPanel12, 3, wxGROW|wxALL, 5);

    wxPanel* itemPanel13 = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel13->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel13->SetBackgroundColour(color1);
    itemBoxSizer11->Add(itemPanel13, 3, wxGROW|wxALL, 5);

    wxPanel* itemPanel14 = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    itemPanel14->SetExtraStyle(wxWS_EX_VALIDATE_RECURSIVELY);
    itemPanel14->SetBackgroundColour(color1);
    itemBoxSizer11->Add(itemPanel14, 3, wxGROW|wxALL, 5);
    
    SetMinSize(wxSize(990, 640));

    Centre();

 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
 
    Bind(wxEVT_MENU, &MainFrame::OnHello, this, ID_Hello);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}
 
void MainFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}
 
void MainFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets Hello World example",
                 "About Hello World", wxOK | wxICON_INFORMATION);
}
 
void MainFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}
