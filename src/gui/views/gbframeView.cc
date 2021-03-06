#include "gui/views/gbframeView.h"
#include "sql/gbsql.h"

/**
  * @brief  Constructor to create a GBFrameView. To display the view one must
  *         call the virtual function Show().
  * @param  const wxString& title: Provides a title to the Main Frame Window.
  *         const wxPoint& pos: Provides the position of the Main Frame Window.
  *         const wxSize& size: Provides the size of the Main Frame Window.
  * @retval none.
  */
GBFrameView::GBFrameView(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size) {
	// Create File Menu
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenu *menuStudent = new wxMenu();
	menuStudent->Append(ID_AddStudentMenuSelect, "&Add Student \tCtrl-S", "Add an individual to your GradeBook");
	menuStudent->Append(ID_ModifyStudentMenuSelect, "&Modify Student(s)", "Modify student information in your GradeBook");

	// Create Course Menu
	wxMenu *menuCourse = new wxMenu;
	menuCourse->Append(ID_AddCourseMenuSelect, "&Add Course ", "Add a course to your GradeBook");
	menuCourse->Append(ID_ModifyCourseMenuSelect, "&Modify Course ", "Modify courses in your GradeBook");
	menuCourse->Append(ID_RemoveCourseMenuSelect, "&Remove Course", "Removes a course from your GradeBook");
	// Create Assessment Menu
	wxMenu *menuAssessment = new wxMenu;
	menuAssessment->Append(ID_AddAssessmentMenuSelect, "&Add an Assessment ", "Add an individual Assessment to your GradBook");
	menuAssessment->Append(ID_ModifyAssessmentMenuSelect, "&Modify Assessments ", "Modify Assessment(s) to your GradBook");
	// Create Options Menu
	wxMenu *menuOptions = new wxMenu;
	menuOptions->Append(ID_OptionsMenuSelect, "&User Options");

	wxMenu *menuView = new wxMenu();
	wxMenu *subMenuGradeView = new wxMenu();
	subMenuGradeView->AppendRadioItem(ID_OriginalView, "&Original");
	subMenuGradeView->AppendRadioItem(ID_AdjustedView, "&Adjusted");
	subMenuGradeView->AppendRadioItem(ID_BothView, "&Both");
	menuView->AppendSubMenu(subMenuGradeView, "&Grade Display");

    // Create Help Menu
	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);
	// Adds Menus to MenuBar
	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append( menuFile, "&File" );
	menuBar->Append( menuView, "&View");
	menuBar->Append( menuCourse, "&Course" );
	menuBar->Append( menuStudent, "&Student");
	menuBar->Append( menuAssessment, "&Assessments" );
	menuBar->Append( menuOptions, "&Options");
	menuBar->Append( menuHelp, "&Help" );

	m_pColumnMenu = new wxMenu();
	m_pColumnMenu->Append(ID_LabelGraph, "Graph");
	wxMenu *menuCurves = new wxMenu();
	menuCurves->Append(ID_ShiftMenuSelect, "&Shift Grades");
	m_pColumnMenu->AppendSubMenu(menuCurves, "&Curves");
	m_pColumnMenu->AppendSeparator();
	m_pColumnMenu->Append(ID_LabelDelete, "Delete");

	m_pRowMenu = new wxMenu();
	m_pRowMenu->Append(ID_LabelDelete, "Delete");

	SetMenuBar( menuBar );
	CreateStatusBar();
	SetStatusText( "Welcome to GradeBook!" );

	// Create Frame Sizers
	m_pCourseDropDownListSizer = new wxBoxSizer(wxHORIZONTAL);
	m_pGBFrameSizer = new wxBoxSizer(wxVERTICAL);
	SetClientSize(GBAPPSIZE);
	// Create Base Panel
	m_pGBFramePanel = new wxPanel(this);
	m_pGBFramePanel->SetBackgroundColour(wxColour(char(255),char(255), char(255), char(0) ));//wxWHITE

	// Add CourseDropDownList and GridView to Panel
	m_pCourseComboBox = new wxComboBox(m_pGBFramePanel, ID_CourseDropDownList,  wxEmptyString, wxDefaultPosition, wxDefaultSize);
	m_pCourseComboBox->SetEditable(false);

	m_pGridView = new wxGrid(m_pGBFramePanel, ID_GridView, wxDefaultPosition, wxDefaultSize, 0, "ID_GridView" );

	m_pGradeTable = new GradeTable();
	m_pGridView->SetTable(m_pGradeTable);

	// Apply Sizer to CourseDropDownList and GridView
	m_pCourseDropDownListSizer->Add(m_pCourseComboBox, 1, wxSHAPED |  wxTOP, 10);
	m_pGBFrameSizer->Add(m_pCourseDropDownListSizer, 0 , wxEXPAND | wxALL , 2);
	m_pGBFrameSizer->Add(m_pGridView, 1, wxEXPAND | wxALL, 2);

	// Set GBFrameSizer as primary sizer
	m_pGBFramePanel->SetSizer(m_pGBFrameSizer);

	// Connect Controller
	m_pCon = new GBFrameController(this);

	// Connects Event Handler(s) to Controller
	Bind(wxEVT_GRID_LABEL_RIGHT_CLICK, &GBFrameView::OnLabelRightClick, this, ID_GridView);
	Bind(wxEVT_MENU, &GBFrameController::OnLabelDelete, m_pCon, ID_LabelDelete);
	Bind(wxEVT_MENU, &GBFrameController::OnExit, m_pCon, wxID_EXIT);
	Bind(wxEVT_MENU, &GBFrameController::OnAbout, m_pCon, wxID_ABOUT);
	Bind(wxEVT_MENU, &GBFrameController::AddStudent, m_pCon, ID_AddStudentMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::ModifyStudent, m_pCon, ID_ModifyStudentMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::AddCourse, m_pCon, ID_AddCourseMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::ModifyCourse, m_pCon, ID_ModifyCourseMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::ModifyAssessment, m_pCon, ID_ModifyAssessmentMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::AddAssessment, m_pCon, ID_AddAssessmentMenuSelect);
	Bind(wxEVT_MENU, &GBFrameController::UserOptions, m_pCon, ID_OptionsMenuSelect);
	Bind(wxEVT_COMBOBOX, &GBFrameController::NewCourseSelected, m_pCon, ID_CourseDropDownList);
	Bind(wxEVT_MENU, &GBFrameController::OnRemoveCourse, m_pCon, ID_RemoveCourseMenuSelect);
	Bind(wxEVT_GRID_CELL_CHANGED, &GBFrameController::OnGradeCellChanged, m_pCon, ID_GridView);
	Bind(wxEVT_MENU, &GBFrameController::OnGraphClicked, m_pCon, ID_LabelGraph);
	Bind(wxEVT_MENU, &GBFrameController::OnShiftGradesClicked, m_pCon, ID_ShiftMenuSelect);
	Bind(wxEVT_MENU, &GBFrameView::OnOriginalViewClicked, this, ID_OriginalView);
	Bind(wxEVT_MENU, &GBFrameView::OnAdjustedViewClicked, this, ID_AdjustedView);
	Bind(wxEVT_MENU, &GBFrameView::OnBothViewClicked, this, ID_BothView);
}

void GBFrameView::OnLabelRightClick(wxGridEvent &event) {
	if (event.GetCol() == -1 && event.GetRow() == -1) {
		return;
	}

	if (event.GetCol() >= 0 && event.GetRow() == -1) {
		m_pGridView->SelectCol(event.GetCol());

		m_pGridView->PopupMenu(m_pColumnMenu, event.GetPosition().x, event.GetPosition().y);
	} else if (event.GetCol() == -1 && event.GetRow() >= 0) {
		m_pGridView->SelectRow(event.GetRow());

		m_pGridView->PopupMenu(m_pRowMenu, event.GetPosition().x, event.GetPosition().y);
	}
}

void GBFrameView::OnOriginalViewClicked(wxCommandEvent &event) {
	m_pGradeTable->UpdateView(ORIGINAL);
}

void GBFrameView::OnAdjustedViewClicked(wxCommandEvent &event) {
	m_pGradeTable->UpdateView(ADJUSTED);
}

void GBFrameView::OnBothViewClicked(wxCommandEvent &event) {
	m_pGradeTable->UpdateView(BOTH);
}

GradeTable::GradeTable()
	:	wxGridTableBase() {
	m_gradeView = ORIGINAL;
}

void GradeTable::UpdateView(GradeView view) {
	m_gradeView = view;

	if (GetView()) {
		wxGridTableMessage msg(this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES);

		GetView()->ProcessTableMessage(msg);
	}
}

void GradeTable::ForceRefresh() {
	if (GetView()) {
		wxGridTableMessage msg(this, wxGRIDTABLE_REQUEST_VIEW_GET_VALUES);

		GetView()->ProcessTableMessage(msg);
	}
}

int GradeTable::GetNumberRows() {
	return m_rows.size();
}

int GradeTable::GetNumberCols() {
	return m_cols.size();
}

wxString GradeTable::GetValue(int row, int col) {
	if (m_gradeView == ORIGINAL) {
		return m_grades[row][col].Value();
	} else if (m_gradeView == ADJUSTED) {
		return m_grades[row][col].AdjValue();
	} else {
		return wxString::Format("%s - %s", m_grades[row][col].Value(), m_grades[row][col].AdjValue());
	}
}

void GradeTable::SetValue(int row, int col, const wxString &value) {
	if (m_grades[row][col].Id().IsSameAs("-1")) {
		Grade g;

		g.SetValue(value);

		m_grades[row][col] = g;
	} else {
		m_grades[row][col].SetValue(value);
	}
}

void GradeTable::Clear() {
	if (m_rows.size() > 0) {
		if (GetView()) {
			wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_DELETED, 0, m_rows.size());

			GetView()->ProcessTableMessage(msg);
		}

		m_rows.clear();
	}

	if (m_cols.size() > 0) {
		if (GetView()) {
			wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_DELETED, 0, m_cols.size());

			GetView()->ProcessTableMessage(msg);
		}

		m_cols.clear();
	}

	if (m_grades.size() > 0) {
		for (int x = 0; x < m_grades.size(); ++x) {
			m_grades[x].clear();
		}

		m_grades.clear();
	}
}

wxString GradeTable::GetRowLabelValue(int row) {

  long SavedStudentNameFormat = Config::Instance()->GetStudentFormat(wxT("StudentNameDisplayFormat"), SavedStudentNameFormat);

  if(SavedStudentNameFormat == 0){

	return wxString::Format("%s, %s", m_rows[row].Last(), m_rows[row].First());
  }
  else if(SavedStudentNameFormat == 1){

    return wxString::Format("%s, %s", m_rows[row].First(), m_rows[row].Last());
  }
  else if(SavedStudentNameFormat == 2){

    return wxString::Format("%s", m_rows[row].StudentId());
  }

}

wxString GradeTable::GetColLabelValue(int col) {
	return m_cols[col].Title();
}

void GradeTable::AddStudent(int index, const Student &s) {
	m_rows.push_back(s);

	m_grades.push_back(vector<Grade>(m_cols.size()));

	if (GetView()) {
		wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_ROWS_APPENDED, 1);

		GetView()->ProcessTableMessage(msg);
	}
}

void GradeTable::AddAssessment(int index, const Assessment &a) {
	m_cols.push_back(a);

	if (GetView()) {
		wxGridTableMessage msg(this, wxGRIDTABLE_NOTIFY_COLS_APPENDED, 1);

		GetView()->ProcessTableMessage(msg);
	}
}

void GradeTable::AddGrade(int row, int col, const Grade &g) {
	m_grades[row][col] = g;
}

Student &GradeTable::GetStudent(int index) {
	return m_rows[index];
}

Assessment &GradeTable::GetAssessment(int index) {
	return m_cols[index];
}

Grade &GradeTable::GetGrade(int row, int col) {
	return m_grades[row][col];
}
