#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/dirctrl.h>
#include <wx/apptrait.h>
#include <wx/combobox.h>
#include "ParserXML.hpp"



int ParserXML::count = 0;

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnRun(wxCommandEvent& event);
    void OnClean(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnRadioBoxType(wxCommandEvent& event);
    void OnRadioBoxTrain(wxCommandEvent& event);
    void OnRadioBoxSelected(wxCommandEvent& event);
    void OnRadioBoxCombined(wxCommandEvent& event);
    void OnCalculateRoute(wxCommandEvent& event);
    void OnFileChanged(wxFileDirPickerEvent& event);


    wxTextCtrl* m_inputField;
    wxTextCtrl* m_outputField;
    wxFilePickerCtrl *inputFilePicker;
    wxDirPickerCtrl *outputDirPicker;

    wxButton*  routeButton;

    wxRadioBox* radioBoxType;
    wxRadioBox* radioBoxTrain;
    wxRadioBox* radioBoxCombined;
    wxRadioBox* radioBoxSelected;

    wxComboBox *list1, *list2;

};

enum
{
    ID_Quit = 1,
    ID_About,
    ID_Run,
    ID_Clean,
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame* frame = new MyFrame("PARSER XML", wxPoint(50, 50), wxSize(900, 400));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Set up the menu bar
    wxMenu *menuFile = new wxMenu;
    menuFile->Append(ID_About, "&About");
    menuFile->AppendSeparator();
    menuFile->Append(ID_Quit, "E&xit");
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    SetMenuBar(menuBar);

    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");
    
    // Create a panel to hold the input and output fields and buttons
    wxPanel* panel = new wxPanel(this, wxID_ANY);
  
    // Create the input and output fields
    wxStaticText *inputLabel = new wxStaticText(panel, wxID_ANY, "Input File:");
    inputLabel->SetFont( wxFont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
    
    inputFilePicker = new wxFilePickerCtrl(panel, wxID_ANY);
    wxStaticText *outputLabel = new wxStaticText(panel, wxID_ANY, "Output Folder:");
    outputLabel->SetFont( wxFont( 9, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );
    outputDirPicker = new wxDirPickerCtrl(panel, wxID_ANY);

    // Create the "Run" and "Clear" buttons
    wxButton* runButton = new wxButton(panel, wxID_ANY, "Run");
    wxButton* clearButton = new wxButton(panel, wxID_ANY, "Clear");
    routeButton = new wxButton(panel, wxID_ANY, "Calculate Route");
    routeButton->Disable();



    //  Create a radioBox for selecting the number of trains
    wxString radioBoxChoicesType[] = { wxT("UMC file"), wxT("Simple File") };
	int radioBox2Choices = sizeof( radioBoxChoicesType ) / sizeof( wxString );
	radioBoxType = new wxRadioBox( panel, wxID_ANY, wxT("Type of file?"), wxDefaultPosition, wxDefaultSize, radioBox2Choices, radioBoxChoicesType, 1, wxRA_SPECIFY_COLS );
	radioBoxType->SetSelection( 0 );

    wxString radioBoxChoicesTrain[] = { wxT("1"), wxT("2") };
	int radioBoxNChoices = sizeof( radioBoxChoicesTrain ) / sizeof( wxString );
	radioBoxTrain = new wxRadioBox( panel, wxID_ANY, wxT("How many train?"), wxDefaultPosition, wxDefaultSize, radioBoxNChoices, radioBoxChoicesTrain, 1, wxRA_SPECIFY_COLS );
	radioBoxTrain->SetSelection( 0 );

    wxString m_choice1Choices[] = { wxT("No"), wxT("Yes") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
    radioBoxCombined = new wxRadioBox( panel, wxID_ANY, wxT("Do you wanna combined routes?"), wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 1, wxRA_SPECIFY_COLS );
	radioBoxCombined->SetSelection( 0 );

	wxString m_choice2Choices[] = { wxT("No"), wxT("Yes") };
    int m_choice2NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
    radioBoxSelected = new wxRadioBox( panel, wxID_ANY, wxT("Do you wanna choose the routes?"), wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 1, wxRA_SPECIFY_COLS );
	radioBoxSelected->SetSelection( 0 );

    // Create the two lists
    list1 = new wxComboBox( panel, wxID_ANY, wxT("Route1"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
    list2 = new wxComboBox( panel, wxID_ANY, wxT("Route2"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );

    // Add some items to the lists

    CreateStatusBar();
    SetStatusText("Welcome to the XML parser!");

    // Create a vbox and hbox to layout the input and output fields and buttons
    wxBoxSizer* hbox = new wxBoxSizer( wxHORIZONTAL );

    wxBoxSizer* vbox = new wxBoxSizer( wxVERTICAL );

    vbox->Add(inputLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(m_inputField, 1, wxEXPAND | wxALL, 5);
    vbox->Add(inputFilePicker, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(outputLabel, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);
    vbox->Add(m_outputField, 1, wxEXPAND | wxALL, 5);
    vbox->Add(outputDirPicker, 0, wxEXPAND | wxLEFT | wxRIGHT | wxTOP, 10);

    hbox->Add(radioBoxType, 0, wxALL, 10 );
	hbox->Add(radioBoxTrain, 0, wxALL, 10 );
    hbox->Add( radioBoxCombined, 0, wxALL, 10 );
    hbox->Add( radioBoxSelected, 0, wxALL, 10 );
    hbox->Add(list1, 1, wxALL,10);
    hbox->Add(list2, 1, wxALL,10);


    vbox->Add(hbox,1,wxEXPAND,10 );

    vbox->Add(routeButton, 0, wxALIGN_BOTTOM | wxALIGN_CENTER | wxALL, 5 );
    vbox->Add(runButton, 0, wxALIGN_BOTTOM | wxALIGN_CENTER | wxALL, 5 );
    vbox->Add(clearButton, 0, wxALIGN_BOTTOM | wxALIGN_CENTER | wxALL, 5 );

    // Connect the button events to their event handlers
    runButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnRun, this);
    routeButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnCalculateRoute, this);
    clearButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnClean, this);

    Bind(wxEVT_MENU, &MyFrame::OnQuit, this, ID_Quit);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, ID_About);
    Bind(wxEVT_COMMAND_FILEPICKER_CHANGED, &MyFrame::OnFileChanged, this);

    radioBoxType->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxType, this);
    radioBoxTrain->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxTrain, this);
    radioBoxSelected->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxSelected, this);
    radioBoxCombined->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxCombined, this);

    list1->Disable();
    list2->Disable();

    panel->SetSizer(vbox);
    Layout();
    Centre( wxBOTH );


}

void MyFrame::OnRun(wxCommandEvent& event)
{
    // TODO: Add code to run the input and output fields here

}

void MyFrame::OnClean(wxCommandEvent& event)
{
    inputFilePicker->SetPath("");
    outputDirPicker->SetPath("");
    radioBoxTrain->SetSelection(0);
    radioBoxType->SetSelection(0);
    radioBoxSelected->SetSelection(0);
    radioBoxCombined->SetSelection(0);
    radioBoxTrain->Enable();
    list1->Disable();
    list2->Disable();
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a XML Parser that convert XML train configuration in classes in UML language.", "About XML Parser", wxOK | wxICON_INFORMATION);
}


void MyFrame::OnRadioBoxType(wxCommandEvent& event)
{
    int selection = event.GetInt();
    if (selection == 0)
    {
        // Show choice list 1 and hide choice list 2
        radioBoxCombined->Enable();
        radioBoxTrain->Enable();
        radioBoxSelected->Enable();
        radioBoxTrain->SetSelection( 0 );
        radioBoxSelected->SetSelection( 0 );
        radioBoxCombined->SetSelection( 0 );
    }
    else if (selection == 1)
    {
        // Show choice list 2 and hide choice list 1
        radioBoxTrain->Disable();
        radioBoxCombined->Disable();
        radioBoxSelected->Disable();
        list1->Disable();
        list2->Disable();
    }
}
void MyFrame::OnRadioBoxTrain(wxCommandEvent& event)
{
    int selection = event.GetInt();
    if (selection == 0)
    {
        // Show choice list 1 and hide choice list 2
        radioBoxSelected->Enable();
        radioBoxCombined->Enable();
        radioBoxCombined->SetSelection( 0 );
        // radioBoxSelected->SetSelection( 0 );
        if(radioBoxSelected->GetSelection() == 1)
            list2->Disable();
        
    }
    else if (selection == 1)
    {
        // Show choice list 2 and hide choice list 1
        radioBoxCombined->Disable(); 
        radioBoxSelected->Enable();   
        if(radioBoxSelected->GetSelection() == 0 ){
            list1->Disable();
            list2->Disable();    
        }else{
            list1->Enable();
            list2->Enable();
        }
    }
}
void MyFrame::OnRadioBoxSelected(wxCommandEvent& event)
{
    int selection = event.GetInt();
    if (selection == 0)
    {
        list1->Disable();
        list2->Disable();
    }
    else if (selection == 1)
    {
        if(radioBoxTrain->GetSelection() == 1)
            list2->Enable();
        list1->Enable();
    }
}
void MyFrame::OnRadioBoxCombined(wxCommandEvent& event)
{
    int selection = event.GetInt();
    if (selection == 0)
    {
        radioBoxSelected->Enable();  

    }
    else if (selection == 1)
    {
        radioBoxSelected->Disable(); 
        list1->Disable();
        list2->Disable();
    }
}
void MyFrame::OnCalculateRoute(wxCommandEvent& event)
{
    ParserXML parser;
    string path = inputFilePicker->GetPath().ToStdString();
    int size = parser.countRoutes(path);
    cout << size << endl;
    for (int i = 0; i < size; i++)
    {
        list1->Append(wxString::Format("%d", i+1));
        list2->Append(wxString::Format("%d", i+1));
    }


}

void MyFrame::OnFileChanged(wxFileDirPickerEvent& event)
{
    routeButton->Enable();
}