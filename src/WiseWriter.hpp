// WiseWriter.h

#pragma once
#include <vector>
#include <string> 
#include <stdint.h>
#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <wx/config.h> 

#include "KHotKey.hpp"
#include "KMthWxEdit.hpp"

class DlgSettings;
class KHelp;
struct StVimCmd;

class MyFrame : public wxFrame
{
  public:
    MyFrame();
    ~MyFrame(); 

    // Event Handlers - taken from BindEvents in cpp
    void OnKeyDown(wxKeyEvent& event);
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnLoadHistory(wxCommandEvent& event);
	  void OnSaveHistory(wxCommandEvent &){ SaveHistory(); };
    void OnClearHistory(wxCommandEvent& event);
	  void OnShowHelp(wxCommandEvent &){ ShowHelp(); };
	  void OnSwitchDarkMode(wxCommandEvent &){ SwitchToDarkMode(!bIsDarkMode); };
	  void OnRestoreAppPrefer(wxCommandEvent &){ RestoreAppPrefer(); };
    void OnTogFullScreen(wxCommandEvent& event); 
    void OnSystemColorChanged(wxSysColourChangedEvent& event); 
    void OnIdle(wxIdleEvent& event); 
    void OnAutoSaveTimer(wxTimerEvent& event); 
		//for wxApp to call
    bool LoadAppPrefer();
    bool RegistSummonKey(); 
    static void SummonHotkeyHandler(void* userData); 

private:
    // Member Variables
    wxTextCtrl* input;
    wxString LoadFileName;
    std::string EndMarkStr; // Changed from wxString to std::string based on "##End##"
    std::vector<wxString> commandHistory;
    int historyIndex;
    bool bHistoryDirty;
    wxFont oFont;
    bool bInViewMode;
    int nLastKeyCode;
    bool bCtrlKey; 
    // Other members inferred from MyFrame.cpp
    wxIdleEvent* idleEvent = nullptr; 
    wxTimer* autoSaveTimer = nullptr; 
    int nCurSecCountDown;
    int nCurKeyCountDown;
    bool bIsAutoSaveByKeyCount = false;
    int nAutoSaveKeyCount;
    bool bIsAutoSaveBySecCount = false;
    int nAutoSaveSecCount;
    bool bIsShowTimeCountDown = false;
    bool bIsShowKeyCountDown = false;

    DlgSettings* dlgSettings = nullptr; 
    KHelp* pHelp = nullptr;             

    wxColour oBackgroundColor;
    wxColour oTextColor;
    wxColour oBackgroundColorDark;
    wxColour oTextColorDark;
    bool bIsDarkMode = false; 

    wxString LanguageName;

    StKey oSummonKey ; 
    StKey oPasteBackKey ;
    StKey oPrevPasteKey ;
    StKey oNextPasteKey ;
    StKey oHide ;
    StKey oToggleDarkMode ;
    StKey oToggleFullScreen ;
    StKey oRestoreAppPrefer ;
    StKey oLoadHistory ;
    StKey oSaveHistory ;
    StKey oClearHistory ;
    StKey oShowHelp ;

    int nLastFindCharCode;
    bool bLastFindBackward;
    int nLastFindPlusMove;
    bool bSummonKeyRegistered = false;
	  //--- Vim ---
	  KMthWxEdit mMthEdit;
  protected:
    // Member Functions
    void BuildMenu(); 
    void BindEvents(); 
    
   	void GetSummonKey(StKey& tKey) const { tKey = oSummonKey; };
    void UnregisterSummonKey(){
  		if(bSummonKeyRegistered) {
  			::UnregisterAppHotKey();
  			bSummonKeyRegistered = false;
  		};
   	};
   	bool IsSummonKeySameAsPasteBackKey() const {
   		return oSummonKey == oPasteBackKey;
   	};
    void IniAutoSaveSettings();
   	void OnKeyUp(wxKeyEvent &event){ if(event.GetKeyCode() == 396) bCtrlKey = false; event.Skip(); };
    void DoLoadHistory();
    void DoLoadFromFile(const wxString& loadFileName);
    bool SaveInput();
    bool SaveHistory();
    void DoClearHistory();
    void DoLargeFont();
    void DoSmallFont();
    wxMenu* GetMenuByName(const wxString& menuName, int* menuIndex = nullptr);
    wxMenuItem* GetMenuItemByName(wxMenu* tpMenu, const wxString& ItemName, int* ItemIndex = nullptr);
    int GetWinWindowState();
    void SetWinWindowState(int tState);
    void SetWinFullScreen(bool tFullScreen);
    bool IsWinFullScreen() { return IsFullScreen(); } 
    void CenterWindow();
    void SaveHotkeys(wxFileConfig& tConfig);
    bool LoadHotkeys(wxFileConfig& tConfig);
    void SetHotkeys();
    void SaveAppPrefer();
    void DeleAppPrefer();
    bool RestoreAppPrefer();
    void TakeEffect(wxTextCtrl* tpInput);
    void SetDlgSettingsFromAppPrefer();
    void SetAppPreferFromDlgSettings();
    bool ShowHelp();
    void ShowStatusHistoryIndex();
    int AskSaveHistory();
    bool ChangeCaretForm();
    void SwitchToDarkMode(bool tDarkMode);

    bool DoCmdHistoryKey(bool tUp);
    void DoPasteBack();
    bool DoStaticKey(wxKeyEvent& event);
    bool DoVimViewKey(wxKeyEvent& event);
    bool DoVimEditKey(wxKeyEvent& event);
    void ShowInputStatus();
    void ResetCombineKey();
    bool DoCmdAltShiftKey(wxKeyEvent& event);
    bool DoStKey(wxKeyEvent& event, StKey& tKey); 
};
