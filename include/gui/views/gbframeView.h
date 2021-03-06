#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "gui/controllers/gbframeController.h"
#include "data/config.h"
#include <wx/string.h>
#define GBAPPSIZE wxSize(800,600)
#define GBDIALOGSIZE wxSize(350,450)


enum GradeView {
	ORIGINAL,
	ADJUSTED,
	BOTH
};

class GradeTable : public wxGridTableBase {
	public:
		GradeTable();

		void UpdateView(GradeView view);
		void ForceRefresh();

		virtual int GetNumberRows();
		virtual int GetNumberCols();

		virtual wxString GetValue(int row, int col);
		virtual void SetValue(int row, int col, const wxString &value);

		virtual void Clear();

		virtual wxString GetRowLabelValue(int row);
		virtual wxString GetColLabelValue(int col);

		void AddStudent(int index, const Student &s);
		void AddAssessment(int index, const Assessment &s);
		void AddGrade(int row, int col, const Grade &g);

		Student &GetStudent(int index);
		Assessment &GetAssessment(int index);
		Grade &GetGrade(int row, int col);

	private:
		GradeView m_gradeView;
		vector<Student> m_rows;
		vector<Assessment> m_cols;
		vector<vector<Grade> > m_grades;
        wxString ini_filename;
        wxFileConfig *config;
        long SavedStudentNameFormat;
};

class GBFrameView : public wxFrame {
  public:
    GBFrameView(const wxString& title, const wxPoint& pos, const wxSize& size);
    wxBoxSizer 				*m_pCourseDropDownListSizer;
    wxBoxSizer 				*m_pGBFrameSizer;
  	wxComboBox 				*m_pCourseComboBox;
    wxPanel					*m_pGBFramePanel;
    wxGrid					*m_pGridView;
    wxDialog				*m_pGBDialog;
    wxPanel					*m_pGBDialogPanel;
	GradeTable				*m_pGradeTable;
	wxMenu 					*m_pColumnMenu;
	wxMenu					*m_pRowMenu;

	protected:
    GBFrameController			*m_pCon;

	private:
		void OnLabelRightClick(wxGridEvent &event);
		void OnOriginalViewClicked(wxCommandEvent &event);
		void OnAdjustedViewClicked(wxCommandEvent &event);
		void OnBothViewClicked(wxCommandEvent &event);
};


enum {
	ID_AddCourseMenuSelect = 1,
	ID_CourseDropDownList = 2,
	ID_GradeBookPanel = 3,
	ID_GridView = 4,
	ID_AddAssessmentMenuSelect = 5,
	ID_ModifyAssessmentMenuSelect = 6,
	ID_ModifyAssignmentToolBarButton = 7,
	ID_GBDialogPanel = 8,
	ID_AddStudentMenuSelect = 9,
	ID_ModifyStudentMenuSelect = 10,
	ID_LabelDelete,
	ID_LabelGraph,
	ID_RemoveCourseMenuSelect,
	ID_OptionsMenuSelect,
	ID_ModifyCourseMenuSelect,
	ID_ShiftMenuSelect,
	ID_OriginalView,
	ID_AdjustedView,
	ID_BothView,
};
