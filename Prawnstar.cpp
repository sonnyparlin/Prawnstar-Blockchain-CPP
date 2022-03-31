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

    wxColour color1, color2, textColor;
    color1.Set(wxT("#ffffff"));
    color2.Set(wxT("#555555"));
    textColor.Set(wxT("#000000"));

    wxFrame* wxFrame = this;
    wxGridSizer* mainGridSizer = new wxGridSizer(0, 3, 0, 0);
    SetSizer(mainGridSizer);

    wxPanel* leftPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize );
    leftPanel->SetBackgroundColour(color2);
    leftPanel->SetForegroundColour( textColor );
    mainGridSizer->Add(leftPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* leftPanelSizer = new wxBoxSizer(wxVERTICAL);
    leftPanel->SetSizer(leftPanelSizer);

    wxString msg("\n NODE STATUS:\n"
                " ID: de28f2d9-ad6d-81af-1625-42da8394ed7c\n"
                " Status: Up\n"
                " Blocks forged by this node: 24\n"
                " Node IP: 192.168.1.145\n"
                " Node Port: 10001\n"
                " Node API Port: 8001\n"
                " Master Node: 192.168.1.15\n"
                );
    wxStaticText* m_text = new wxStaticText( leftPanel, wxID_ANY, msg );
    m_text->SetBackgroundColour(color1);
    wxFont font = m_text->GetFont();
    font.SetPointSize(12);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    m_text->SetFont(font);
    leftPanelSizer->Add(m_text, 3, wxGROW|wxALL, 5);

    // wxPanel* lTop = new wxPanel( leftPanel, -1, wxDefaultPosition, wxDefaultSize );
    // lTop->SetBackgroundColour(color1);
    wxButton* button = new wxButton(leftPanel, -1, L"Start Server");
    wxButton* button2 = new wxButton(leftPanel, -1, L"Configure Server");
    // button->SetBackgroundColour(color1);
    // button->SetForegroundColour(textColor);
    wxBoxSizer *hbox1 = new wxBoxSizer(wxHORIZONTAL);

    hbox1->Add(button);
    hbox1->Add(button2);
    leftPanelSizer->Add(hbox1, 0, wxALIGN_CENTRE, 1);
    // leftPanelSizer->Add(lTop, 3, wxGROW|wxALL, 5);

    wxPanel* lBot = new wxPanel( leftPanel, -1, wxDefaultPosition, wxDefaultSize );
    lBot->SetBackgroundColour(color1);
    leftPanelSizer->Add(lBot, 6, wxGROW|wxALL, 5);

    wxPanel* centerPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize);
    centerPanel->SetBackgroundColour(color2);
    mainGridSizer->Add(centerPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* centerPanelSizer = new wxBoxSizer(wxVERTICAL);
    centerPanel->SetSizer(centerPanelSizer);

    wxPanel* cTop = new wxPanel( centerPanel, -1, wxDefaultPosition, wxDefaultSize);
    cTop->SetBackgroundColour(color1);
    centerPanelSizer->Add(cTop, 6, wxGROW|wxALL, 5);

    wxPanel* cBot = new wxPanel( centerPanel, -1, wxDefaultPosition, wxDefaultSize );
    cBot->SetBackgroundColour(color1);
    centerPanelSizer->Add(cBot, 3, wxGROW|wxALL, 5);

    wxPanel* rightPanel = new wxPanel( wxFrame, -1, wxDefaultPosition, wxDefaultSize );
    rightPanel->SetBackgroundColour(color2);
    mainGridSizer->Add(rightPanel, 0, wxGROW|wxALL, 5);

    wxBoxSizer* rightPanelSizer = new wxBoxSizer(wxVERTICAL);
    rightPanel->SetSizer(rightPanelSizer);

    wxPanel* rTop = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    rTop->SetBackgroundColour(color1);
    rightPanelSizer->Add(rTop, 3, wxGROW|wxALL, 5);

    wxPanel* rMid = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    rMid->SetBackgroundColour(color1);
    rightPanelSizer->Add(rMid, 3, wxGROW|wxALL, 5);

    wxPanel* rBot = new wxPanel( rightPanel, -1, wxDefaultPosition, wxDefaultSize );
    rBot->SetBackgroundColour(color1);
    rightPanelSizer->Add(rBot, 3, wxGROW|wxALL, 5);
    
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
