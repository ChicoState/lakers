#include "gbapp.h"
#include "gui/views/gbframeView.h"
#include "sql/gbsql.h"
#include <wx/fileconf.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <iostream>

using namespace std;
#ifdef __GUI__
IMPLEMENT_APP(GBApp)
#endif

bool GBApp::OnInit() {

  wxString ini_filename = wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + "GbUserOptions.INI";
  wxString ExecutablePath = wxStandardPaths::Get().GetExecutablePath();
  wxString DatabasePath;
  wxFileConfig *config;

  ifstream InFile(ini_filename.c_str());

  if(!InFile){

    config = new wxFileConfig( "", "", ini_filename);
    ExecutablePath.Replace("gbapp.exe","");
    DatabasePath = ExecutablePath + "gb.db";
    config->Write( wxT("/gbDataBasePath"),  DatabasePath );
    config->Write( wxT("/StudentNameDisplayFormat"), 0 );
    config->Flush();
    delete config;
  }
  else{

    config = new wxFileConfig( "", "", ini_filename);
    DatabasePath = config->Read(wxT("gbDataBasePath"), DatabasePath) ;
    delete config;
    InFile.close();
  }

  GBSql::Instance()->Initialize(DatabasePath);

	if (!wxApp::OnInit()) {
		return false;
	}

  GBFrameView *pGBBase = new GBFrameView("Grade Book", wxPoint(0,0), GBAPPSIZE);

  pGBBase->Show();

  SetTopWindow(pGBBase);

    return true;
}

int GBApp::OnExit() {
  GBSql::Instance()->Close();

  return 0;
}

void GBApp::OnInitCmdLine(wxCmdLineParser &parser) {
	parser.AddOption("d", "dummy");
}

bool GBApp::OnCmdLineParsed(wxCmdLineParser &parser) {
	wxString database;

	if (parser.Found("d", &database)) {
		GBSql::Instance()->Close();
		GBSql::Instance()->Initialize(database);
		GBSql::Instance()->PopulateDummy();
	}	

	return true;
}
