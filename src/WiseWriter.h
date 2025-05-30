// WiseWriter.h
#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
#include "DlgSettings.h"
#include "KHotKey.h"
#include "KHelp.hpp"

#define CIniFileName "WiseWriter.ini"
#define CUsrIniFileName "UsrWiseWriter.ini"
const int CMinIdleEventActiveTime = 3000; // Minimum idle event time
const int CCheckTimerPerMiniSec = 10000; // Minimum check timer per millisecond
const int CCheckTimerPerSec = CCheckTimerPerMiniSec/1000; // Minimum check timer per second

class MyFrame : public wxFrame
{
private:
	wxString LanguageName; // Language name(English, 繁體中文, 日本語, 한국어, etc.)) 
	wxString LoadFileName; // File name to load history
	std::vector<wxString> commandHistory;
	wxString EndMarkStr; // End of block text mark
	int historyIndex;
	bool bHistoryDirty = false; // Flag: History is dirty or not
	wxFont oFont;
	wxColor oTextColor;
	wxColor oBackgroundColor;
	wxColor oTextColorDark;
	wxColor oBackgroundColorDark;
	bool bIsDarkMode = false;
	DlgSettings *dlgSettings = nullptr; // Settings dialog
	bool bInViewMode = false; // input editor mode: View or Edit
	int nLastKeyCode = -1; // Last key code
	bool bCtrlKey = false; // Ctrl key pressed
	StKey oSummonKey; // Summon key
	StKey oPasteBackKey; // Paste back key
	StKey oPrevPasteKey; // Previous paste key
	StKey oNextPasteKey; // Next paste key
	StKey oHide;
	StKey oToggleDarkMode;
	StKey oToggleFullScreen;
	StKey oRestoreAppPrefer;
	StKey oLoadHistory;
	StKey oSaveHistory;
	StKey oClearHistory;
	StKey oShowHelp;
	bool bSummonKeyRegistered = false; // Summon key registered
	// auto save
	bool bIsAutoSaveByKeyCount=true;
	int nAutoSaveKeyCount=100;
	int nCurKeyCountDown=CMinAutoSaveKeyCount;
	bool bIsAutoSaveBySecCount=true;
	int nAutoSaveSecCount=100;
	int nCurSecCountDown=CMinAutoSaveSecCount;
	wxTimer *autoSaveTimer = nullptr; // Timer for auto save
	wxIdleEvent *idleEvent = nullptr; // Idle event for auto save
	bool bIsShowTimeCountDown = false; // Show time countdown
	bool bIsShowKeyCountDown = false; // Show key countdown
	// find char : t T f F vim key
	int nLastFindCharCode = -1; // Last find char code
	bool bLastFindBackward = false; // Last find backward
	int nLastFindPlusMove = 0; // Last find plus move
	// KHelp
	KHelp* pHelp=nullptr; // Help object

public:
	wxTextCtrl *input;

	MyFrame();
	~MyFrame();
  void BuildMenu();
	void BindEvents();
	void OnClose(wxCloseEvent& event);
	void OnExit(wxCommandEvent &event);

	//system Menu
	void OnSettings(wxCommandEvent &event);
	void OnAbout(wxCommandEvent &event);
	void OnShowHelp(wxCommandEvent &){ ShowHelp(); };
	// file Menu
	void OnLoadHistory(wxCommandEvent &event);
	void DoLoadHistory();
	void OnSaveHistory(wxCommandEvent &){ SaveHistory(); };
	void OnClearHistory(wxCommandEvent &event);
	void DoClearHistory();

	void OnTogFullScreen(wxCommandEvent &);
	void OnSwitchDarkMode(wxCommandEvent &){ SwitchToDarkMode(!bIsDarkMode); };
	void OnRestoreAppPrefer(wxCommandEvent &){ RestoreAppPrefer(); };

	// auto save
	void IniAutoSaveSettings();
	void OnIdle(wxIdleEvent &event);
	void OnAutoSaveTimer(wxTimerEvent &event);
	// other Events
	void OnKeyDown(wxKeyEvent &event);
	void OnKeyUp(wxKeyEvent &event){ if(event.GetKeyCode() == 396) bCtrlKey = false; event.Skip(); };
	virtual void OnSystemColorChanged(wxSysColourChangedEvent& event); // 系統顏色改變事件(如深色模式)
	//=== helper functions ===
	void GetSummonKey(StKey& tKey) const { tKey = oSummonKey; };
	// menu helper
	wxMenu* GetMenuByName(const wxString& menuName,int* menuIndex=0);
	wxMenuItem* GetMenuItemByName(wxMenu* tpMenu,const wxString& menuName, int* menuIndex=0);
	// preferences
	void SaveAppPrefer();
	void SaveHotkeys(wxFileConfig& tConfig);
	bool LoadAppPrefer();
	bool LoadHotkeys(wxFileConfig& tConfig);
	void SetHotkeys();
	bool RestoreAppPrefer();
	void DeleAppPrefer();
	void SetDlgSettingsFromAppPrefer();
	void SetAppPreferFromDlgSettings(); 
	void TakeEffect(wxTextCtrl* tInput); // Set font, color, and others by app prefer
	// file helper
	void DoLoadFromFile(const wxString& tFileName);
	int AskSaveHistory(); // return 0 cancel, 1 save, 2 don't save, 3 no need to save
	bool SaveHistory();
	bool SaveInput();
	// display
	void DoLargeFont();
	void DoSmallFont();
  void SwitchToDarkMode(bool tDarkMode=true);
	int GetWinWindowState(); // 0: normal, 1: minimized, 2: maximized
	void SetWinWindowState(int tState); // 0: normal, 1: minimized, 2: maximized
	bool IsWinFullScreen(){ wxPoint pos = GetScreenPosition(); return pos.x <= 2 && pos.y <= 2; }; 
	void SetWinFullScreen(bool tFullScreen);
	void CenterWindow();
	bool ShowHelp();
	void ShowStatusHistoryIndex();
	bool ChangeCaretForm(); // no use for TextCtrl
	void ShowInputStatus();
	// VIM like functions
	bool SetViewMod(bool tInView);
	bool LeaveEditMode();
	bool EnterEditMode();
	long GetLineStartPos(){
		long currentPos = input->GetInsertionPoint();
		long tX,tY;
		input->PositionToXY(currentPos,&tX,&tY);
		return input->XYToPosition(0, tY);
	};
	long GetLineEndPos(long tLineCount=1){
		long currentPos = input->GetInsertionPoint();
		long tX,tY;
		input->PositionToXY(currentPos,&tX,&tY);
		return input->XYToPosition(0, tY + tLineCount);
	};
	bool GetLineStartEndPos(long& tStartPos, long& tEndPos,long tLineCount=1){
		long currentPos = input->GetInsertionPoint();
		long tX,tY;
		input->PositionToXY(currentPos,&tX,&tY);
		tStartPos = input->XYToPosition(0, tY);
		tEndPos= input->XYToPosition(0, tY + tLineCount); 
		return true;
	};
	void MoveCaretAhead(int tCharCount);
	int DeleCaretChar(int tCharCount);
	int DeleToLineEnd(){
		long tCurPos=input->GetInsertionPoint();
		long tLineEndPos = GetLineEndPos();
		input->Remove(tCurPos,tLineEndPos-1); // -1 for the last char '\n'
		return  tLineEndPos - tCurPos;
	}
	int DeleToLineStart(){
		long tCurPos=input->GetInsertionPoint();
		long tLineStartPos = GetLineStartPos();
		input->Remove(tLineStartPos, tCurPos);
		return tCurPos - tLineStartPos;
	};
	bool MoveCaretUp(int tLineCount); // negative for up, positive for down
	void MoveCaretPgUp(){ MoveCaretUp(-(input->GetNumberOfLines()/2)); };
	void MoveCaretPgDown(){ MoveCaretUp(input->GetNumberOfLines()/2); };
	long DoFindChar(int tCharCode,bool tBackward=false); // return -1 if not found, return new position if found
	bool FindChar(int tCharCode,bool tBackward=false,int tPlusMove=0,bool tRemember=true);
	int DoDeleLine(int tLineCount=1);
	int DoDCharKey(int tKeyCode);
	bool GetCurPhrasePos(long& tStartPos, long& tEndPos);  // phrase may be SpcLike or Alphabet phrase
	int DoDelePhrease(int tPhreaseCount=1);
	int NextPhrase();
	int PrevPhrase();
	void DoPasteBack();
	bool DoCmdHistoryKey(bool tUp);
	bool IsSpaceLike(wxUniChar tChar);
	bool MoveCaretToLineTop();
	bool MoveCaretToLineBottom();
	bool DoStKey(wxKeyEvent &event, StKey& tKey);
	void DoTabKey(bool tShift = false);
	bool DoStaticKey(wxKeyEvent &event);
	bool DoVimViewKey(wxKeyEvent &event);
	bool DoVimEditKey(wxKeyEvent &event);
	bool DoVimCombineKey(wxKeyEvent &event);
	bool DoCmdAltShiftKey(wxKeyEvent &event); //special key
	bool IsInputDirty() const { return input->IsModified(); };
	void ResetCombineKey();
	bool RegistSummonKey();
	void UnregisterSummonKey(){
		if(bSummonKeyRegistered) {
			::UnregisterAppHotKey();
			bSummonKeyRegistered = false;
		};
	};
	bool IsSummonKeySameAsPasteBackKey() const {
		return oSummonKey == oPasteBackKey;
	};
	static void SummonHotkeyHandler(void *userData);
};
