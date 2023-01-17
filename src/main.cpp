#include <wx/wx.h>
#include <wx/filepicker.h>
#include <wx/dirctrl.h>
#include <wx/apptrait.h>
#include <wx/combobox.h>
#include <wx/msgdlg.h>


#include "NetworkLayout.hpp"
#include "writerUMC.hpp"
#include "writerSimple.hpp"
#include "ParserXML.hpp"
#include <stdexcept>
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <memory>

#include <unordered_map>
#include <string>
#include <cstring>
#include <exception>
#include <direct.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sstream>


using namespace rapidxml;
using namespace std;


int ParserXML::count = 0;

std::unique_ptr<writer> writer::write(fileType type){
    if(type == 0) {
        auto uniqueP = make_unique<writerUMC>();
        return uniqueP;
    }
    if(type == 1){
        auto uniqueP = make_unique<writerSimple>();
        return uniqueP;
    }
    return nullptr;
}

void createFolder(const std::string &s1,std::string &outputFile, std::stringstream &ss){
    try {
        outputFile = outputFile + s1;
        outputFile +=  + "/";
        ss.str("");
        ss.clear();
        ss <<  outputFile;
        int folder = _mkdir(ss.str().c_str());
        if(folder == 0) std::cout << "Created " << ss.str() << " success\n";
        }
    catch (const std::exception &e){
        std::cerr << e.what();
    }
}

bool run(string input, string outputFile,int file, int train,int combined,int select,int route1, int route2){
    outputFile += "\\";
    auto pXML = make_unique<ParserXML>(input);
    std::stringstream ss;
    createFolder(pXML->SplitFilename(input),outputFile,ss);
    if(file == 0){
        auto obj = writer::write(UMCFile);
        createFolder("UMC",outputFile,ss);
        if(train == 0){
            if(combined == 1){
                createFolder("SingleRoutesCombined",outputFile, ss);
                Interlock il_comb = pXML->getIl().routeCombiner(pXML->getIl());
                pXML->setIl(il_comb);  
                obj->writeFile(outputFile,pXML,train,0);
                pXML->setCount(0);
                return true;
            }
            else{
                if(select == 0 ){
                    createFolder("SingleRoute",outputFile,ss);
                    obj->writeFile(outputFile,pXML,train,select);
                    pXML->setCount(0);
                    return true;
                }else{
                    createFolder("SingleRouteSelected",outputFile,ss);
                    obj->writeFile(outputFile,pXML,train,select,route1); 
                    pXML->setCount(0);
                    return true;
                }
            }
        }else if(train == 1){
            if(select == 0){
                createFolder("DoubleRoute",outputFile, ss);
                obj->writeFile(outputFile,pXML,train,select);
                pXML->setCount(0);
                return true;
            }else{
                createFolder("DoubleRouteSelected",outputFile,ss);
                obj->writeFile(outputFile,pXML,train,select,route1,route2);
                pXML->setCount(0);
                return true;
            }
        } 
    }
    if(file == 1){
        auto obj = writer::write(SimpleFile);
        createFolder("Simple",outputFile,ss);
        obj->writeFile(outputFile,pXML,1);
        pXML->setCount(0);
        return true;
    }
    return false;    
}

bool CheckPickersFilled(wxFilePickerCtrl* file_picker, wxDirPickerCtrl* dir_picker) {
    return !file_picker->GetPath().empty() && !dir_picker->GetPath().empty();
}

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
    void OnDirChanged(wxFileDirPickerEvent& event);



    wxTextCtrl* m_inputField;
    wxTextCtrl* m_outputField;
    wxFilePickerCtrl *inputFilePicker;
    wxDirPickerCtrl *outputDirPicker;

    wxButton*  routeButton;
    wxButton*  runButton;

    wxRadioBox* radioBoxType;
    wxRadioBox* radioBoxTrain;
    wxRadioBox* radioBoxCombined;
    wxRadioBox* radioBoxSelected;

    wxComboBox *list1, *list2;
    wxGauge* m_gauge;

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
    MyFrame* frame = new MyFrame("PARSER XML", wxPoint(50, 50), wxSize(900, 500));
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
    runButton = new wxButton(panel, wxID_ANY, "Run");
    wxButton* clearButton = new wxButton(panel, wxID_ANY, "Clear");
    routeButton = new wxButton(panel, wxID_ANY, "Calculate Route");
    routeButton->Disable();



    //  Create a radioBox for selecting the number of trains
    wxString radioBoxChoicesType[] = { wxT("UMC file"), wxT("Simple File") };
	int radioBox2Choices = sizeof( radioBoxChoicesType ) / sizeof( wxString );
	radioBoxType = new wxRadioBox( panel, wxID_ANY, wxT("Type?"), wxDefaultPosition, wxDefaultSize, radioBox2Choices, radioBoxChoicesType, 1, wxRA_SPECIFY_COLS );
	radioBoxType->SetSelection( 0 );

    wxString radioBoxChoicesTrain[] = { wxT("1"), wxT("2") };
	int radioBoxNChoices = sizeof( radioBoxChoicesTrain ) / sizeof( wxString );
	radioBoxTrain = new wxRadioBox( panel, wxID_ANY, wxT("Train?"), wxDefaultPosition, wxDefaultSize, radioBoxNChoices, radioBoxChoicesTrain, 1, wxRA_SPECIFY_COLS );
	radioBoxTrain->SetSelection( 0 );

    wxString m_choice1Choices[] = { wxT("No"), wxT("Yes") };
	int m_choice1NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
    radioBoxCombined = new wxRadioBox( panel, wxID_ANY, wxT("Combined routes?"), wxDefaultPosition, wxDefaultSize, m_choice1NChoices, m_choice1Choices, 1, wxRA_SPECIFY_COLS );
	radioBoxCombined->SetSelection( 0 );

	wxString m_choice2Choices[] = { wxT("No"), wxT("Yes") };
    int m_choice2NChoices = sizeof( m_choice1Choices ) / sizeof( wxString );
    radioBoxSelected = new wxRadioBox( panel, wxID_ANY, wxT("Choosen routes?"), wxDefaultPosition, wxDefaultSize, m_choice2NChoices, m_choice2Choices, 1, wxRA_SPECIFY_COLS );
	radioBoxSelected->SetSelection( 0 );

    // Create the two lists
    list1 = new wxComboBox( panel, wxID_ANY, wxT("Route1"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
    list2 = new wxComboBox( panel, wxID_ANY, wxT("Route2"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );

    m_gauge = new wxGauge(panel, wxID_ANY, 100, wxPoint(10, 10), wxSize(250, 25));
    m_gauge->Disable();

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
    vbox->Add(m_gauge,0,wxALIGN_BOTTOM | wxALIGN_CENTER | wxALL, 5 );

    // Connect the button events to their event handlers
    runButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnRun, this);
    routeButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnCalculateRoute, this);
    clearButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MyFrame::OnClean, this);

    Bind(wxEVT_MENU, &MyFrame::OnQuit, this, ID_Quit);
    Bind(wxEVT_MENU, &MyFrame::OnAbout, this, ID_About);
    Bind(wxEVT_COMMAND_FILEPICKER_CHANGED, &MyFrame::OnFileChanged, this);

    this->Connect(inputFilePicker->GetId(), wxEVT_COMMAND_FILEPICKER_CHANGED, wxFileDirPickerEventHandler(MyFrame::OnFileChanged));
    this->Connect(outputDirPicker->GetId(), wxEVT_COMMAND_DIRPICKER_CHANGED, wxFileDirPickerEventHandler(MyFrame::OnDirChanged));

    radioBoxType->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxType, this);
    radioBoxTrain->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxTrain, this);
    radioBoxSelected->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxSelected, this);
    radioBoxCombined->Bind(wxEVT_RADIOBOX, &MyFrame::OnRadioBoxCombined, this);

    list1->Disable();
    list2->Disable();
    runButton->Disable();
    
    panel->SetSizer(vbox);
    Layout();
    Centre( wxBOTH );
}

void MyFrame::OnRun(wxCommandEvent& event)
{
    std::string input = inputFilePicker->GetPath().ToStdString();
    std::string output = outputDirPicker->GetPath().ToStdString();
    int train = radioBoxTrain->GetSelection();
    int type = radioBoxType->GetSelection();
    int combinedSelection = radioBoxCombined->IsEnabled() ? radioBoxCombined->GetSelection() : 0;
    int selectedSelection = radioBoxSelected->IsEnabled() ? radioBoxSelected->GetSelection() : 0;
    int route1 = list1->GetSelection();
    int route2 = list2->GetSelection();
    bool success = false;
    if(train == 1 && radioBoxSelected->IsEnabled() && route1 == route2 && radioBoxSelected->GetSelection() == 1){
        wxMessageDialog dialog(nullptr, "Select two different routes", "Error", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        return;
    }else{
        m_gauge->Enable();
        success  = run(input,
            output,
            type,
            train,
            combinedSelection,
            selectedSelection,
            route1,
            route2); 
            m_gauge->SetValue(100);
    }if(success){
        wxMessageDialog dialog(nullptr, "The process completed successfully!", "Success", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        
    }
    else{
        wxMessageDialog dialog(nullptr, "The process not completed! \n Retry with different parameters", "Fail", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
    }
    m_gauge->SetValue(0);
    m_gauge->Disable();
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
        runButton->Enable();
    }
    else if (selection == 1)
    {
        if(radioBoxTrain->GetSelection() == 1)
            list2->Enable();
        list1->Enable();
        runButton->Disable();
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
    list1->Clear();
    list2->Clear();
    for (int i = 0; i < size; i++)
    {
        list1->Append(wxString::Format("%d", i+1));
        list2->Append(wxString::Format("%d", i+1));
    }
    if(!list1->IsListEmpty() && !list2->IsListEmpty()){
        list1->SetSelection(0);
        list2->SetSelection(1);
        runButton->Enable();
    }else{
        wxMessageDialog dialog(nullptr, "Error: Routes not correctly loaded.", "Fail", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
    }
    
}

void MyFrame::OnFileChanged(wxFileDirPickerEvent& event)
{
    wxFileName inputFile(inputFilePicker->GetPath());
    if (!inputFile.FileExists()) {
        wxMessageDialog dialog(nullptr, "Error: input file does not exist.", "Fail", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        inputFilePicker->SetPath("");
        return;
    }
    if (inputFile.GetExt() != "xml") {
        wxMessageDialog dialog(nullptr, "Error: input file is not an xml file.", "Fail", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        inputFilePicker->SetPath("");
        return;
    }
    std::ifstream fileInput(inputFilePicker->GetPath().ToStdString());
    if (!fileInput.good()) {
        wxMessageDialog dialog(nullptr, "Error: input file cannot be opened.", "Fail", wxOK | wxICON_INFORMATION);
        dialog.ShowModal();
        inputFilePicker->SetPath("");
        return;
    }
    runButton->Enable(CheckPickersFilled(inputFilePicker,outputDirPicker));
    
}

void MyFrame::OnDirChanged(wxFileDirPickerEvent& event) {
    routeButton->Enable(CheckPickersFilled(inputFilePicker,outputDirPicker));
    runButton->Enable(CheckPickersFilled(inputFilePicker,outputDirPicker));
}

