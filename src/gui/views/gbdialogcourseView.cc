#include "gui/views/gbdialogcourseView.h"
#include "sql/gbsql.h"

/**
  * @brief  Constructor to create a GBDialogCourseView. To display the view one must
  *         call the virtual function ShowModal().
  * @param  wxWindow *parent: The parent of the dialog.
  * @retval none.
  */
GBDialogCourseView::GBDialogCourseView(wxWindow *parent)
  : wxDialog(parent, wxID_ANY, wxT("Add Course"), wxDefaultPosition, GB_COURSE_DIALOGSIZE) {

    wxStaticBox         *CourseNameStaticBox;
	wxStaticBox         *ImportStudentCSVStaticBox;
	wxStaticBox         *SelectStudentsToImportStaticBox;

	// Create Dialog
	SetClientSize(GB_COURSE_DIALOGSIZE);
	// Create Panel
	m_pGBDialogPanel = new wxPanel(this, wxID_ANY, wxPoint(0,0), GB_COURSE_DIALOGSIZE, wxTAB_TRAVERSAL, "ID_GBDialogPanel");
	// Create Static Boxes
	CourseNameStaticBox = new wxStaticBox(m_pGBDialogPanel, wxID_ANY, "Course Name", wxPoint(10,10), wxSize(325,50), 0,"Course Name");
	SelectStudentsToImportStaticBox = new wxStaticBox(m_pGBDialogPanel, wxID_ANY, "Select Students To Import", wxPoint(10,210), wxSize(325,200), 0,"Course Number");
	ImportStudentCSVStaticBox =  new wxStaticBox(m_pGBDialogPanel, wxID_ANY, "Import Students (File: *.csv)", wxPoint(10,140), wxSize(325,50), 0,"Import Students");
	// Create TextCtrl, FileCtrl, and CheckListBox
	m_pCourseNameTextCtrl	= new wxTextCtrl(m_pGBDialogPanel, wxID_ANY, wxEmptyString, wxPoint(20,30), wxSize(300,25), wxTE_CAPITALIZE, wxDefaultValidator, "CourseNameTextCtrl");
	m_pSelectFileLocationCtrl = new wxFilePickerCtrl(m_pGBDialogPanel, ID_FileSelect, wxEmptyString, "Select .csv file", "*.csv", wxPoint(20,160),  wxDefaultSize, wxFLP_OPEN | wxFLP_FILE_MUST_EXIST, wxDefaultValidator, "ImportStudentCSVFilePickerCtrl");
	m_pcsvFileViewListBox = new  wxCheckListBox (m_pGBDialogPanel, ID_StudentSelection,   wxPoint(10,225), wxSize(325,185));
	// Create Button
	m_pImportButton = new wxButton(m_pGBDialogPanel, ID_AddButton, "Add", wxPoint(20, 430),wxDefaultSize, 0,wxDefaultValidator, "ImportButton");
	// Connect Controller
	m_pController = new GBDialogCourseController(this);
	// Connect Events Handler(s) to Controller
    Bind(wxEVT_CLOSE_WINDOW, &GBDialogCourseController::DialogIsBeingClosed, m_pController);
	m_pSelectFileLocationCtrl->Bind(wxEVT_FILEPICKER_CHANGED, &GBDialogCourseController::FileHasBeenSelected, m_pController);
	m_pImportButton->Bind(wxEVT_BUTTON, &GBDialogCourseController::AddButtonWasClicked, m_pController);
}

GBDialogCourseView::~GBDialogCourseView(){
	delete m_pController, m_pGBDialogPanel, m_pCourseNameTextCtrl,
			m_pSelectFileLocationCtrl, m_pImportButton;
}
