#pragma once
#include <map>
#include <wx/wx.h>
#include <wx/choice.h>
#include <wx/dataview.h>
// === Constants ===
// === Define ===
// for setting ListCtrl fields
struct StField { 
	std::string name;
	int width;
};
// for setting Hotkeys ListCtrl contents
struct StHotkeySet {
	std::string label;      // 儲存用 key，例如 SaveFile
	std::string hotkey;     // 預設熱鍵
	wxString describe;   // 顯示說明
};


// === Global Functions ===
void GetDefIniFileName(wxString& tIniFileName); // get ini file name
void GetDefUsrIniFileName(wxString& tIniFileName); // get user ini file name
void RestartApp(const char* tCmdLine);
bool IsInDarkMode(); // check if system is in dark mode
bool IsInDarkMode(wxWindow* tWin); // check if window is in dark mode

void InitListColumns(wxDataViewListCtrl* tlist,const StField* tDefFields, size_t tFieldCount); 
void SetListHotkeySetsRows(wxDataViewListCtrl* tList, const StHotkeySet* tHotkeySets, size_t tRowCount); 