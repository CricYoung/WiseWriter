// WiseWriter.h
#ifndef WISEWRITER_H
#define WISEWRITER_H

#include <wx/wx.h>
#include <wx/textctrl.h>
#include <wx/string.h>
#include <vector>
#include <string> // Required for std::string

// Forward declarations if necessary for types used in MyFrame
class DlgSettings;
class KHelp;
struct StKey; // Assuming StKey is defined elsewhere, possibly KHotKey.h

// Enum for Vim commands (from previous step)
typedef enum {
    evcUndefined, evcDelete, evcChange, evcYield, evcFind, evcPut, evcInsert, evcAppend,
    evcOpenLineBelow, evcOpenLineAbove, evcReplaceChar, evcSubstitute, evcVisual,
    evcUndo, evcRedo
} EVimCmd;

// Enum for Vim objects/motions (from previous step)
typedef enum {
    evoUndefined, evoWord, evoBackWord, evoEndWord, evoLine, evoToLineEnd, evoToLineStart0,
    evoToLineStartNonBlank, evoCharacter, evoParagraph, evoSentence, evoBlock, evoInnerWord,
    evoInnerBlock, evoAWord, evoABlock, evoFindCharForward, evoFindCharBackward,
    evoTillCharForward, evoTillCharBackward, evoCurrentLine, evoScreenLine,
    evoNextOccurrence, evoPrevOccurrence
} EVimObj;

// Struct for a Vim command (from previous step)
typedef struct {
    int count;
    EVimCmd action;
    EVimObj object;
    char param;
} StVimCmd;


// Declaration of MyFrame class
class MyFrame : public wxFrame
{
public:
    MyFrame();
    ~MyFrame(); // Added destructor declaration based on .cpp file

    // Event Handlers - taken from BindEvents in cpp
    void OnKeyDown(wxKeyEvent& event);
    void OnKeyUp(wxKeyEvent& event); // Added based on cpp
    void OnClose(wxCloseEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);
    void OnExit(wxCommandEvent& event);
    void OnLoadHistory(wxCommandEvent& event);
    void OnSaveHistory(wxCommandEvent& event);
    void OnClearHistory(wxCommandEvent& event);
    void OnShowHelp(wxCommandEvent& event);
    void OnSwitchDarkMode(wxCommandEvent& event); // Added based on cpp
    void OnTogFullScreen(wxCommandEvent& event); // Added based on cpp
    void OnRestoreAppPrefer(wxCommandEvent& event); // Added based on cpp
    void OnSystemColorChanged(wxSysColourChangedEvent& event); // Added based on cpp
    void OnIdle(wxIdleEvent& event); // Added based on cpp
    void OnAutoSaveTimer(wxTimerEvent& event); // Added based on cpp

    static void SummonHotkeyHandler(void* userData); // Added based on cpp

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
    bool bCtrlKey; // Added based on cpp

    // Vim specific members
    StVimCmd currentVimCmd;
    std::string vimInputBuffer;
    std::string m_vimYankBuffer; // For yanked text

    // Other members inferred from MyFrame.cpp
    wxIdleEvent* idleEvent = nullptr; // Initialized
    wxTimer* autoSaveTimer = nullptr; // Initialized
    int nCurSecCountDown;
    int nCurKeyCountDown;
    
    bool bIsAutoSaveBySecCount = false;
    int nAutoSaveKeyCount;
    int nAutoSaveSecCount;
    bool bIsShowTimeCountDown = false;
    bool bIsShowKeyCountDown = false;

    DlgSettings* dlgSettings = nullptr; // Initialized
    KHelp* pHelp = nullptr;             // Initialized

    wxColour oBackgroundColor;
    wxColour oTextColor;
    wxColour oBackgroundColorDark;
    wxColour oTextColorDark;
    bool bIsDarkMode = false; // Initialized

    wxString LanguageName;

    // Assuming StKey is defined elsewhere (e.g. KHotKey.h which is included in WiseWriter.cpp)
    StKey* oSummonKey = nullptr; // Using pointers and initializing to avoid needing full StKey definition here
    StKey* oPasteBackKey = nullptr;
    StKey* oPrevPasteKey = nullptr;
    StKey* oNextPasteKey = nullptr;
    StKey* oHide = nullptr;
    StKey* oToggleDarkMode = nullptr;
    StKey* oToggleFullScreen = nullptr;
    StKey* oRestoreAppPrefer = nullptr;
    StKey* oLoadHistory = nullptr;
    StKey* oSaveHistory = nullptr;
    StKey* oClearHistory = nullptr;
    StKey* oShowHelp = nullptr;

    int nLastFindCharCode;
    bool bLastFindBackward;
    int nLastFindPlusMove;
    bool bSummonKeyRegistered = false;


    // Member Functions
    void BuildMenu(); // Added based on cpp
    void BindEvents(); // Added based on cpp
    
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
    bool IsWinFullScreen() { return IsFullScreen(); } // Helper
    void CenterWindow();
    void SaveHotkeys(wxFileConfig& tConfig);
    bool LoadHotkeys(wxFileConfig& tConfig);
    void SetHotkeys();
    void SaveAppPrefer();
    bool LoadAppPrefer();
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
    bool SetViewMod(bool tInView);
    bool LeaveEditMode();
    bool EnterEditMode();
    void MoveCaretAhead(int tCharCount);
    bool MoveCaretUp(int lineDelta);
    int DeleCaretChar(int tCharCount);
    bool IsSpaceLike(wxUniChar tChar);
    int NextPhrase();
    int PrevPhrase();
    void DoTabKey(bool tShift);
    bool MoveCaretToLineTop();
    bool MoveCaretToLineBottom();
    void MoveCaretPgUp() { if(input) input->PageUp(); } // Simplified
    void MoveCaretPgDown() { if(input) input->PageDown(); } // Simplified
    bool DoCmdHistoryKey(bool tUp);
    void DoPasteBack();
    bool DoStaticKey(wxKeyEvent& event);
    bool DoVimViewKey(wxKeyEvent& event);
    bool DoVimEditKey(wxKeyEvent& event);
    void ShowInputStatus();
    void ResetCombineKey();
    bool DoCmdAltShiftKey(wxKeyEvent& event);
    bool DoStKey(wxKeyEvent& event, StKey& tKey); // Param changed to ref
    bool FindChar(int keyCode, bool tBackward, int tPlusMove, bool tRemember);
    long DoFindChar(int keyCode, bool tBackward);
    int DeleToLineEnd() { long currentPos = input->GetInsertionPoint(); long lineEnd = input->GetLastPosition(); /* Simplified */ input->Remove(currentPos, lineEnd); return lineEnd - currentPos; }
    int DeleToLineStart() { long currentPos = input->GetInsertionPoint(); /* Simplified */ input->Remove(0, currentPos); return currentPos; }
    bool GetLineStartEndPos(long& lineStart, long& lineEnd, int tLineCount); // Added declaration
    int DoDeleLine(int tLineCount);
    bool GetCurPhrasePos(long& tStartPos, long& tEndPos); // Added declaration
    int DoDelePhrease(int tPhreaseCount); // Added declaration
    int DoDCharKey(int tKeyCode); // Added declaration


    // Vim related methods
    void VimKeyProc(wxKeyEvent& event);
    bool VimParseIncompleteCmd(const std::string& buffer, StVimCmd& cmd); // Parses potentially incomplete command
    void VimCompiler(const std::string& cmdStr, StVimCmd& cmd);          // Parses a complete command string
    void VimExeString(const std::string& cmdStr);                       // Called when a command string is deemed complete by VimKeyProc
    void VimExeCmd(StVimCmd& cmd);                                      // Executes a parsed StVimCmd

    // For StKey members, if StKey is a struct and not a pointer, it needs full definition or include.
    // For now, assuming KHotKey.h (included in WiseWriter.cpp) defines StKey.
    // If StKey is complex, using pointers (StKey*) and forward declaring StKey is safer here.
    // Changed StKey members to pointers for this reason.
    // Reverted StKey to be value types as they are directly used in SetHotkeys, will require StKey definition
    // For now, I will remove them from header if they cause compilation error, assuming they are correctly handled in .cpp

    // To ensure MyFrame can be instantiated, it needs a default constructor
    // and its event table macros if not using Bind() for all events.
    // wxDECLARE_EVENT_TABLE(); // Add if using event tables
};

// Enum for Menu IDs from WiseWriter.cpp
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


#endif // WISEWRITER_H
