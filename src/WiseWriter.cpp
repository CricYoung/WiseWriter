// WiseWriter.cpp
#include <wx/textfile.h> // For wxTextFile
#include <wx/colordlg.h> // For wxColourDialog
#include <wx/fontdlg.h> // For wxFontDialog
#include <wx/caret.h>
#include <wx/config.h>
#include <wx/stdpaths.h>

#include "WiseWriter.h" // Should include LinkOS.h indirectly
#include "KGlobal.h"
#include "LinkOS.h"
#include "KHotKey.h" // For hotkey functions
#include "KWxColor.h" // For wxColour
#include "DlgAbout.h"
#include "DlgSettings.h" // For DlgSettings
#include "DlgChoseLang.h" // For DlgChoseLang
// Global variables
enum MenuIDs {
	ID_Menu_LoadHistory = wxID_HIGHEST + 1,
	ID_Menu_SaveHistory,
	ID_Menu_ClearHistory,
	ID_Menu_Help,
	ID_Menu_LargeFont,
	ID_Menu_SmallFont,
	ID_Menu_ToggleDarkMode,
	ID_Menu_ToggleFullScreen,
	ID_Menu_RestoreAppPrefer,
};

std::string gTitle="【慧寫】:輸入輔助器"; 
std::string gTitleEn="WiseWriter: Input Assistant";
// --- listHotKeys contents ---
StHotkeySet DefHotkeySets[] = {
	{ "Summon", "Cmd+Enter", _("Summon WiseWriter from other app's input field") },
	{ "PasteBack", "Cmd+Enter", _("Paste content to source app") },
	{ "PrevPaste", "Cmd+PageUp", _("Previous Paste") }, 
	{ "NextPaste", "Cmd+PageDown", _("Next Paste") }, 
	{ "Hide", "Esc", _("Hide and don't paste") },
	{ "ToggleDarkMode", "Shift+Ctrl+Alt+Back", _("Toggle Dark Mode") },
	{ "ToggleFullScreen", "Ctrl+F11", _("Toggle Full Screen") },
	{ "RestoreAppPrefer", "Ctrl+R", _("Restore App Prefer") },
	{ "LoadHistory", "Ctrl+L", _("Load History") },
	{ "SaveHistory", "Ctrl+S", _("Save History") },
	{ "ClearHistory", "Ctrl+D", _("Clear History") },
	{ "ShowHelp", "F1", _("Show Help") },
}; 

const size_t CDefHotkeyCount = sizeof(DefHotkeySets) / sizeof(StHotkeySet);

MyFrame::MyFrame() : wxFrame(NULL, wxID_ANY, gTitle, wxDefaultPosition, wxSize(500, 200)),
	LoadFileName(""),EndMarkStr("##End##"), historyIndex(-1),bHistoryDirty(false),
	oFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL)),
	bInViewMode(false), nLastKeyCode(-1), bCtrlKey(false)
										 
{
	input = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 
		wxTE_MULTILINE | wxTE_PROCESS_ENTER | wxTE_PROCESS_TAB | wxWANTS_CHARS); 
	input->SetFont(oFont);
	input->OSXDisableAllSmartSubstitutions(); // Disable smart substitutions on macOS
	BuildMenu(); // also effective no need CallAfter
	BindEvents(); // Bind events to functions

	CreateStatusBar(4);
	SetStatusWidths(4, new int[4]{ -1, 100, 100, 100 });  // 第一欄自適應，第二、三欄固定寬度
	SetStatusText(_("Welcome to 慧寫...😊"),0); 
	SetViewMod(false);
};

void MyFrame::BuildMenu()
{
	wxMenuBar* menuBar = new wxMenuBar;
	wxMenu* fileMenu = new wxMenu;
	fileMenu->Append(wxID_ABOUT, _("&About"));
	fileMenu->Append(wxID_PREFERENCES, _("Settings"));
	fileMenu->Append(ID_Menu_LoadHistory, _("Load History"));   
	fileMenu->Append(ID_Menu_SaveHistory, _("&Save History")); 
	fileMenu->Append(ID_Menu_ClearHistory, _("Clear History")); 
	fileMenu->AppendSeparator();
	fileMenu->Append(wxID_EXIT, _("&Exit")); 
	fileMenu->Append(ID_Menu_Help, _("&Help"));
	menuBar->Append(fileMenu, _("&File"));

	wxMenu* editMenu = new wxMenu;

	editMenu->Append(ID_Menu_ToggleDarkMode,_("Toggle Dark Mode")); 
	editMenu->Append(ID_Menu_ToggleFullScreen, _("Toggle Full Screen")); 
	editMenu->AppendSeparator();
	editMenu->Append(ID_Menu_RestoreAppPrefer, _("Restore App Prefer")); 
	menuBar->Append(editMenu, _("&Edit"));
	SetMenuBar(menuBar);
	
	menuBar->MacSetCommonMenuBar(menuBar); // 讓 Quit 和 Prefer 移到 macOS 標準位置（系統選單）
	return;
};

void MyFrame::BindEvents()
{
	// this->Bind(wxEVT_CHAR_HOOK, &MyFrame::OnKeyDown, this); // change OnCharHook to OnKeyDown
  input->Bind(wxEVT_KEY_DOWN, &MyFrame::OnKeyDown, this);
	input->Bind(wxEVT_KEY_UP, &MyFrame::OnKeyUp, this);
  this->Bind(wxEVT_CLOSE_WINDOW, &MyFrame::OnClose, this);
  Bind(wxEVT_MENU, &MyFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, &MyFrame::OnSettings, this, wxID_PREFERENCES);
  Bind(wxEVT_MENU, &MyFrame::OnExit, this, wxID_EXIT);
  Bind(wxEVT_MENU, &MyFrame::OnLoadHistory, this, ID_Menu_LoadHistory);
  Bind(wxEVT_MENU, &MyFrame::OnSaveHistory, this, ID_Menu_SaveHistory);
  Bind(wxEVT_MENU, &MyFrame::OnClearHistory, this, ID_Menu_ClearHistory);
  Bind(wxEVT_MENU, &MyFrame::OnShowHelp, this, ID_Menu_Help);

  Bind(wxEVT_MENU, &MyFrame::OnSwitchDarkMode,this, ID_Menu_ToggleDarkMode) ;
  Bind(wxEVT_MENU, &MyFrame::OnTogFullScreen, this, ID_Menu_ToggleFullScreen);
  Bind(wxEVT_MENU, &MyFrame::OnRestoreAppPrefer, this, ID_Menu_RestoreAppPrefer);
  Bind(wxEVT_SYS_COLOUR_CHANGED, &MyFrame::OnSystemColorChanged, this);

  }

void MyFrame::OnIdle(wxIdleEvent &)
{
	if(bIsAutoSaveBySecCount){ 
  	if(nCurSecCountDown <= 0){
			nCurSecCountDown = CMinAutoSaveSecCount; // Reset countdown
			autoSaveTimer->Stop(); // Stop timer
    	SetStatusText(_("Auto save..."), 1);
    	if(SaveHistory()) SetStatusText(_("Auto save done"), 1);
    	else SetStatusText(_("Not saved !"), 1);
    	autoSaveTimer->Start(CCheckTimerPerMiniSec); // Restart timer
			return ;
		};
	};
	if(bIsAutoSaveByKeyCount) {
		if(nCurKeyCountDown <= 0){
			SetStatusText(_("Auto save..."), 1);
    	if(SaveHistory()) SetStatusText(_("Auto save done"), 1);
    	else SetStatusText(_("Not saved !"), 1);
			nCurKeyCountDown = CMinAutoSaveKeyCount; // Reset countdown
			return ;
		};
	};
}

void MyFrame::OnAutoSaveTimer(wxTimerEvent &)
{
	if(!bIsAutoSaveBySecCount) return; 
	if(nCurSecCountDown > 0) { 
		nCurSecCountDown-=CCheckTimerPerSec;
		if(nCurSecCountDown < 0) nCurSecCountDown = 0;
		if(bIsShowTimeCountDown) 
			SetStatusText(wxString::Format("Auto save countdown %d seconds", nCurSecCountDown), 0);
		// when countdown to 0, will save history when idle
	}
}

void MyFrame::IniAutoSaveSettings()
{
	if(!idleEvent){
	  idleEvent = new wxIdleEvent(); 
		Bind(wxEVT_IDLE, &MyFrame::OnIdle, this);
	};
	if(bIsAutoSaveBySecCount) {
		if(!autoSaveTimer) {
		  autoSaveTimer = new wxTimer(this, wxID_ANY); 
  		Bind(wxEVT_TIMER, &MyFrame::OnAutoSaveTimer, this);
		}
	  autoSaveTimer->Start(CCheckTimerPerMiniSec); // Start timer for auto save
	}
	nCurKeyCountDown = nAutoSaveKeyCount; 
	nCurSecCountDown = nAutoSaveSecCount; 
}

MyFrame::~MyFrame()
{
	// Destructor is called when the frame is actually destroyed.
	// Since OnClose uses Veto, this might only happen at app exit.
	// Unregistering in MyApp::OnExit might be more reliable if the frame persists.
	// UnregisterSummonKey(); // Moved to MyApp::OnExit for reliability
	if(dlgSettings){
		dlgSettings->Destroy();
		dlgSettings = nullptr;
	}
}

// use system theme color , user can switch to preferred color by SwitchToDarkMode
void MyFrame::OnSystemColorChanged(wxSysColourChangedEvent& ){
	// SwitchToDarkMode(::IsInDarkMode());
	input->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW)); // Set background color
	input->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT)); // Set text color
}

void MyFrame::OnAbout(wxCommandEvent &)
{
	DlgAbout* tDlg=new DlgAbout(this,_("About WiseWriter"));
	if(!tDlg) return;
	tDlg->SetSize(300,200);
	tDlg->ShowModal();
	tDlg->Destroy();
}

void MyFrame::OnSettings(wxCommandEvent &)
{
	if(!dlgSettings)
		dlgSettings=new DlgSettings(this);
	if(!dlgSettings) return;
	SetDlgSettingsFromAppPrefer();
	dlgSettings->TakeEffect();
	UnregisterSummonKey(); // Unregister the previous hotkey
	int tAns=dlgSettings->ShowModal();
	switch (tAns) {
    case wxID_OK:
		  SetAppPreferFromDlgSettings();
			SetHotkeys(); // Set hotkeys
			SaveAppPrefer(); // Save app prefer
			TakeEffect(input);
  		break;
  	case wxID_CANCEL:
  	case wxID_CLOSE:
  		break;
		case wxID_RESET:
		  SetAppPreferFromDlgSettings();
			SaveAppPrefer(); // Save app prefer
			::RestartApp("--restart"); // Restart the app
			return;
		default:
			break;
	};
	RegistSummonKey();
}



void MyFrame::OnLoadHistory(wxCommandEvent &)
{
	DoLoadHistory();
}

void MyFrame::DoLoadHistory()
{
	wxFileDialog openFileDialog(this, "Open History File", "", "", "Text files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_OK) 
	{
		LoadFileName = openFileDialog.GetPath();
		DoLoadFromFile(LoadFileName);
		ShowStatusHistoryIndex();
	};
}

void MyFrame::DoLoadFromFile(const wxString& loadFileName)
{
    wxTextFile file(loadFileName);
    if (!file.Open()) {
        SetStatusText(wxString::Format("Could not open file: %s", loadFileName), 0);
        return;
    }

    commandHistory.clear();
    wxString elementText;
    wxString line;

    // 清除標記的首尾換行
    wxString mark = EndMarkStr;
    mark.Trim().Trim(false); // 移除首尾空白與換行

    bool isFirstLine = true;
		bool tFindMark = false; // Flag to check if the mark is found
    while (!file.Eof()) {
        // 讀取一段 element（以 mark 分隔）
        while (!file.Eof()) {
            line = isFirstLine ? file.GetFirstLine() : file.GetNextLine();
            isFirstLine = false;

            if (line.IsEmpty()) {
                elementText += "\n";
                continue;
            }

            if (line == mark){
							  tFindMark = true; // Mark found
                break;
						}

            elementText += line + "\n";
        }

				if(tFindMark == false) break; // Break if the mark is not found
        commandHistory.push_back(elementText);
        elementText.Clear();
				tFindMark = false; // Reset the flag for the next element
    }

    file.Close();

    historyIndex = static_cast<int>(commandHistory.size()) - 1;

    if (historyIndex >= 0)
        input->SetValue(commandHistory[historyIndex]);
    else
        input->Clear();

		input->SetModified(false); // Reset modified flag
    bHistoryDirty = false;
		if(bIsAutoSaveByKeyCount) nCurKeyCountDown = nAutoSaveKeyCount; // Reset auto save countdown

    SetStatusText(wxString::Format("History loaded from %s", loadFileName), 0);
}

//save input content to commandHistory
bool MyFrame::SaveInput(){
	if(input->IsModified() == false) return false; // No need to save if not modified
	if (input->GetValue().IsEmpty() && historyIndex >= (int)commandHistory.size()) return false; // No input to save
	if (historyIndex >= 0 && historyIndex < (int)commandHistory.size()) {
		commandHistory[historyIndex] = input->GetValue();
	} else {
		commandHistory.push_back(input->GetValue());
		historyIndex = static_cast<int>(commandHistory.size()) - 1;
	}
	input->SetModified(false); // Reset modified flag
	bHistoryDirty = true; // Set History dirty flag to true
	return true;
};

bool MyFrame::SaveHistory()
{
  SaveInput(); // Save current input to history
	if(!bHistoryDirty) return true; 
  if((!bIsAutoSaveByKeyCount && !bIsAutoSaveBySecCount) || LoadFileName.IsEmpty()){
  	wxFileName fileName(LoadFileName);
		wxString tMsg;
		if(LoadFileName.IsEmpty() && bIsAutoSaveByKeyCount && nCurKeyCountDown<=0) tMsg=_("Auto Save History");
		else tMsg=_("Save History"); 
  	wxFileDialog saveFileDialog(this, tMsg, "", fileName.GetFullName(), "Text files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
  	int tAns=saveFileDialog.ShowModal();
  	if(tAns != wxID_OK) {
  		SetStatusText(_("Save History canceled."),0);
  	 	return false;
  	}
		LoadFileName = saveFileDialog.GetPath();
	};
	wxTextFile file(LoadFileName);
	if (!file.Create(LoadFileName) && !file.Open()) {    
		SetStatusText(wxString::Format("Could not save file: %s", LoadFileName),0);
	  return false;
	}

	file.Clear(); // Clear existing content
	for (const auto &line : commandHistory) {
		file.AddLine(line);
		file.AddLine(EndMarkStr); // AddLine wiil add '\n' at the end automatically !
	}
	file.Write();
	file.Close();
	SetStatusText(wxString::Format(_("History saved to %s"), LoadFileName),0);
	bHistoryDirty = false; 
	nCurKeyCountDown = nAutoSaveKeyCount; 
	nCurSecCountDown = nAutoSaveSecCount;
	return true;
}

void MyFrame::OnClearHistory(wxCommandEvent &)
{
	DoClearHistory();
}

void MyFrame::DoClearHistory()
{
	wxMessageDialog dialog(this, "Are you sure you want to clear the history?", "Clear History", wxYES_NO | wxICON_QUESTION);
	if (dialog.ShowModal() == wxID_YES)
	{
		commandHistory.clear();
		historyIndex = -1; // Reset history index
		SetStatusText("History cleared.",0);
		input->Clear(); // Clear input field
		ShowInputStatus(); 
	}
	else
	{
		SetStatusText("History not cleared.",0);
	}
	ShowStatusHistoryIndex();
}

void MyFrame::OnClose(wxCloseEvent &event)
{
	int tAns=AskSaveHistory();
	switch(tAns) {
	case 0: // Cancel
		return;
	case 1: // Save
		if(SaveHistory() == false) return ; // cancel or fail to Save history
		break; //history saved
	case 2: // Don't save
		break;
	case 3: // No need to save
		break;
  };
	SaveAppPrefer(); // Save app Prefer
  // ::UnregisterAppHotKey();	// Ensure hotkey is unregistered
	UnregisterSummonKey(); // Unregister the summon key
  event.Skip(); // Allow the frame to close
}

void MyFrame::OnExit(wxCommandEvent &)
{
	Close(true); // Close the frame
};

void MyFrame::DoLargeFont()
{
	oFont.SetPointSize(oFont.GetPointSize() + 1);
	input->SetFont(oFont);
	SetStatusText(wxString::Format("Font size increased to %d", oFont.GetPointSize()),0);
}

void MyFrame::DoSmallFont()
{
	if(oFont.GetPointSize() <= 1) return; // Prevent font size from going below 1
	oFont.SetPointSize(oFont.GetPointSize() - 1);
	input->SetFont(oFont);
	SetStatusText(wxString::Format("Font size decreased to %d", oFont.GetPointSize()),0);
}
// void MyFrame::OnChoseFont(wxCommandEvent &)
// {
// 	wxFontData fontData;
// 	fontData.SetInitialFont(oFont);
// 	wxFontDialog fontDialog(this, fontData);
// 	if (fontDialog.ShowModal() == wxID_OK)
// 	{
// 		oFont = fontDialog.GetFontData().GetChosenFont();
// 		input->SetFont(oFont);
// 		SetStatusText(wxString::Format("Font changed to %s", oFont.GetFaceName()),0);
// 	}
// 	else
// 	{
// 		SetStatusText("Font change canceled.",0);
// 	}
// }

// void MyFrame::OnChoseBackground(wxCommandEvent &)
// {
// 	wxColourData data;
// 	data.SetChooseFull(true);
// 	data.SetColour(input->GetBackgroundColour());
// 	wxColourDialog dialog(this, &data);
// 	if (dialog.ShowModal() == wxID_OK)
// 	{
// 		oBackgroundColor = dialog.GetColourData().GetColour();
// 		input->SetBackgroundColour(oBackgroundColor); 
// 		SetStatusText("Background color changed.",0);
// 	}
// 	else
// 	{
// 		SetStatusText("Background color change canceled.",0);
// 	}
// };

// void MyFrame::OnChoseTextColor(wxCommandEvent &)
// {
// 	wxColourData data;
// 	data.SetChooseFull(true);
// 	data.SetColour(input->GetForegroundColour());
// 	wxColourDialog dialog(this, &data);
// 	if (dialog.ShowModal() == wxID_OK)
// 	{
// 		oTextColor = dialog.GetColourData().GetColour();
// 		input->SetForegroundColour(oTextColor); 
// 		SetStatusText("Text color changed.",0);
// 	}
// 	else
// 	{
// 		SetStatusText("Text color change canceled.",0);
// 	}
// };

// void MyFrame::OnChoseDarkBackground(wxCommandEvent &)
// {
// 	wxColourData data;
// 	data.SetChooseFull(true);
// 	data.SetColour(input->GetBackgroundColour());
// 	wxColourDialog dialog(this, &data);
// 	if (dialog.ShowModal() == wxID_OK)
// 	{
// 		oBackgroundColorDark = dialog.GetColourData().GetColour();
// 		input->SetBackgroundColour(oBackgroundColorDark); 
// 		SetStatusText("Dark background color changed.",0);
// 	}
// 	else
// 	{
// 		SetStatusText("Dark background color change canceled.",0);
// 	}
// };
// void MyFrame::OnChoseDarkTextColor(wxCommandEvent &)
// {
// 	wxColourData data;
// 	data.SetChooseFull(true);
// 	data.SetColour(input->GetForegroundColour());
// 	wxColourDialog dialog(this, &data);
// 	if (dialog.ShowModal() == wxID_OK)
// 	{
// 		oTextColorDark = dialog.GetColourData().GetColour();
// 		input->SetForegroundColour(oTextColorDark); 
// 		SetStatusText("Dark text color changed.",0);
// 	}
// 	else
// 	{
// 		SetStatusText("Dark text color change canceled.",0);
// 	}
// };

void MyFrame::OnTogFullScreen(wxCommandEvent &){
	SetWinFullScreen(!IsWinFullScreen());
};

// void MyFrame::OnChoseLang(wxCommandEvent &)
// {
//   DlgChoseLang* tDlg=new DlgChoseLang(this,_("Chose Language"));
// 	if(!tDlg) return;
// 	tDlg->ChoseLang(LanguageName);
// 	tDlg->SetSize(300,200);
// 	tDlg->ShowModal();
// 	wxString tLang=tDlg->GetLang();
//   if(tDlg->IsChosed() == false) return ;
// 	int tLangIndex;
// 	if(ConvertLangToCode(tLang,tLangIndex) == false) {
// 		wxLogError("Language not supported.");
// 		delete tDlg;
// 		return;
// 	};
// 	delete tDlg;
// 	LanguageName = tLang;
// 	//dialog ask user to restart the app
// 	wxMessageDialog dialog(this, "Do you want to restart the app to apply the language change?", "Restart App", wxYES_NO | wxICON_QUESTION);	
// 	dialog.SetYesNoLabels("Restart", "Don't Restart");
// 	if (dialog.ShowModal() == wxID_NO) {
// 		return; // Don't restart
// 	}
//   int tAns=AskSaveHistory();
// 	if(tAns == 0) return; // Cancel
// 	if(tAns == 1) {
// 		if(SaveHistory() == false) return; // Save history
// 	};
// 	SaveAppPrefer(); // Save app Prefer
// 	::RestartApp("--restart"); // Restart the app
// }


wxMenu* MyFrame::GetMenuByName(const wxString& menuName,int* menuIndex) {
	wxMenuBar* menuBar = GetMenuBar();
	if(!menuBar) return nullptr;

  for (int i = 0; i < menuBar->GetMenuCount(); ++i) {
    wxString title = menuBar->GetMenuLabelText(i);
    if (title == _(menuName) || title == menuName) { if(menuIndex) *menuIndex=i; return menuBar->GetMenu(i); };
  };
	if(menuIndex) 
		*menuIndex=-1;
	return nullptr;
};

wxMenuItem* MyFrame::GetMenuItemByName(wxMenu* tpMenu,const wxString& ItemName, int* ItemIndex) {
	if(!tpMenu) return nullptr;

	for (int i = 0; i < tpMenu->GetMenuItemCount(); ++i) {
		wxMenuItem* tpItem= tpMenu->FindItemByPosition(i);
		wxString title = tpItem->GetItemLabelText();
		if (title == _(ItemName) || title == ItemName) { if(ItemIndex) *ItemIndex=i; return tpMenu->FindItemByPosition(i); };
	};
	if(ItemIndex)
	  *ItemIndex=-1;
	return nullptr;
};

// void MyFrame::OnSetFocus(wxFocusEvent& ) {
  // placeholder for focus event:
	// change background color
	// show caret
	// UnregisterSummonKey();
// };

// void MyFrame::OnKillFocus(wxFocusEvent& ) {
	// placeholder for kill focus event:
	// change background color
	// hide caret
	// RegistSummonKey();
// };

int MyFrame::GetWinWindowState()
{
	int state = 0; // Normal
	if (IsIconized()) {
		state = 1; // Minimized
	} else if (IsMaximized()) {
		state = 2; // Maximized
	}
	return state;
};

void MyFrame::SetWinWindowState(int tState)
{
	switch (tState) {
		case 0: // Normal
			Show();
			Iconize(false);
			Maximize(false);
			break;
		case 1: // Minimized
			Show();
			Iconize(true);
			Maximize(false);
			break;
		case 2: // Maximized
			Show();
			Iconize(false);
			Maximize(true);
			break;
		default:
			break;
	}
};

void MyFrame::SetWinFullScreen(bool tFullScreen){
	if (tFullScreen) {
		EnableFullScreenView(true);
		ShowFullScreen( wxFULLSCREEN_ALL & ~wxFULLSCREEN_NOMENUBAR );
	} else {
		ShowFullScreen(false); 
		SetWinWindowState(0); // Set to normal
	}
}; 
void MyFrame::CenterWindow(){
	wxSize tSize=GetSize();
	int X = (wxGetDisplaySize().GetWidth() - tSize.GetWidth()) / 2;
	int Y = (wxGetDisplaySize().GetHeight() - tSize.GetHeight()) / 2;
	SetPosition(wxPoint(X,Y));
};


void MyFrame::SaveHotkeys(wxFileConfig& tConfig){
  tConfig.SetPath("/Hotkeys");
  for (size_t i = 0; i < CDefHotkeyCount; ++i) {
      wxString key = DefHotkeySets[i].label;
      wxString hotkeyStr = DefHotkeySets[i].hotkey;
      tConfig.Write(key, hotkeyStr); 
  }
}

bool MyFrame::LoadHotkeys(wxFileConfig& tConfig){
  tConfig.SetPath("/Hotkeys");
	bool tAns = tConfig.Exists("/Hotkeys");
	if(!tAns) return false;
  for (size_t i = 0; i < CDefHotkeyCount; ++i) {
    wxString key = DefHotkeySets[i].label;
    wxString hotkey;
		if(tConfig.Read(key, &hotkey, DefHotkeySets[i].hotkey)) {
			DefHotkeySets[i].hotkey = hotkey; // Update the hotkey
		} else {
		  tAns=false;
		} 
	}
	return tAns;
}

/*
	{ "ToggleDarkMode", "Shift+Ctrl+Alt+Back", _("Toggle Dark Mode") },
	{ "ToggleFullScreen", "Ctrl+F11", _("Toggle Full Screen") },
	{ "RestoreAppPrefer", "Ctrl+R", _("Restore App Prefer") },
	{ "LoadHistory", "Ctrl+L", _("Load History") },
	{ "SaveHistory", "Ctrl+S", _("Save History") },
	{ "ClearHistory", "Ctrl+D", _("Clear History") },
	{ "ShowHelp", "F1", _("Show Help") },
*/
void MyFrame::SetHotkeys(){
	StKey tKey;
  for(int tNdx=0; tNdx < CDefHotkeyCount; tNdx++){
		std::string tLabel=DefHotkeySets[tNdx].label;
		std::string tHotKey=DefHotkeySets[tNdx].hotkey;
		Str2Key(tHotKey,tKey);	
		if(tLabel == "Summon"){
			oSummonKey = tKey; // Save the summon key
			continue;
		} else if(tLabel == "PasteBack"){
			oPasteBackKey = tKey; // Save the paste back key
			continue;
		} else if(tLabel == "PrevPaste"){
			oPrevPasteKey = tKey; // Save the previous paste key
			continue;
		} else if(tLabel == "NextPaste"){
			oNextPasteKey = tKey; // Save the next paste key
			continue;
		} else if(tLabel == "Hide"){
			oHide = tKey; // Save the hide key
			continue;
		} else if(tLabel == "ToggleDarkMode"){
			oToggleDarkMode = tKey; // Save the toggle dark mode key
			continue;
		} else if(tLabel == "ToggleFullScreen"){
			oToggleFullScreen = tKey; // Save the toggle full screen key
			continue;
		} else if(tLabel == "RestoreAppPrefer"){
			oRestoreAppPrefer = tKey; // Save the restore app prefer key
			continue;
		} else if(tLabel == "LoadHistory"){
			oLoadHistory = tKey; // Save the load history key
			continue;
		} else if(tLabel == "SaveHistory"){
			oSaveHistory = tKey; // Save the save history key
			continue;
		} else if(tLabel == "ClearHistory"){
			oClearHistory = tKey; // Save the clear history key
			continue;
		} else if(tLabel == "ShowHelp"){
			oShowHelp = tKey; // Save the show help key
			continue;
		}
	}

}

void MyFrame::SaveAppPrefer(){
	wxString tIniFileName;
	::GetAvailableIniFullName(tIniFileName,true);
	wxFileConfig tConfig("WiseWriter","Cric.Dev",tIniFileName,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
	tConfig.SetPath("/Preferences"); 
	// Save font settings
	tConfig.Write("FontSize", oFont.GetPointSize());
	tConfig.Write("FontFace", oFont.GetFaceName());
	tConfig.Write("FontStyle", static_cast<int>(oFont.GetStyle()));
	tConfig.Write("FontWeight", static_cast<int>(oFont.GetWeight())); 
  tConfig.Write("FontFamily", static_cast<int>(oFont.GetFamily()));
	//window state
	int tState = GetWinWindowState();
	bool tFullScreen = IsWinFullScreen();
	if(tFullScreen){ SetWinFullScreen(false); wxSleep(1); };  // Set to normal if full screen
	if(tState != 0) SetWinWindowState(0); // Set to normal if not normal
	if(tState == 1) tState = 0; // Set to normal if minimized
	tConfig.Write("WindowPosX", GetPosition().x);
	tConfig.Write("WindowPosY", GetPosition().y);
	tConfig.Write("WindowWidth", GetSize().GetWidth());
	tConfig.Write("WindowHeight", GetSize().GetHeight());
	tConfig.Write("WindowState", tState);
	tConfig.Write("WindowFullScreen", tFullScreen);
	// input background color
	wxString tColorStr = oBackgroundColor.GetAsString(wxC2S_HTML_SYNTAX);
	tConfig.Write("BackgroundColor", tColorStr);
	tColorStr = oTextColor.GetAsString(wxC2S_HTML_SYNTAX);
	tConfig.Write("TextColor", tColorStr);
	tColorStr = oBackgroundColorDark.GetAsString(wxC2S_HTML_SYNTAX);
	tConfig.Write("BackgroundColorDark", tColorStr);
	tColorStr = oTextColorDark.GetAsString(wxC2S_HTML_SYNTAX);
	tConfig.Write("TextColorDark", tColorStr);
	// Save history file name
	tConfig.Write("LoadFileName", LoadFileName);
	// Save language
	tConfig.Write("LanguageName", LanguageName);
	// Save hotkeys
	SaveHotkeys(tConfig);
	// Save Auto Save
	tConfig.Write("IsAutoSaveByKeyCount", bIsAutoSaveByKeyCount);
	tConfig.Write("AutoSaveKeyCount", nAutoSaveKeyCount);
	tConfig.Write("IsAutoSaveBySecCount", bIsAutoSaveBySecCount);
	tConfig.Write("AutoSaveSecCount", nAutoSaveSecCount);
	tConfig.Write("IsShowTimeCountDown", bIsShowTimeCountDown);
	tConfig.Write("IsShowKeyCountDown", bIsShowKeyCountDown);
	tConfig.Flush(); // Save the config
}


bool MyFrame::LoadAppPrefer(){
	wxString tIniFileName;
	::GetAvailableIniFullName(tIniFileName,true);
	if(!wxFileExists(tIniFileName)) return false;
	wxFileConfig tConfig("WiseWriter","Cric.Dev",tIniFileName,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
	tConfig.SetPath("/Preferences");
	// Load font settings
	int tFontSize = tConfig.Read("FontSize", 20);
	wxString tFontFace = tConfig.Read("FontFace", "Arial");
	int style = static_cast<int>(wxFONTSTYLE_NORMAL);
  tConfig.Read("FontStyle", &style, style);
  oFont.SetStyle(static_cast<wxFontStyle>(style));
  int weight = static_cast<int>(wxFONTWEIGHT_NORMAL);
  tConfig.Read("FontWeight", &weight, weight);
  oFont.SetWeight(static_cast<wxFontWeight>(weight));
	int tFontFamily = static_cast<int>(wxFONTFAMILY_DEFAULT);
	tConfig.Read("FontFamily", &tFontFamily, wxFONTFAMILY_DEFAULT);
	oFont.SetFamily(static_cast<wxFontFamily>(tFontFamily));
	oFont.SetPointSize(tFontSize);
	oFont.SetFaceName(tFontFace);
	input->SetFont(oFont);

 	// Load window state
 	bool tFullScreen;
 	tConfig.Read("WindowFullScreen",&tFullScreen, false); 
 	int tState;
 	tConfig.Read("WindowState",&tState, 0); 
 	int X,Y,Width,Height;
 	tConfig.Read("WindowPosX", &X, 100);
 	tConfig.Read("WindowPosY", &Y, 100);
 	tConfig.Read("WindowWidth", &Width, 500);
 	tConfig.Read("WindowHeight", &Height, 200);
 	SetWinWindowState(0); // Set to normal first
 	if(X < 0) X=0;
 	if(Y < 0) Y=0;
 	SetPosition(wxPoint(X,Y));
 	//is default window ?
 	if(X==100 && Y==100 && Width==500 && Height==200) {
 		//center the window
 		CenterWindow();
 	};
 	wxSize tSize=GetSize();
 	if(Width < 0) Width = tSize.GetWidth();
 	if(Height < 0) Height = tSize.GetHeight();
 	SetSize(wxSize(Width,Height));
  if(tState == 2 ) SetWinWindowState(2); // Set to maximized
 	if(tFullScreen) SetWinFullScreen(true); // Set to full screen

 	// Load input background color
 	wxColor tSysColor=wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
 	wxString tSysColorStr=tSysColor.GetAsString(wxC2S_HTML_SYNTAX);
 	wxString tColorStr;
	tConfig.Read("BackgroundColor", &tColorStr, tSysColorStr);
 	oBackgroundColor.Set(tColorStr); 
  tConfig.Read("BackgroundColorDark", &tColorStr, tSysColorStr);
	oBackgroundColorDark.Set(tColorStr);

  tSysColor=wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  tSysColorStr=tSysColor.GetAsString(wxC2S_HTML_SYNTAX);
  tConfig.Read("TextColor", &tColorStr, tSysColorStr);
 	oTextColor.Set(tColorStr);
  tConfig.Read("TextColorDark", &tColorStr, tSysColorStr);
	oTextColorDark.Set(tColorStr);

  SwitchToDarkMode(::IsInDarkMode()); 
	
	// Load history file name
	LoadFileName = tConfig.Read("LoadFileName", "");
	if(!LoadFileName.empty()) {
		DoLoadFromFile(LoadFileName);
	};
	LanguageName = tConfig.Read("LanguageName", ""); // Load language
	ShowStatusHistoryIndex();
	// Load hotkeys
	LoadHotkeys(tConfig);
	SetHotkeys();
	// Load auto save
	bIsAutoSaveByKeyCount = tConfig.Read("IsAutoSaveByKeyCount", false); // Load auto save 
	nAutoSaveKeyCount = tConfig.Read("AutoSaveKeyCount", CMinAutoSaveKeyCount); 
	if(nAutoSaveKeyCount < CMinAutoSaveKeyCount) nAutoSaveKeyCount = CMinAutoSaveKeyCount; 
	nCurKeyCountDown=nAutoSaveKeyCount;
	bIsAutoSaveBySecCount = tConfig.Read("IsAutoSaveBySecCount", false); // Load auto save
	nAutoSaveSecCount = tConfig.Read("AutoSaveSecCount", CMinAutoSaveSecCount);
	if(nAutoSaveSecCount < CMinAutoSaveSecCount) nAutoSaveSecCount = CMinAutoSaveSecCount;
	bIsShowTimeCountDown = tConfig.Read("IsShowTimeCountDown", false);
	bIsShowKeyCountDown = tConfig.Read("IsShowKeyCountDown", false);
	if(bIsAutoSaveBySecCount || bIsAutoSaveByKeyCount) IniAutoSaveSettings();	
	return true;
}

void MyFrame::DeleAppPrefer(){
	wxString tIniFileName;
	::GetAvailableIniFullName(tIniFileName,true);
	wxRemoveFile(tIniFileName); // Delete the ini file
};

bool MyFrame::RestoreAppPrefer()
{
  DeleAppPrefer();
	wxString tIniFileName;
	::GetAvailableIniFullName(tIniFileName,false);
	if(!wxFileExists(tIniFileName)) return false; 
	wxString tUsrIniFileName;
	::GetAvailableIniFullName(tUsrIniFileName,true);
	wxCopyFile(tIniFileName, tUsrIniFileName, true);
	return LoadAppPrefer(); //will set to default
}

void MyFrame::TakeEffect(wxTextCtrl* tpInput)
{
	if(!tpInput) return;
	tpInput->SetFont(oFont);
	SwitchToDarkMode(bIsDarkMode);
	IniAutoSaveSettings();
	SetStatusText("",0);
}

void MyFrame::SetDlgSettingsFromAppPrefer()
{
	if(!dlgSettings) return;
	dlgSettings->SetPreferFont(oFont);
	dlgSettings->SetPreferBackgroundColor(oBackgroundColor);
	dlgSettings->SetPreferTextColor(oTextColor);
	dlgSettings->SetPreferBackgroundColorDark(oBackgroundColorDark);
	dlgSettings->SetPreferTextColorDark(oTextColorDark);
	dlgSettings->SetLanguageName(LanguageName);
	dlgSettings->SetHotkeySets(DefHotkeySets, CDefHotkeyCount);
	dlgSettings->SetIsAutoSaveByKeyCount(bIsAutoSaveByKeyCount);
	dlgSettings->SetAutoSaveKeyCount(nAutoSaveKeyCount);
	dlgSettings->SetIsAutoSaveBySecCount(bIsAutoSaveBySecCount);
	dlgSettings->SetAutoSaveSecCount(nAutoSaveSecCount);
	dlgSettings->SetIsShowTimeCountDown(bIsShowTimeCountDown);
	dlgSettings->SetIsShowKeyCountDown(bIsShowKeyCountDown);
}

void MyFrame::SetAppPreferFromDlgSettings()
{
	if(!dlgSettings) return;
	dlgSettings->GetPreferFont(oFont);
	input->SetFont(oFont);
  dlgSettings->GetLightBackgroundColor(oBackgroundColor);
	dlgSettings->GetLightTextColor(oTextColor);
	dlgSettings->GetDarkBackgroundColor(oBackgroundColorDark);
	dlgSettings->GetDarkTextColor(oTextColorDark);
	dlgSettings->GetLanguageName(LanguageName);
	dlgSettings->GetHotkeySets(DefHotkeySets, CDefHotkeyCount);
	dlgSettings->GetIsAutoSaveByKeyCount(bIsAutoSaveByKeyCount);
	dlgSettings->GetAutoSaveKeyCount(nAutoSaveKeyCount);
	if(nAutoSaveKeyCount < CMinAutoSaveKeyCount) nAutoSaveKeyCount = CMinAutoSaveKeyCount; 
	dlgSettings->GetIsAutoSaveBySecCount(bIsAutoSaveBySecCount);
	dlgSettings->GetAutoSaveSecCount(nAutoSaveSecCount);
	if(nAutoSaveSecCount < CMinAutoSaveSecCount) nAutoSaveSecCount = CMinAutoSaveSecCount;
	dlgSettings->GetIsShowTimeCountDown(bIsShowTimeCountDown);
	dlgSettings->GetIsShowKeyCountDown(bIsShowKeyCountDown);
}

bool MyFrame::ShowHelp(){
  if(!pHelp){
		wxString tIniFileName;
		if(!::GetAvailableIniFullName(tIniFileName,true)) return false; 
  	pHelp=new KHelp(this,_("WiseWriter Help"));
  	if(!pHelp) return false;
  	pHelp->SetSize(800,600);
  	wxSize tSize=pHelp->GetSize();
  	int X = (wxGetDisplaySize().GetWidth() - tSize.GetWidth()) / 2;
  	int Y = (wxGetDisplaySize().GetHeight() - tSize.GetHeight()) / 2;
    pHelp->SetPosition(wxPoint(X,Y));
		wxFileName tFile(tIniFileName);
		//get directory
		wxString tDir=tFile.GetPath();
		int tLangCode=::GetPreferLang(tIniFileName);
		wxString tHelpFileName;
		if(tLangCode == wxLANGUAGE_CHINESE_TRADITIONAL || tLangCode == wxLANGUAGE_CHINESE_SIMPLIFIED) tHelpFileName=tDir+"/WiseWriterHelp.txt";
		else tHelpFileName=tDir+"/WiseWriterHelpEnglish.txt";
		if(wxFileExists(tHelpFileName)) {
			if(!pHelp->LoadFile(tHelpFileName)) return false;
		};
	};
  pHelp->ShowModal(); 
	return true;
}

void MyFrame::ShowStatusHistoryIndex()
{
	if (historyIndex >= 0 && historyIndex < (int)commandHistory.size()) {
		SetStatusText(wxString::Format("Index: %d/%d", historyIndex + 1, (int)commandHistory.size()), 3);
	}
	else {
		SetStatusText(wxString::Format("Index: New/%d", (int)commandHistory.size()), 3);
	}
}

// return 0 cancel, 1 save, 2 don't save, 3 no need to save
int MyFrame::AskSaveHistory()
{
  wxString tTxt=input->GetValue();
	tTxt.Trim();
	if (tTxt.IsEmpty() && historyIndex >= commandHistory.size() && !bHistoryDirty) return 3; // no need to save
	bool tInputDirty = input->IsModified();
	if(historyIndex < commandHistory.size() && input->GetValue() == commandHistory[historyIndex]) tInputDirty = false; // no need to save
	if( !tInputDirty && !bHistoryDirty) return 3; // no need to save

	int tAns=wxID_YES;
	if((!bIsAutoSaveByKeyCount && !bIsAutoSaveBySecCount) || LoadFileName.IsEmpty()){
		wxMessageDialog dialog(this, _("Do you want to save the history before exiting?"), _("Save History"), wxYES_NO | wxCANCEL | wxICON_QUESTION);
		dialog.SetYesNoLabels(_("Save"), _("Don't Save"));
		tAns=dialog.ShowModal();
	};
	// Handle the user's response
	if(tAns == wxID_CANCEL)  return 0; 
	if(tAns == wxID_YES) {
		return 1; //saved 
	};
	return 2; // don't save
}
// implement some functions as VIM
// change Caret form between View Mode and Edit Mode
bool MyFrame::ChangeCaretForm()
{
	wxCaret *caret = input->GetCaret();
	if (!caret)
		return false; // No caret exists

	if (bInViewMode)
		caret->SetSize(wxSize(8, caret->GetSize().GetHeight())); // Example block size
	else
		caret->SetSize(wxSize(1, caret->GetSize().GetHeight())); // Standard line caret
	 
	return true;
}

void MyFrame::SwitchToDarkMode(bool tDarkMode){
	if (tDarkMode) {
		input->SetBackgroundColour(oBackgroundColorDark);
		input->SetForegroundColour(oTextColorDark);
		bIsDarkMode = true;
	} else {
		input->SetBackgroundColour(oBackgroundColor);
		input->SetForegroundColour(oTextColor);
		bIsDarkMode = false;
	}
};

bool MyFrame::SetViewMod(bool tInView)
{
	bInViewMode = tInView;
	// SetTitle(bInViewMode ? "輸入輔助器(F1:Help) View Mode" : "輸入輔助器(F1:Help) Edit Mode");
	SetStatusText(bInViewMode ? "〖View Mode〗" : "【Edit Mode】", 2);
	ChangeCaretForm();
	return bInViewMode;
}

bool MyFrame::LeaveEditMode()
{
	return SetViewMod(true);
}

bool MyFrame::EnterEditMode()
{
	return SetViewMod(false);
}

void MyFrame::MoveCaretAhead(int tCharCount)
{
	long currentPos = input->GetInsertionPoint();
	long newPos = currentPos + tCharCount;
	long lastPos = input->GetLastPosition();

	// Ensure new position is within bounds
	if (newPos < 0)
		newPos = 0;
	if (newPos > lastPos)
		newPos = lastPos;

	input->SetInsertionPoint(newPos);
}

bool MyFrame::MoveCaretUp(int lineDelta)
{
    long currentPos = input->GetInsertionPoint();
    int totalLines = input->GetNumberOfLines();
		long tX,tY;
		input->PositionToXY(currentPos,&tX,&tY);
		int curLine = tY;
		if (curLine < 0 || curLine >= totalLines) return false; // Invalid line
    int targetLine = curLine + lineDelta;
    if (targetLine < 0) targetLine = 0;
    if (targetLine >= totalLines) targetLine = totalLines - 1;

    long newPos = input->XYToPosition(0, targetLine);
    input->SetInsertionPoint(newPos);
    return true;
}


int MyFrame::DeleCaretChar(int tCharCount)
{
	long currentPos = input->GetInsertionPoint();
	long lastPos = input->GetLastPosition();
	long startDelete = currentPos;
	long endDelete = currentPos + tCharCount;

	if (tCharCount < 0) {																				 // Deleting backwards (like backspace)
		startDelete = currentPos + tCharCount; // tCharCount is negative
		endDelete = currentPos;
	}

	// Clamp to valid range
	if (startDelete < 0)
		startDelete = 0;
	if (endDelete > lastPos)
		endDelete = lastPos;
	if (startDelete >= endDelete)
		return 0; // Nothing to delete

	input->Remove(startDelete, endDelete);
	// Caret automatically moves to startDelete after Remove
	return endDelete - startDelete; // Return number of chars actually deleted
}

bool MyFrame::IsSpaceLike(wxUniChar tChar)
{
	if(wxIsalnum(tChar)) return false;
	return true;
	// if (tChar == ' ' || tChar == '\n' || tChar == WXK_TAB) return true;
	// return false;
};

// Move caret to next phrase
// return -1 if fail
// return newPos if success
int MyFrame::NextPhrase()
{
	// if current position is at the end of the text, do nothing
	long currentPos = input->GetInsertionPoint();
	long lastPos = input->GetLastPosition();
	if (currentPos == lastPos)
		return -1;
	// Move caret to next phrase
	long newPos = currentPos + 1;
	// if char is space, move to next char not space
  wxUniChar tChar=input->GetValue().GetChar(newPos);
	if (IsSpaceLike(tChar)) {
		while (newPos < lastPos && IsSpaceLike(input->GetValue().GetChar(newPos)))
			newPos++;
	}
	else {
		// if char is not space, move to next space
		while (newPos < lastPos && !IsSpaceLike(input->GetValue().GetChar(newPos)))
			newPos++;
		// pass through space
		while (newPos < lastPos && IsSpaceLike(input->GetValue().GetChar(newPos)))
			newPos++;
	}

	// if newPos is greater than lastPos, set it to lastPos
	if (newPos > lastPos)
		newPos = lastPos;
	// set the caret position
	return newPos;
}

// shift+tab to move caret to previous phrase
int MyFrame::PrevPhrase()
{
	// if current position is at the beginning of the text, do nothing
	long currentPos = input->GetInsertionPoint();
	if (currentPos == 0)
		return -1;
		
	// Move caret to previous phrase
	long newPos = currentPos - 1;
	// if char is space, move to previous char not space
	if (IsSpaceLike(input->GetValue().GetChar(newPos))) {
		while (newPos > 0 && IsSpaceLike(input->GetValue().GetChar(newPos))) 
			newPos--;
	}
	else {
		// if char is not space, move to previous space
		while (newPos > 0 && !IsSpaceLike(input->GetValue().GetChar(newPos))) 
			newPos--;

		// pass through space
		while (newPos > 0 && IsSpaceLike(input->GetValue().GetChar(newPos))) 
			newPos--;
	}

	// back to the first char in cuurrent phrase
	while (newPos > 0 && !IsSpaceLike(input->GetValue().GetChar(newPos))) 
		newPos--;
	 
	if (newPos > 0)
		newPos++;
	// if newPos is less than 0, set it to 0
	if (newPos < 0)
		newPos = 0;

	return newPos;
};

void MyFrame::DoTabKey(bool tShift)
{
	long currentPos = input->GetInsertionPoint();
	long newPos = currentPos;
	if (tShift) newPos = PrevPhrase();
	else newPos = NextPhrase();
	 
	if (newPos == -1) return; 
	input->SetInsertionPoint(newPos);
	return; 
}

bool MyFrame::MoveCaretToLineTop()
{
	long newPos = input->GetInsertionPoint();
	//move caret to the start of the line
	if(input->GetValue().GetChar(newPos) == '\n') newPos--;
	if(newPos <=0) return false; // No more lines
	while (newPos > 0 && input->GetValue().GetChar(newPos) != '\n')	 
			newPos--;
	if(newPos > 0) newPos++;
	else if(newPos < 0) newPos = 0; // No line found
	input->SetInsertionPoint(newPos);
	return true;
}
bool MyFrame::MoveCaretToLineBottom()
{
	long newPos = input->GetInsertionPoint();
	long lastPos = input->GetLastPosition();
	if (newPos == lastPos) return false; // No more lines
	//move caret to the start of the line
	while (newPos < lastPos && input->GetValue().GetChar(newPos) != '\n')	 
			newPos++;
	if(newPos > lastPos) newPos = lastPos;
	input->SetInsertionPoint(newPos);
	return true;
}
// Handle arrow keys for command history
// Ctrl+Up and Ctrl+Down to navigate command history
bool MyFrame::DoCmdHistoryKey(bool tUp){
	SaveInput(); // Save current input to history
	if (tUp) { 
		if (historyIndex <= 0) return false; // No more history
		historyIndex--;
		input->SetValue(commandHistory[historyIndex]);
		input->SetModified(false); // Reset modified flag
		return true;
	};

	// Ctrl+Down: Next command in history
	if (historyIndex < (int)commandHistory.size() - 1) {
		historyIndex++;
		input->SetValue(commandHistory[historyIndex]); 
    input->SetModified(false); // Reset modified flag
		return true;
	};
	
	// new command if to the end of history , also return false
	if (historyIndex == (int)commandHistory.size() - 1) {
		historyIndex++;
		input->SetValue(""); // Clear input; 
		input->SetModified(false); // Reset modified flag
	};
	return false; // No more history
}

void MyFrame::DoPasteBack()
{
	std::string text = input->GetValue().ToStdString();
	//only save if not empty and is a new command
	if(text.empty() == false && historyIndex >= (int)commandHistory.size()) { 
		commandHistory.push_back(input->GetValue()); 
		bHistoryDirty=true; 
	}; 
	historyIndex = commandHistory.size() - 1; // Set history index to the last command
	// RegistSummonKey(); 
	// RememberFrontApp(); // Remember before hiding
	SetClipboardText(text);
	Hide();											 // Hide this window first
	// usleep(50000);							 // Small delay before switching might help
	RestoreFocusToPreviousApp(); // Switch back
	usleep(200);							 // Wait for focus switch and target app to be ready
	SendPasteGemini();				 // Send Cmd+V (+ Enter if enabled)
	return;											 // Handled
};

// return true if handled
// return false if not handled
bool MyFrame::DoStaticKey(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	switch (keyCode) {
		case WXK_TAB: // Tab key
			DoTabKey(event.ShiftDown()); break;
		// case WXK_LEFT:
		// 	MoveCaretAhead(-1); break;
  	// case WXK_RIGHT:
  	// 	MoveCaretAhead(+1); break;
  	// case WXK_UP:
  	// 	MoveCaretUp(-1); break;
  	// case WXK_DOWN:
  	// 	MoveCaretUp(+1); break;
		case WXK_HOME: // Move to beginning of line
			MoveCaretToLineTop(); break;
		case WXK_END: // Move to end of line
			MoveCaretToLineBottom(); break;
		case WXK_PAGEUP: // Move to previous page
		  //if at top of the page, move to previous history
			if(input->GetInsertionPoint() == 0) {
				DoCmdHistoryKey(true); ShowStatusHistoryIndex(); return true; // Handled
			};
		  MoveCaretPgUp(); return true;
			// return false;
		case WXK_PAGEDOWN: // Move to next page
			//if at end of the page, move to next history
			if(input->GetInsertionPoint() == input->GetLastPosition()) {
				DoCmdHistoryKey(false); ShowStatusHistoryIndex(); return true; // Handled
			};
			MoveCaretPgDown(); return true;
			// return false;;
		case '=':
		case WXK_NUMPAD_ADD:
		  if(event.CmdDown()) { DoLargeFont(); return true; } // Handled
			return false; // Not handled
		case '-':
		case WXK_NUMPAD_SUBTRACT:
		  if(event.CmdDown()) { DoSmallFont(); return true; } // Handled
			return false; // Not handled
		default:
			return false;					
	}
	return true; // Handled
};

bool MyFrame::DoVimViewKey(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	// bool altDown = event.AltDown();					// Check for Alt/Option key
	switch (keyCode) {
		case 'I':
			EnterEditMode(); break; // Handled
		case 'A':
			MoveCaretAhead(1); EnterEditMode(); break; // Handled
		case 'H':
			MoveCaretAhead(-1); break;
		case 'L':
			MoveCaretAhead(+1); break;
		case 'K':
			MoveCaretUp(-1); break;
	  case 'J': 
			MoveCaretUp(+1); break;
		// case 'B': 
		  // DoTabKey(true); break; // Previous phrase
		// case 'F':
		  // DoTabKey(false); break; // Next phrase
		// case 'T':
			// DoTabKey(event.ShiftDown()); break;
		case '0':
		case 'U': // as numpad Home
		  MoveCaretToLineTop(); break;
		case '4': // '$' == shift+4
		case 'M': // as numpad End
		  MoveCaretToLineBottom(); break;
		case 'O': // as numpad PgUp
			MoveCaretPgUp(); break;
		case '.': // as numpad PgDown
			MoveCaretPgDown(); break;
		case 'X': // Delete character under cursor
			DeleCaretChar(1); break;
		case WXK_ESCAPE:	 // Go back to edit mode from view mode? Or just stay? Let's stay for now.
			EnterEditMode(); break;
		case WXK_RETURN: 
		  EnterEditMode(); break; // Handled
		default:
			return false;					
	};
	return true; //handled
};

bool MyFrame::DoVimEditKey(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	switch (keyCode) {
		case 'J': 
			if(nLastKeyCode == 'J') { 
				nLastKeyCode=-1; 
				MoveCaretAhead(-1); 
				DeleCaretChar(1); 
				LeaveEditMode(); 
				ResetCombineKey();
				return true;
			}
			nLastKeyCode = keyCode; // Save the last key code
			SetStatusText(wxString::Format(" %c ", keyCode), 1); // Show status
			return false; // Handled
		case WXK_ESCAPE: ResetCombineKey(); LeaveEditMode(); return true; // Handled 
		case WXK_RETURN: 
			return false;
		default:
			return false;
  };
	return true; // handled
};

void MyFrame::ShowInputStatus(){
	if(input->IsModified()) {
		SetTitle(gTitle+"[Modified]");
	} else {
		SetTitle(gTitle);
	};
}

void MyFrame::ResetCombineKey()
{
	nLastKeyCode = -1; // Reset last key code
	SetStatusText(wxString::Format("    "), 1); 
	return ; 
};

bool MyFrame::DoCmdAltShiftKey(wxKeyEvent &event){
	int keyCode = event.GetKeyCode();
	switch(keyCode) {
		case '0': // Move to beginning of line
			MoveCaretToLineTop();
			break;
		case '9': // Move to end of line
			MoveCaretToLineBottom();
			break;
		case 'S': // Delete to end of line
			if(bInViewMode) return false;
			DeleToLineEnd();
			break ;
		case 'B': // Delete to beginning of line
			if(bInViewMode) return false;
			DeleToLineStart();
			break;
		case '[': // Previous command in history
			DoCmdHistoryKey(true);
			ShowStatusHistoryIndex();
			break;
		case ']': // Next “this is a pen”command in history
			DoCmdHistoryKey(false);
			ShowStatusHistoryIndex();
			break;
		default:
		  return false; // Not handled
	}
	return true; // Handled
};

bool MyFrame::DoStKey(wxKeyEvent &, StKey& tKey)
{
	if(tKey == oHide){
		::HideCurrentApp();
		RestoreFocusToPreviousApp(); // Switch back
		usleep(20);							 // Wait for focus switch and target app to be ready
		return true; // Handled
	};

	if(tKey == oPasteBackKey) {
		DoPasteBack(); ShowStatusHistoryIndex(); return true; // Handled
	};
	if(tKey == oPrevPasteKey) {
		fprintf(stderr,"oPrevPasteKey\n");
		DoCmdHistoryKey(true); ShowStatusHistoryIndex(); return true; // Handled
	};
	if(tKey == oNextPasteKey) {
		fprintf(stderr,"oNextPasteKey\n");
		DoCmdHistoryKey(false); ShowStatusHistoryIndex(); return true; // Handled
	};
	if(tKey == oToggleDarkMode) {
		SwitchToDarkMode(!bIsDarkMode); 
		return true; // Handled
	};
	if(tKey == oToggleFullScreen) {
		SetWinFullScreen(!IsWinFullScreen()); 
		return true; // Handled
	};
	if(tKey == oRestoreAppPrefer) {
		RestoreAppPrefer(); 
		return true; // Handled
	};
	if(tKey == oLoadHistory) {
		DoLoadHistory(); 
		return true; // Handled
	};
	if(tKey == oSaveHistory) {
		SaveHistory(); 
		return true; // Handled
	};
	if(tKey == oClearHistory) {
		DoClearHistory(); 
		return true; // Handled
	};
	if(tKey == oShowHelp) {
		ShowHelp(); 
		return true; // Handled
	};
	return false;
};

bool MyFrame::FindChar(int keyCode,bool tBackward,int tPlusMove,bool tRemember)
{
	if(tRemember) {
		nLastFindCharCode = keyCode; // Save the last key code
		bLastFindBackward = tBackward; // Save the last direction
		nLastFindPlusMove = tPlusMove; // Save the last plus move
	}
	long tNewPos=DoFindChar(keyCode,tBackward);
	if(tNewPos < 0) return false;
	 
	input->SetInsertionPoint(tNewPos);
	if(tPlusMove) MoveCaretAhead(tPlusMove);  
	return true;
};

// return -1 if not found, return new position if found
long MyFrame::DoFindChar(int keyCode,bool tBackward){
	long currentPos = input->GetInsertionPoint();
	long lastPos = input->GetLastPosition();

	if(tBackward) {
		if (currentPos == 0) return -1; // No more lines
		for(long newPos = currentPos - 2; newPos >= 0; newPos--) {
			if (input->GetValue().GetChar(newPos) == keyCode) return newPos; 
		}
		return -1; // Not found
	} 

	if (currentPos == lastPos) return -1; // No more lines
	for(long newPos = currentPos + 1; newPos < lastPos; newPos++) {
		if (input->GetValue().GetChar(newPos) == keyCode) return newPos;
	}
	return -1; // Not found
};

int MyFrame::DoDeleLine(int tLineCount)
{
	//mark the start and end of the line
	long lineStart,lineEnd;
	if(GetLineStartEndPos(lineStart,lineEnd, tLineCount) == false) return 0; // No more lines
	input->Remove(lineStart, lineEnd);
	// Caret automatically moves to startDelete after Remove
	return lineEnd - lineStart; 
};

// alfabet or number phrease , or none alfa number phrease
bool MyFrame::GetCurPhrasePos(long& tStartPos, long& tEndPos){
	long currentPos = input->GetInsertionPoint();
	long lastPos = input->GetLastPosition();
	// Move caret to current phrase head
	long newPos = currentPos;
	// if char is space, move to previous char not space
	if (IsSpaceLike(input->GetValue().GetChar(newPos))) {
		while (newPos > 0 && IsSpaceLike(input->GetValue().GetChar(newPos))) newPos--;
		if(newPos <= 0) newPos = 0; 
		if(!IsSpaceLike(input->GetValue().GetChar(newPos))) newPos++;
		tStartPos = newPos;

		// pass through space
		newPos = currentPos;
		while (newPos < lastPos && IsSpaceLike(input->GetValue().GetChar(newPos))) newPos++;
		if(newPos >= lastPos) newPos = lastPos;
		// if(!IsSpaceLike(input->GetValue().GetChar(newPos))) newPos--;
		tEndPos = newPos;
		return true;
	};

	// if char is not space, move to none space startPos
	while (newPos > 0 && !IsSpaceLike(input->GetValue().GetChar(newPos))) newPos--;
	if(newPos <= 0) newPos = 0; 
	if(IsSpaceLike(input->GetValue().GetChar(newPos))) newPos++;
	tStartPos = newPos;
	
	newPos = currentPos;
	while (newPos > 0 && !IsSpaceLike(input->GetValue().GetChar(newPos))) newPos++; 
	if(newPos >= lastPos) newPos = lastPos;
	// if(IsSpaceLike(input->GetValue().GetChar(newPos))) newPos--;
	tEndPos = newPos;
	return true;
}

int MyFrame::DoDelePhrease(int tPhreaseCount){
	long tartPos, endPos;
	long tCount = 0;
	for(int i=0; i<tPhreaseCount; i++) {
		if(!GetCurPhrasePos(tartPos, endPos)) return 0; // No more lines
		input->Remove(tartPos, endPos);
		tCount += endPos - tartPos;
	};
	// Caret automatically moves to startDelete after Remove
	return tCount;
};


int MyFrame::DoDCharKey(int tKeyCode){
	switch(tKeyCode) {
		case 'D': // Delete to end of line
		case 'd':
			return DoDeleLine(1);
		case 'W':
		case 'w':
			return DoDelePhrease(1);
		case '4': // '$' = shift+4
		case 'E':
		case 'e':
			return DeleToLineEnd();
		case '0':
		case 'B':
		case 'b':
			return DeleToLineStart();
		default: 
		  return false; // Not handled
	}
}

// vim like key handling
// jj t? T? f? F? dd d$ d0
bool MyFrame::DoVimCombineKey(wxKeyEvent &event)
{
	int keyCode = event.GetKeyCode();
	bool tShift = event.ShiftDown();
	if(!tShift) keyCode = wxTolower(keyCode); 
	switch(nLastKeyCode){
		case 'F':
		  SetStatusText(wxString::Format(" %c%c ",nLastKeyCode, keyCode), 1); 
			FindChar(keyCode,true,1,true); 
			nLastKeyCode=-1;
			return true; 
		case 'f':
			SetStatusText(wxString::Format(" %c%c ",nLastKeyCode, keyCode), 1); 
			FindChar(keyCode,false,1,true);  
			nLastKeyCode=-1;
			return true;	
		case 'T':
			SetStatusText(wxString::Format(" %c%c ",nLastKeyCode, keyCode), 1); 
			FindChar(keyCode,true,0,true);
			nLastKeyCode=-1;
			return true;  
		case 't':
			SetStatusText(wxString::Format(" %c%c ",nLastKeyCode, keyCode), 1); 
			FindChar(keyCode,false,0,true); 
			nLastKeyCode=-1;
			return true; 
		case 'D':
		case 'd':
			SetStatusText(wxString::Format(" %c%c ",nLastKeyCode, keyCode), 1);
			if(!DoDCharKey(keyCode)) break;
			nLastKeyCode=-1;
			return true;
		// case '/': 
		// case 'Y':
		  // return true;
		default:
			break;
	}
	
	switch(keyCode) {
		case ';':
		case 'n':
		  FindChar(nLastFindCharCode,bLastFindBackward,nLastFindPlusMove,false);
			return true;
		case 'N':
		  FindChar(nLastFindCharCode,!bLastFindBackward,nLastFindPlusMove,false);
			return true;
		case 'T': 
		case 't':
		case 'F':
		case 'f':
		case 'D': 
		case 'd':
		// case 'Y': 
		// case 'y':
		// case '/':
			nLastKeyCode = keyCode; // Save the last key code
			SetStatusText(wxString::Format(" %c ", keyCode), 1); // Show status 
			return true; // Handled
		default:
		  break;
	}
	nLastKeyCode = -1; 
	return false;
};
// 使用 wxEVT_KEY_DOWN 來處理所有需要在預設行為之前的鍵盤事件
void MyFrame::OnKeyDown(wxKeyEvent &event)
{
	if(bIsAutoSaveByKeyCount) {
		if(nCurKeyCountDown > 0){
			 nCurKeyCountDown--;
			 if(bIsShowKeyCountDown) SetStatusText(wxString::Format("Auto Save after %i Keydown", nCurKeyCountDown), 0); 
		};
		if(nCurKeyCountDown == 0) {
			if(bIsShowKeyCountDown) SetStatusText(wxString::Format("Auto Save when idle after %i Keycounts", nAutoSaveKeyCount), 0);
		  // will save file when idle
		};
	};

	int keyCode = event.GetKeyCode();
	if(keyCode == 396) { // 396 is the key code for the "Control" key on Mac wxWidgets
		bCtrlKey = true;
		return ;
	};
	bool altDown = event.AltDown();					// Check for Alt/Option key
	bool shiftDown = event.ShiftDown();			// Check for Shift key
	bool cmdDown = event.CmdDown();				// Check for Command key
  // fprintf(stderr,"keyCode=%i(%c), controlDown=%i, altDown=%i, shiftDown=%i, cmdDown=%i\n",keyCode,keyCode,bCtrlKey,altDown,shiftDown,cmdDown);

  StKey tKey(bCtrlKey, altDown, shiftDown, cmdDown, keyCode);
  // Check for hotkeys
  if(DoStKey(event, tKey)) { ResetCombineKey(); ShowInputStatus(); return; }; // Handled
  //for myself, I use remap [space] to Shift+Cmd+Option 
  if(cmdDown && altDown && shiftDown) {
  	if(DoCmdAltShiftKey(event)){ ResetCombineKey(); ShowInputStatus();  return; };// Handled
  }; 
	// common key handling
	if(DoStaticKey(event)) { ResetCombineKey(); ShowInputStatus(); return; }; // Handled
	// VIM like key handling
  // View Mode Logic
	if(keyCode >= 0x7F) { // Ignore non-printable characters
		event.Skip();
		return;
	};
	if (bInViewMode) {
		VimKeyProc(event); // Process Vim keys
		// event.Skip() is intentionally not called, managed by VimKeyProc or by returning here.
		// The original logic also had a return here, effectively not calling event.Skip().
		return; 
	};

	// Edit Mode Logic
	if(DoVimEditKey(event)){ ShowInputStatus();  return ; }; // Vim-like key handling
  if(keyCode != 'J') ResetCombineKey(); // Reset last key code if not 'J'
	// If not handled by any special logic, allow default processing (typing)
	event.Skip();
};

bool MyFrame::RegistSummonKey()
{
  StKey tKey;
  GetSummonKey(tKey); // Get the summon key from the frame
  int tModifier=tKey.GetCarbonModifier();
  int tMacKeyCode=Wxk2MacKey(tKey.keyCode); 
  if(!::RegisterAppHotKey(tMacKeyCode,tModifier, &MyFrame::SummonHotkeyHandler, this)) {
		wxLogError("Failed to register summon hotkey.");
		return false;
	}
	bSummonKeyRegistered = true;
	return true;
};

void MyFrame::SummonHotkeyHandler(void *userData)
{
	MyFrame *frame = static_cast<MyFrame *>(userData);
	if (frame)
	{
		//if active window is this app, and Summon key is the same as PasteBack key, do PasteBack
		if (frame->IsActive() && frame->IsSummonKeySameAsPasteBackKey()) {
			frame->DoPasteBack();
			return;
		}
		// Check if the frame is already visible (e.g., user clicked away and hit hotkey again)
		// if (!frame->IsShown())
		// {
			::RememberFrontApp(); // Remember the app that was active *before* we activate
		// }
		// Use ObjC function (from LinkOS module) to bring *this app* to front
		::ActivateAppWindow();
		// Now show and raise the C++ window
		frame->Show();
		frame->Raise();
		// Set focus specifically to the text input
		frame->input->SetFocus();
		// Optionally reset mode to Edit when shown
	  frame->EnterEditMode();
	  frame->ShowStatusHistoryIndex();
	}
	else
	{
		wxLogError("SummonHotkeyHandler called with null userData!");
	}
	// No return value needed for the generic callback
}

// Implementation for VimKeyProc
void MyFrame::VimKeyProc(wxKeyEvent& event)
{
    int keyCode = event.GetKeyCode();
    wxUniChar uc = event.GetUnicodeKey();

    // 1. Handle ESC key
    if (keyCode == WXK_ESCAPE) {
        vimInputBuffer.clear();
        currentVimCmd = {}; // Reset command (param 0, action undefined, object undefined)
        currentVimCmd.action = evcUndefined;
        currentVimCmd.object = evoUndefined;
        currentVimCmd.count = 0;
        EnterEditMode(); // Switch to Edit Mode
        SetStatusText("Vim Buffer Cleared (ESC)", 1); // Clear status
        return;
    }

    // 2. Append to vimInputBuffer (if it's a printable character)
    // We only append actual characters, not modifiers or special keys like Shift, Ctrl etc.
    // WXK_NONE is returned if the key press does not correspond to a printable character.
    if (uc != WXK_NONE) {
        // Basic ASCII range check, can be expanded if more complex chars needed for commands
        // For Vim commands, typically we care about ASCII characters.
        if (uc >= 32 && uc <= 126) { // Printable ASCII characters
            vimInputBuffer += (char)uc;
        } else {
            // Potentially handle other special wxKeyCodes that ARE part of commands but don't have simple Unicode,
            // e.g. WXK_RETURN, WXK_TAB if they were to be part of a command sequence.
            // For now, we primarily focus on chars like 'd', 'w', '2', 'f', 'x' etc.
            // If a non-ASCII char is pressed that we don't want in buffer, we can ignore or beep.
             event.Skip(); // Let other handlers deal with it if not a simple char we want.
             return;
        }
    } else {
        // Handle special keys that don't have a Unicode representation but might be part of a command
        // (e.g., if you wanted to map WXK_F1 to something in Vim mode).
        // For now, if it's not ESC (handled above) and not a printable char, we ignore it in VimKeyProc.
        // This prevents arrow keys, function keys etc. from populating the vimInputBuffer unless specifically handled.
        // event.Skip(); // Pass it on if not used by VimKeyProc.
        return; // Or simply return if these keys have no meaning in Vim command buffer.
    }
    
    SetStatusText(wxString::FromUTF8(vimInputBuffer.c_str()), 1); // Display current buffer

    // 3. Parse vimInputBuffer
    // Placeholder for return status from VimParseIncompleteCmd
    // enum class ParseStatus { Complete, Incomplete, Invalid };
    // bool isComplete = VimParseIncompleteCmd(vimInputBuffer, currentVimCmd); 
    // For now, using a simple bool: true for complete, false for incomplete/invalid.
    // The subtask asks VimParseIncompleteCmd to return false for "incomplete" initially.
    // We need a way to distinguish Invalid from Incomplete.
    // Let's assume VimParseIncompleteCmd will set cmd.action to evcUndefined for invalid sequences.
    
    bool parseResult = VimParseIncompleteCmd(vimInputBuffer, currentVimCmd);

    // 4. Handle parsing results
    if (currentVimCmd.action == evcUndefined && !vimInputBuffer.empty()) { // Considered INVALID by convention
        // Check if buffer is not empty because an empty buffer parsed might correctly result in evcUndefined
        // for the initial state of currentVimCmd.
        // This case handles where VimParseIncompleteCmd explicitly marks it as invalid.
        wxBell(); // Audible feedback for invalid command
        SetStatusText(wxString::Format("Invalid: %s", vimInputBuffer.c_str()), 1);
        vimInputBuffer.clear();
        currentVimCmd = {}; // Reset
        currentVimCmd.action = evcUndefined;
        currentVimCmd.object = evoUndefined;
        currentVimCmd.count = 0;
    } else if (parseResult == true) { // COMPLETE
        // currentVimCmd is updated by VimParseIncompleteCmd
        VimExeString(vimInputBuffer); // Execute the command
        SetStatusText(wxString::Format("Executed: %s", vimInputBuffer.c_str()), 1);
        vimInputBuffer.clear();
        currentVimCmd = {}; // Reset
        currentVimCmd.action = evcUndefined;
        currentVimCmd.object = evoUndefined;
        currentVimCmd.count = 0;
    } else { // INCOMPLETE but valid so far
        // currentVimCmd is updated by VimParseIncompleteCmd.
        // Wait for more input.
        // Status bar already shows current vimInputBuffer.
    }
}

// Stub for VimParseIncompleteCmd
// Returns true if command is complete, false if incomplete or invalid.
// For invalid, it should set cmd.action = evcUndefined as a signal.
bool MyFrame::VimParseIncompleteCmd(const std::string& buffer, StVimCmd& cmd)
{
    // TODO: Implement actual Vim command parsing logic.
    // For now, this stub treats all commands as incomplete unless they are "dd",
    // and "dx" as invalid to test the logic.
    // It should fill cmd (count, action, object, param) based on the buffer.

    cmd.action = evcUndefined; // Default to undefined / invalid
    cmd.object = evoUndefined;
    cmd.count = 0;
    cmd.param = 0;

    if (buffer.empty()) {
        return false; // Incomplete (empty is not a command)
    }

    // Example: "dd" is complete
    if (buffer == "dd") {
        cmd.action = evcDelete;
        cmd.object = evoLine; // Simplified, 'dd' implies current line
        cmd.count = 1; // or parse count if present
        return true; // Complete
    }
    
    // Example: "d" is incomplete
    if (buffer == "d") {
        cmd.action = evcDelete; // Action is known
        // object, count might still be coming
        return false; // Incomplete
    }
    
    // Example: "dw" is complete
     if (buffer == "dw") {
        cmd.action = evcDelete;
        cmd.object = evoWord;
        cmd.count = 1;
        return true; // Complete
    }

    // Example: "dx" is invalid (let's assume x is not a valid motion for d)
    if (buffer == "dx") {
        cmd.action = evcUndefined; // Mark as invalid
        return false; // Technically the command sequence is "complete" but invalid
                      // The logic in VimKeyProc will check cmd.action for evcUndefined
    }
    
    // Default: treat as incomplete for this stub
    // A real parser would be more sophisticated.
    // If the buffer starts with a valid command prefix, keep action set.
    if (!buffer.empty()) {
        char firstChar = buffer[0];
        // Rudimentary check, a real parser would be a state machine.
        if (firstChar == 'd') cmd.action = evcDelete;
        else if (firstChar == 'c') cmd.action = evcChange;
        else if (firstChar == 'y') cmd.action = evcYield;
        // ... etc.
        // If cmd.action is still evcUndefined after checking prefixes, it's likely invalid from the start.
    }


    return false; // Default to incomplete
}

// Implementation for VimExeString
void MyFrame::VimExeString(const std::string& cmdStr)
{
    // 1. Call VimCompiler to parse the complete command string
    VimCompiler(cmdStr, currentVimCmd); // currentVimCmd is a member, VimCompiler populates it

    // 2. Call VimExeCmd to execute the command
    VimExeCmd(currentVimCmd);

    // 3. Clear vimInputBuffer and reset currentVimCmd
    vimInputBuffer.clear();
    currentVimCmd.action = evcUndefined;
    currentVimCmd.object = evoUndefined;
    currentVimCmd.count = 0;
    currentVimCmd.param = 0; // Reset param as well

    // Update status or provide feedback
    SetStatusText(wxString::Format("Cmd Executed. Buffer cleared. Last cmd: %s", cmdStr.c_str()), 0);
}

// Implementation for VimCompiler
void MyFrame::VimCompiler(const std::string& cmdStr, StVimCmd& cmd)
{
    // 1. Initialization
    cmd.count = 1; // Default count is 1, not 0. "dw" is 1dw.
    cmd.action = evcUndefined;
    cmd.object = evoUndefined;
    cmd.param = 0;

    if (cmdStr.empty()) {
        return; // Nothing to parse
    }

    size_t index = 0;

    // 3. Parse Count
    // Handles counts like "123dw"
    if (isdigit(cmdStr[index])) {
        if (cmdStr[index] == '0' && (cmdStr.length() > 1 && isdigit(cmdStr[index+1])) ) {
            // '0' followed by another digit is not a valid count prefix,
            // '0' itself might be a motion (evoToLineStart0)
            // This condition might need refinement based on how '0' motion is handled
        } else {
            size_t countEnd = index;
            while (countEnd < cmdStr.length() && isdigit(cmdStr[countEnd])) {
                countEnd++;
            }
            std::string countStr = cmdStr.substr(index, countEnd - index);
            if (!countStr.empty()) {
                try {
                    int parsedCount = std::stoi(countStr);
                    if (parsedCount > 0) { // Count must be positive. '0' is not a count for "0dw".
                        cmd.count = parsedCount;
                    } else if (parsedCount == 0 && countStr.length() == 1) {
                        // If '0' is the entire "count part", it might be a motion.
                        // Let action/object parsing handle it. For now, keep count = 1.
                    }
                    index = countEnd;
                } catch (const std::out_of_range&) {
                    // Count too large, treat as error or default
                    cmd.action = evcUndefined; return; // Invalid command
                } catch (const std::invalid_argument&) {
                    // Not a number, should not happen if isdigit passed
                    cmd.action = evcUndefined; return; // Invalid command
                }
            }
        }
    }
    
    if (index >= cmdStr.length()) { // Only count specified, e.g., "123"
        cmd.action = evcUndefined; // Incomplete or invalid
        return;
    }

    // 4. Parse Action
    char actionChar = cmdStr[index];
    bool actionParsed = true;

    switch (actionChar) {
        case 'd': cmd.action = evcDelete; break;
        case 'y': cmd.action = evcYield; break;
        case 'c': cmd.action = evcChange; break;
        case 'p': cmd.action = evcPut; break; // Typically no object, or uses register
        case 'i': cmd.action = evcInsert; break; // Mode change, no object
        case 'a': cmd.action = evcAppend; break;  // Mode change, no object
        case 'o': cmd.action = evcOpenLineBelow; break; // No object
        case 'O': cmd.action = evcOpenLineAbove; break; // No object
        case 'r': cmd.action = evcReplaceChar; break; // Needs param
        case 's': cmd.action = evcSubstitute; break; // Char substitute, or line with 'S'
        case 'f': cmd.action = evcFind; cmd.object = evoFindCharForward; break; // Needs param
        case 'F': cmd.action = evcFind; cmd.object = evoFindCharBackward; break;// Needs param
        case 't': cmd.action = evcFind; cmd.object = evoTillCharForward; break; // Needs param
        case 'T': cmd.action = evcFind; cmd.object = evoTillCharBackward; break;// Needs param
        case 'x': // Special case: x -> d + evoCharacter (delete char under cursor)
            cmd.action = evcDelete;
            cmd.object = evoCharacter;
            // cmd.count applies to 'x' as well, e.g. "2x" deletes 2 chars
            index++; // Consume 'x'
            goto end_parse; // 'x' is a complete command action + object
        case 'D': // Special case: D -> d$
            cmd.action = evcDelete;
            cmd.object = evoToLineEnd;
            index++;
            goto end_parse;
        case 'C': // Special case: C -> c$
            cmd.action = evcChange;
            cmd.object = evoToLineEnd;
            index++;
            goto end_parse;
        case 'Y': // Special case: Y -> yy
            cmd.action = evcYield;
            cmd.object = evoLine;
            index++;
            goto end_parse;
        default:
            actionParsed = false; // Not a recognized action char
            cmd.action = evcUndefined; // Mark as invalid
            return; // Early exit if action is not recognized
    }
    index++; // Move past the action character

    // 5. Parse Object/Motion (if applicable)
    if (index >= cmdStr.length()) { // Action requires more but string ended
        // e.g. just 'd' is incomplete if not 'dd'. 'p', 'o', 'O', 'i', 'a' are fine.
        if (cmd.action == evcDelete || cmd.action == evcYield || cmd.action == evcChange ||
            ((cmd.action == evcFind) && (cmd.object == evoFindCharForward || cmd.object == evoFindCharBackward || cmd.object == evoTillCharForward || cmd.object == evoTillCharBackward)) ||
             cmd.action == evcReplaceChar) {
            cmd.action = evcUndefined; // Mark as incomplete/invalid because it needs an object/param
        }
        goto end_parse; // Or it's a complete command like 'p'
    }

    // Check for doubled characters like dd, yy, cc
    if ((cmd.action == evcDelete && cmdStr[index] == 'd') ||
        (cmd.action == evcYield && cmdStr[index] == 'y') ||
        (cmd.action == evcChange && cmdStr[index] == 'c')) {
        cmd.object = evoLine;
        index++;
        goto end_parse;
    }

    // Parse parameter for f, F, t, T, r
    if (cmd.action == evcReplaceChar || 
        ((cmd.action == evcFind) && (cmd.object == evoFindCharForward || cmd.object == evoFindCharBackward || cmd.object == evoTillCharForward || cmd.object == evoTillCharBackward))) {
        if (index < cmdStr.length()) {
            cmd.param = cmdStr[index];
            index++;
        } else {
            cmd.action = evcUndefined; // Needs a param, but string ended
        }
        goto end_parse;
    }
    
    // Specific object/motion parsing if action doesn't imply object and is not param-based
    // This section is only reached if the command is not yet complete (e.g. 'd' followed by something other than 'd')
    // And if the command takes an object (d, y, c)
    if (cmd.action == evcDelete || cmd.action == evcYield || cmd.action == evcChange) {
        if (index < cmdStr.length()) {
            char objectChar = cmdStr[index];
            switch (objectChar) {
                case 'w': cmd.object = evoWord; break;
                case 'b': cmd.object = evoBackWord; break;
                case 'e': cmd.object = evoEndWord; break;
                case '$': cmd.object = evoToLineEnd; break;
                case '0': cmd.object = evoToLineStart0; break;
                case '^': cmd.object = evoToLineStartNonBlank; break;
                case 'l': cmd.object = evoCharacter; break; // move right
                case 'h': cmd.object = evoCharacter; break; // move left (TODO: directionality for evoCharacter?)
                // TODO: Inner/A text objects like "iw", "aw", "i(", "a(" etc. would need more logic
                default:
                    cmd.action = evcUndefined; // Invalid object for d,y,c
                    goto end_parse;
            }
            index++;
        } else {
             // 'd', 'y', 'c' by themselves are invalid/incomplete.
             // This should have been caught by VimParseIncompleteCmd ideally,
             // but VimCompiler confirms it.
            cmd.action = evcUndefined;
        }
    }

end_parse:
    if (index < cmdStr.length()) {
        // Trailing characters that were not parsed. This means the command string was not fully understood.
        // e.g. "dwx" - "dw" is parsed, "x" is trailing.
        // For a strict compiler, this could be an error.
        // For now, we assume the initial part is what was intended if it forms a valid command.
        // Or, mark as error:
        // cmd.action = evcUndefined; 
    }
    // SetStatusText(wxString::Format("VimCompiler: %s -> cnt:%d, act:%d, obj:%d, prm:%c", 
    //    cmdStr.c_str(), cmd.count, cmd.action, cmd.object, cmd.param), 1);
}


// Helper function to get text for a given motion.
// This is a simplified version. A real one would be more complex.
// Returns wxString to be compatible with input control methods.
wxString MyFrame::GetTextForMotion(StVimCmd& cmd, long& startPos, long& endPos) {
    startPos = input->GetInsertionPoint();
    long originalPos = startPos;
    endPos = startPos; // Default if motion doesn't move

    // This logic is highly simplified and needs proper implementation for each object type
    // It should determine the range of text the motion covers.
    // For simplicity, let's assume some motions for now.
    switch(cmd.object) {
        case evoLine:
            {
                long currentLineNum = input->GetCurLineNumber();
                long lastLineNum = currentLineNum + cmd.count -1;
                if (lastLineNum >= input->GetNumberOfLines()) lastLineNum = input->GetNumberOfLines() -1;
                
                startPos = input->XYToPosition(0, currentLineNum);
                endPos = input->XYToPosition(0, lastLineNum +1 ); // Position after the last char of last line
                if (endPos > input->GetLastPosition() && lastLineNum < input->GetNumberOfLines()) { // if not the very end of file
                     endPos = input->XYToPosition(input->GetLineLength(lastLineNum), lastLineNum);
                     if (lastLineNum < input->GetNumberOfLines() -1) endPos++; // include newline unless it's the last line and no newline
                } else if (endPos > input->GetLastPosition()) {
                     endPos = input->GetLastPosition();
                }

            }
            break;
        case evoWord:
            // Simplified: get current word or cmd.count words
            // A proper implementation would use NextPhrase/PrevPhrase carefully
            endPos = startPos;
            for (int i = 0; i < cmd.count; ++i) {
                long nextWordPos = NextPhrase(); // NextPhrase needs to return new position
                if (nextWordPos != -1 && nextWordPos > endPos) {
                    endPos = nextWordPos;
                } else {
                    endPos = input->GetLastPosition(); // Or end if no more words
                    break;
                }
            }
             //This is tricky: NextPhrase moves caret. We need a version that calculates range.
             // For now, let's assume a GetCurPhrasePos like logic
            GetCurPhrasePos(startPos, endPos); // This gets current phrase, not count
            if (cmd.count > 1) SetStatusText("Yank for count > 1 on word not fully implemented",1);

            break;
        case evoToLineEnd:
            GetLineStartEndPos(startPos, endPos, 0); // Gets current line start/end
            startPos = originalPos; // Yank from cursor
            break;
        case evoCharacter:
            endPos = startPos + cmd.count;
            if (endPos > input->GetLastPosition()) endPos = input->GetLastPosition();
            break;
        default:
            // For other objects, we'd need more logic.
            // Fallback to no text or single character.
            endPos = startPos; // effectively empty or error
            break;
    }
    if (startPos > endPos) std::swap(startPos, endPos); // Ensure start <= end
    return input->GetRange(startPos, endPos);
}


// Implementation for VimExeCmd
void MyFrame::VimExeCmd(StVimCmd& cmd)
{
    bool bCmdHandled = false;
    long initialPos = input->GetInsertionPoint(); // Store initial position for some ops

    switch (cmd.action) {
        case evcDelete:
            for (int i = 0; i < cmd.count; ++i) { // Apply count for the WHOLE operation typically
                                                // but for some, count is part of motion (e.g. d2w)
                                                // Here, cmd.count should already be set by Compiler for d2w to mean 2 words.
                                                // So, the loop here should be 1 unless cmd itself is repeated (e.g. from a macro later)
                if (i > 0) initialPos = input->GetInsertionPoint(); // update for repeated full commands

                switch (cmd.object) {
                    case evoWord:
                        // For "dw", cmd.count is 1. If "2dw", cmd.count is 2.
                        // So the loop for cmd.count is already handled by the compiler setting cmd.count.
                        // We just need to delete one "object unit" here, which might span multiple words if count was on motion.
                        // This interpretation means the outer cmd.count loop should mostly be 1.
                        // Let's assume compiler sets cmd.count correctly for "2dw" etc.
                        // Thus, the loop `for (int i = 0; i < cmd.count; ++i)` above might be redundant if count is part of motion.
                        // For "dd", count is for lines. For "d2w", count is 2 for words.
                        // The current compiler logic sets cmd.count for d(2w) -> cmd.count = 2, cmd.object = evoWord
                        // Let's simplify: the loop for (int i=0; i<cmd.count... ) applies to the "motion" part.
                        // This means DeleCaretChar(cmd.count) for evoCharacter is correct.
                        // DoDeleLine(cmd.count) for evoLine is correct.
                        // For evoWord with "d2w" (cmd.count=2), we need to delete 2 words.
                        {
                            long startDel, endDel;
                            for(int w=0; w<cmd.count; ++w) {
                                if(w>0) input->SetInsertionPoint(startDel); // after first word deletion, cursor is at startDel
                                if (!GetCurPhrasePos(startDel, endDel) || startDel == endDel) break; // No phrase or empty
                                input->Remove(startDel, endDel);
                                if(w < cmd.count -1 && startDel == input->GetLastPosition()) break; // if deleted to end
                            }
                            bCmdHandled = true;
                        }
                        break;
                    case evoLine: DoDeleLine(cmd.count); bCmdHandled = true; break;
                    case evoToLineEnd: DeleToLineEnd(); bCmdHandled = true; break;
                    case evoToLineStart0: DeleToLineStart(); bCmdHandled = true; break;
                    case evoCharacter: DeleCaretChar(cmd.count); bCmdHandled = true; break; // 'x' or 'dl'
                    case evoFindCharForward: // dtf, dff etc.
                    case evoFindCharBackward:
                    case evoTillCharForward:
                    case evoTillCharBackward:
                        {
                            long currentPos = input->GetInsertionPoint();
                            long targetPos = currentPos;
                            bool found = false;
                            for(int k=0; k < cmd.count; ++k){ // for cases like d2fx
                                targetPos = DoFindChar(cmd.param, (cmd.object == evoFindCharBackward || cmd.object == evoTillCharBackward));
                                if(targetPos == -1) { found=false; break;}
                                found = true;
                                if (cmd.object == evoTillCharForward && targetPos > currentPos) targetPos--; // before char
                                else if (cmd.object == evoTillCharBackward && targetPos < currentPos) targetPos++; // before char (after if moving left)
                                if (k < cmd.count -1 ) input->SetInsertionPoint(targetPos + ((cmd.object == evoFindCharBackward || cmd.object == evoTillCharBackward) ? -1:1) ); // move for next find
                            }

                            if (found) {
                                if (cmd.object == evoFindCharForward || cmd.object == evoTillCharForward) { // Forward
                                    if (targetPos > currentPos) input->Remove(currentPos, targetPos + (cmd.object == evoFindCharForward ? 1:0) );
                                } else { // Backward
                                    if (targetPos < currentPos) input->Remove(targetPos + (cmd.object == evoFindCharBackward ? 0:1), currentPos);
                                }
                                bCmdHandled = true;
                            } else {
                                wxBell(); // Not found
                            }
                        }
                        break;
                    default: SetStatusText("Delete for this object not implemented",0); break;
                }
            } // This outer loop for cmd.count might be removed if count is always part of the motion.
              // For now, it's somewhat ambiguous how "2dw" (cmd.count=2, from compiler) vs "d2w" (cmd.count=2 from compiler)
              // For "dd", cmd.count is 1. "2dd" -> cmd.count=2. DoDeleLine(cmd.count) is correct.
              // For "dw", cmd.count is 1. "d2w" -> cmd.count=2. The inner word deletion loop handles cmd.count.
              // So the outer loop `for (int i=0;...` should effectively run once for most d/y/c commands
              // if the compiler correctly sets cmd.count from things like "2w".
              // The current compiler sets cmd.count from "2" in "2dw".
              // Let's assume the outer loop is for repeating the *entire* StVimCmd, which is usually 1.
              // And cmd.count inside StVimCmd is for the motion's repetition.
              // This means the top loop `for (int i = 0; i < cmd.count; ++i)` should be `for (int i = 0; i < 1; ++i)`
              // and rely on cmd.count being correctly set and used by object handlers.
              // Re-evaluating: The prompt implies `cmd.count` applies to the action. So `2dw` means "delete word, twice".
              // This is different from `d2w` which means "delete 2 words, once".
              // The current compiler is likely parsing `2dw` as count=2, action=d, object=w.
              // This interpretation is used below.
            break;

        case evcChange:
            // Similar to delete, then EnterEditMode()
            {
                bool deleted = false;
                for (int i = 0; i < cmd.count; ++i) { // See notes on count for evcDelete
                     switch (cmd.object) {
                        case evoWord:
                        {
                            long startDel, endDel;
                            for(int w=0; w<cmd.count; ++w) { // cmd.count from "c2w"
                                if(w>0) input->SetInsertionPoint(startDel);
                                if (!GetCurPhrasePos(startDel, endDel) || startDel == endDel) break;
                                input->Remove(startDel, endDel);
                                deleted = true;
                                if(w < cmd.count -1 && startDel == input->GetLastPosition()) break;
                            }
                        }
                        break;
                        case evoLine: DoDeleLine(cmd.count); deleted = true; break; // c2c -> cmd.count=2
                        case evoToLineEnd: DeleToLineEnd(); deleted = true; break; // C
                        // ... other objects
                        default: SetStatusText("Change for this object not implemented",0); break;
                    }
                    if(!deleted && i < cmd.count -1) break; // if one iteration did nothing, stop
                }
                 if(deleted || cmd.object == evoUndefined) EnterEditMode(); // Enter edit mode if text was changed or if it's a change command that implies it (e.g. 'cw' even if word is empty)
                bCmdHandled = true;
            }
            break;

        case evcYield: // Yank
            {
                long startPos, endPos;
                wxString yanked = GetTextForMotion(cmd, startPos, endPos);
                if (!yanked.empty() || startPos != endPos) { // Allow yanking empty if range is valid
                    m_vimYankBuffer = yanked.ToStdString();
                    SetStatusText(wxString::Format("Yanked: %d chars", (int)m_vimYankBuffer.length()),0);
                    input->SetInsertionPoint(initialPos); // Restore cursor after calculating range
                } else {
                    SetStatusText("Nothing to yank for motion.",0);
                }
                bCmdHandled = true;
            }
            break;

        case evcPut:
            if (!m_vimYankBuffer.empty()) {
                bool isLineWise = (m_vimYankBuffer.find('\n') != std::string::npos);
                for (int i = 0; i < cmd.count; ++i) {
                    long currentPos = input->GetInsertionPoint();
                    if (isLineWise) {
                        MoveCaretToLineBottom(); // Go to end of current line
                        if (input->GetInsertionPoint() == input->GetLastPosition() && input->GetValue().Last() != '\n'){
                             input->WriteText("\n"); // Add newline if at EOF without one
                        }
                        input->WriteText("\n" + wxString::FromUTF8(m_vimYankBuffer.c_str()));
                        // Cursor should be at start of newly pasted content, or after for charwise
                    } else {
                        if (currentPos < input->GetLastPosition()) input->SetInsertionPoint(currentPos + 1);
                        input->WriteText(wxString::FromUTF8(m_vimYankBuffer.c_str()));
                    }
                }
            } else {
                SetStatusText("Yank buffer is empty",0);
            }
            bCmdHandled = true;
            break;

        case evcFind: // Standalone f, F, t, T (compiler sets cmd.object for these)
            {
                long finalPos = -1;
                for(int k=0; k < cmd.count; ++k) {
                    bool backward = (cmd.object == evoFindCharBackward || cmd.object == evoTillCharBackward);
                    long foundPos = DoFindChar(cmd.param, backward);
                    if (foundPos != -1) {
                        finalPos = foundPos;
                        if (cmd.object == evoTillCharForward && finalPos > input->GetInsertionPoint()) finalPos--;
                        else if (cmd.object == evoTillCharBackward && finalPos < input->GetInsertionPoint()) finalPos++;
                        
                        input->SetInsertionPoint(finalPos);
                        if (k < cmd.count -1) { // If more finds needed, advance cursor slightly to find next
                           if (backward) input->SetInsertionPoint(finalPos > 0 ? finalPos -1 : 0);
                           else input->SetInsertionPoint(finalPos < input->GetLastPosition() ? finalPos + 1: input->GetLastPosition());
                        }
                    } else {
                        finalPos = -1; // Not found
                        wxBell();
                        break;
                    }
                }
                if(finalPos != -1) input->SetInsertionPoint(finalPos); // Ensure cursor is at the final spot
                bCmdHandled = true;
            }
            break;

        case evcMove: // Standalone motions (w, b, 0, $, etc.)
            for (int i = 0; i < cmd.count; ++i) {
                switch (cmd.object) {
                    case evoWord: NextPhrase(); break;
                    case evoBackWord: PrevPhrase(); break;
                    case evoEndWord: /* TODO: Implement MoveToEndWord */ NextPhrase(); if(input->GetInsertionPoint()>0) MoveCaretAhead(-1); break; // Simplified
                    case evoToLineEnd: MoveCaretToLineBottom(); break;
                    case evoToLineStart0: MoveCaretToLineTop(); break;
                    case evoCharacter: MoveCaretAhead(1); break; // 'l'
                    // 'h' would be MoveCaretAhead(-1) - needs distinction in StVimCmd or compiler
                    default: SetStatusText("Move for this object not implemented",0); break;
                }
            }
            bCmdHandled = true;
            break;
            
        case evcReplaceChar: // 'r' command
            if (cmd.param != 0) { // Ensure param character is set
                DeleCaretChar(cmd.count); // Delete characters to be replaced
                for (int i = 0; i < cmd.count; ++i) {
                    input->WriteText(wxString(cmd.param, 1));
                }
            }
            bCmdHandled = true;
            break;

        case evcOpenLineBelow: // 'o'
            MoveCaretToLineBottom();
            input->WriteText("\n");
            EnterEditMode();
            bCmdHandled = true;
            break;

        case evcOpenLineAbove: // 'O'
            MoveCaretToLineTop(); // Go to beginning of current line
            input->WriteText("\n"); // Insert newline above
            MoveCaretUp(-1);      // Move caret to the newly created line
            EnterEditMode();
            bCmdHandled = true;
            break;
        
        case evcInsert: EnterEditMode(); bCmdHandled = true; break;
        case evcAppend: MoveCaretAhead(1); EnterEditMode(); bCmdHandled = true; break;


        default:
            SetStatusText(wxString::Format("Command action '%d' not implemented yet.", cmd.action), 0);
            break;
    }
    if(bCmdHandled) {
        // Optional: Show status based on executed command
        // SetStatusText(wxString::Format("Executed: %c %d %c", cmd.action, cmd.count, cmd.object), 1);
    }
}