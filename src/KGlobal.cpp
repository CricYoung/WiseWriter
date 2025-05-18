//==== Globe Functions ====
#include <wx/uilocale.h> // For wxLocale
#include <wx/stdpaths.h> // For wxStandardPaths
#include <wx/config.h> // For wxConfig
#include <map> // For std::map

#include "KGlobal.h"


void GetDefIniFileName(wxString& tIniFileName)
{
	wxString tExeFile=wxStandardPaths::Get().GetExecutablePath();
	// get directory of the executable file
	wxFileName tExtFile(tExeFile);
	wxString tDir=tExtFile.GetPath();
	tIniFileName= tDir +  "/../Resources/" + tExtFile.GetName() + ".ini"; 
};

void GetDefUsrIniFileName(wxString& tIniFileName)
{
	wxString tExeFile=wxStandardPaths::Get().GetExecutablePath();
	// get directory of the executable file
	wxFileName tExtFile(tExeFile);
	wxString tDir=tExtFile.GetPath();
	tIniFileName= tDir +  "/../Resources/" + tExtFile.GetName() + "Usr.ini"; 
	return ;
};


 
bool IsInDarkMode(){ return  wxSystemSettings::GetAppearance().IsDark(); };
bool IsInDarkMode(wxWindow* tWin){ return tWin->GetBackgroundColour() == wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW); };

void InitListColumns(wxDataViewListCtrl* tlist,const StField* tDefFields, size_t tFieldCount) {
	for (size_t i = 0; i < tFieldCount; ++i) {
		tlist->AppendTextColumn(
			tDefFields[i].name,       // 標題
			wxDATAVIEW_CELL_INERT,    // 欄位型態（不可編輯）
			tDefFields[i].width,      // 欄寬
			wxALIGN_LEFT,             // 對齊
			wxDATAVIEW_COL_RESIZABLE  // 欄位可調整寬度
		);
	}
}

void SetListHotkeySetsRows(wxDataViewListCtrl* tList, const StHotkeySet* tHotkeySets, size_t tRowCount) {
  for (size_t i = 0; i < tRowCount; ++i) {
  	wxVector<wxVariant> row;
  	row.push_back(wxVariant(tHotkeySets[i].describe));
  	row.push_back(wxVariant(tHotkeySets[i].hotkey));
  	row.push_back(wxVariant(tHotkeySets[i].label));
  	tList->AppendItem(row);
  }
}

void RestartApp(const char* tCmdLine)
{
    wxString appPath = wxStandardPaths::Get().GetExecutablePath();
	  // exe app with parameter "--restart" so the new app will be acknowlogied to Activate the window
		wxString cmd = wxString::Format("%s %s", appPath,tCmdLine);
		wxExecute(cmd, wxEXEC_ASYNC); // Execute the command asynchronously

    wxTheApp->ExitMainLoop();
}
