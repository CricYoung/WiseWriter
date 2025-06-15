
#pragma once

#include <wx/wx.h>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <memory>

#include "KMthVim.hpp" // 引入 KMthVim : TextCtrl 的抽象基類，作為 KVim 呼叫 Editor 的介面

// Vim 指令結構
// Vim has 2 condition: 1st Normal mode, 2nd Visual mode (select mode)
// stage 0: Exe immediately at the first char
// In Normal mode stage 0 (Exe immediately) 
const char gVimNormalCmdCharStage0[] = {
    'p', 'P', 
    'i', 'I', 'a', 'A', 'o', 'O', 
    'l', 'L', 'h', 'H', 'k', 'K', 'b', 'B', 'w', 'W', 'e', 'E', 'G', 
    '~',
    0
};

// In Visual mode stage 0 (Exe immediately)
const char gVimVisualCmdCharStage0[] = {
    'p', 'P', 
    'i', 'I', 'a', 'A', 'o', 'O', 
    'l', 'L', 'h', 'H', 'k', 'K', 'b', 'B', 'w', 'W', 'e', 'E', 'G', 
    '~',
    0
};

// Vim command is like: [count] [command] [count] [object]
// fomation as : [stage 1] [stage 2] [stage 3] [stage 4]
// stage 1: Count number to repeat the command
// STAGE 2: Command character
//   chars like : d, y, c, p, i, a, o, O, r, s, l, h, j, k, b, w, e, g, G, H, L, M, f, F, t, T, x, ...and more
// stage 3: Count number to repeat the command object
// stage 4: Command object
//   objects are: line, word, character, toLineEnd, toLineStart
//   chars like : i, w, (, ), {, }, [, ], 0, ^, $, ...and more

// Available Vim chars at stage 1
// is number of chars

// Available Vim char at stage 2
const char gVimNormalCmdChar[] = {
  // move caret
  'h', 'j', 'k', 'l',    // 左右上下
  'w', 'W', 'b', 'B',    // word/WORD
  'e', 'E',               // word end/WORD end
  '0', '^', '$',         // 行首，非空白行首，行尾
  'g', 'G', 'H', 'M', 'L', // 視窗跳躍
  'f', 'F', 't', 'T',    // find/till
  '%',                   // 括號跳轉
  '{', '}',              // 段落
  '+', '-', '_',         // 行移動
  '(', ')', '{', '}', '[', ']', // bracket
  '_', // line move
  '\'', '`', // mark 
  // edit 
  'i', 'I', 'a', 'A',    // insert/append
  'o', 'O',             // open line
  'r', 'R',             // replace
  's', 'S',             // substitute
  'x', 'X',             // delete char
  'd', 'D',             // delete
  'c', 'C',             // change
  '>', '<', '=', 'J', // block indent
  '~', // switch case
  // search or repeat
  '/', '?',     // 搜尋
  'n', 'N', '*', '#', // search
  '.', ',', ';', ':', '!', '?', // repeat
  // copy/paste
  'p', 'P',             // paste
  'y', 'Y',             // yank
  'm', 'M',         // mark
  // other
  'z', 'Z',             // Close window
  '\\', // escape
  'u', 'U',             // undo
  'v', 'V',             // visual
  '%',             // match bracket
  'Q', 'q', // macro
  // stage 0
  'v', 'V', // visual
  'a', 'A', // append
  'o', 'O', // open line
  'i', 'I', // insert
    0
};

// Available Vim char at stage 2 for Visual mode
const char gVimVisualCmdChar[] = {
    'd', 'y', 'c', 'p', 'r', 's', // 直接作用
    'l', 'h', 'j', 'k', 'b', 'w', 'e', 'g', 'B', 'G',
    'H', 'L', 'M', 'W', 'x', 'X', 'u', 'U', '~',
    'f', 'F', 't', 'T', 'z', 'n', 'N', '*', '#',
    '>', '<', '=', 'J', // block indent
    ':', ';', ',', '[', ']', '^', '$', '0', '_',
    'A', 'I', 'C', 'D', 'S', 'Y',
    // 進入 Visual/Block 不要放這, 已進入的情況
    0
};

// Vim 模式枚舉
enum EVimMode {
    evmNormal,      // 普通模式
    evmInsert,      // 插入模式
    evmVisual,      // 視覺模式
    evmVisualLine,  // 視覺行模式
    evmVisualBlock, // 視覺塊模式
    evmCommand,     // 命令行模式
    evmOperatorPending, // 操作符等待模式
    evmReplace      // 替換模式
};

// Vim 指令枚舉
enum EVimCmd {
    evnUndefined,
    // Normal Mode Commands
    evnDelete, evnChange, evnYank, evnFind, evnReplace,
    evnSubstitute, evnMove, evnOpenLine, evnInsert, evnAppend,
    evnDeleteChar, evnDeleteLine, evnChangeLine, evnYankLine,
    evnJoin, evnIndent, evnUnindent, evnFormat, evnToggleCase,
    evnUndo, evnRedo, evnRepeat, evnMark, evnGoToMark,
    evnSearch, evnSearchNext, evnSearchPrev, evnSearchUnderCursor,
    evnFindChar, evnFindCharBackward, evnTillChar, evnTillCharBackward,
    evnMatchBracket, evnGoToLine, evnGoToTop, evnGoToBottom,
    evnCenterScreen, evnScrollUp, evnScrollDown,
    // Visual Mode Commands
    evvUndefined, evvLine, evvWord, evvBlock, evvChar,
    evvDelete, evvYank, evvChange, evvIndent, evvUnindent,
    evvJoin, evvToggleCase, evvReplace
};

// Vim 物件枚舉
enum EVimObj {
    evoUndefined,
    evoChar, evoWord, evoWORD, evoLine, evoSentence, evoParagraph,
    evoBlock, evoQuotedString, evoParentheses, evoBrackets, evoBraces,
    evoAngleBrackets, evoToLineEnd, evoToLineStart, evoToWordStart,
    evoToWordEnd, evoToWORDStart, evoToWORDEnd, evoInnerWord,
    evoAroundWord, evoInnerWORD, evoAroundWORD, evoInnerSentence,
    evoAroundSentence, evoInnerParagraph, evoAroundParagraph,
    evoInnerParentheses, evoAroundParentheses, evoInnerBrackets,
    evoAroundBrackets, evoInnerBraces, evoAroundBraces,
    evoInnerAngleBrackets, evoAroundAngleBrackets,
    evoInnerQuotedString, evoAroundQuotedString
};

// Vim 指令階段枚舉
enum EVimCmdStage {
    evsIdle,        // 閒置狀態
    evsCount1,      // 第一個計數
    evsCommand,     // 指令字符
    evsCount2,      // 第二個計數
    evsObject,      // 物件/範圍
    evsParameter,   // 參數（如搜尋字串、替換字符等）
    evsComplete     // 指令完成
};

// Vim 指令結構
struct StVimCmd {
    int ActCount;           // 動作計數
    EVimCmd xAct;          // 動作類型
    int ObjCount;          // 物件計數
    EVimObj xObj;          // 物件類型
    char param;            // 參數字符
    std::string searchStr; // 搜尋字串
    bool isRepeat;         // 是否為重複指令
    
    StVimCmd();
    StVimCmd(int c, EVimCmd a, EVimObj o, char p = 0){ SetVimCmd(c, a, o, p); };
    void StVimCmd(int c, EVimCmd a, EVimObj o, char p, const std::string& s) {
      ActCount = c;
      xAct = a;
      ObjCount = 0;
      xObj = o;
      param = p;
      searchStr.clear();
      isRepeat = false;
    };
    void Clear(){
      ActCount = 0;
      xAct = evnUndefined;
      ObjCount = 0;
      xObj = evoUndefined;
      param = 0;
      searchStr.clear();
      isRepeat = false;
    };
    bool IsValid() const {
        return (xAct != evnUndefined && xObj != evoUndefined);
    }
    bool IsValidAction() const {
        return (xAct != evnUndefined);
    }
    bool IsValidObject() const {
        return (xObj != evoUndefined);
    }
    bool RequiresObject() const {
        return (xAct == evnDelete || xAct == evnChange || xAct == evnYank ||
                xAct == evnReplace || xAct == evnSubstitute || xAct == evnFind ||
                xAct == evnFindChar || xAct == evnFindCharBackward ||
                xAct == evnTillChar || xAct == evnTillCharBackward);
    }
    bool IsValidCount() const {
        return (ActCount > 0);
    }
    bool IsValidParam() const {
        return (param != 0);
    }
    bool IsValidSearchStr() const {
        return (!searchStr.empty());
    }
};

// Vim 指令資訊結構
struct StVimCmdInfo {
    char cmdChar;
    EVimCmd xAct;
    // bool requiresObject; // not used: if not exe immediately it's required
    // bool canHaveCount;  // not used: every command can have count, yet handler can determine to ignore it or not
    // EVimMode validInMode; // not used: each mode has its own command list
};

// 游標位置結構
struct StCursorPos {
    long line;
    long column;
    long position;
    
    StCursorPos();
    StCursorPos(long l, long c, long p);
    bool operator==(const StCursorPos& other) const { 
        return (line == other.line && column == other.column && position == other.position);
    };
    bool operator!=(const StCursorPos& other) const { 
        return !(*this == other);
    };
};

// 視覺選擇範圍結構
struct StVisualRange {
    StCursorPos start;
    StCursorPos end;
    EVimMode visualMode;
    bool isActive;
    
    StVisualRange(){ Clear(); };
    void Clear(){
        start.line = 0;
        start.column = 0;
        start.position = 0;
        end.line = 0;
        end.column = 0;
        end.position = 0;
        visualMode = evmNormal;
        isActive = false;
    };
    bool IsValid() const {
        return (start.line != end.line || start.column != end.column);
    };
};

// 標記結構
//TODO: not support currently
struct StVimMark {
    char markChar;
    StCursorPos position;
    std::string filename;
    bool isGlobal;
    
    StVimMark();
    StVimMark(char c, const StCursorPos& pos, const std::string& file = "", bool global = false);
};

// 寄存器結構
//TODO: not support currently
struct StVimRegister {
    char regChar;
    std::string content;
    bool isLinewise;
    bool isBlockwise;
    
    StVimRegister();
    StVimRegister(char c, const std::string& str, bool line = false, bool block = false);
    void Clear();
};

// KVim 主類別
class KVim {
public:
    // 建構與解構
    KVim();
    explicit KVim(KMthVim* tMthVim){ Initialize(); SetTextControl(tMthVim); }; 
    virtual ~KVim();
    
    // 初始化與設定
    void Initialize();
    void SetTextControl(KMthVim* tMthVim){ mMthVim = tMthVim; };
    void Reset();
    // void LoadConfiguration(const wxString& configFile); //TODO: not support currently
    // void SaveConfiguration(const wxString& configFile); //TODO: not support currently
    
    // 模式管理
    EVimMode GetCurrentMode() const;
    void SetMode(EVimMode mode){
        currentMode = mode;
        if (mode == evmNormal) {
            EnterNormalMode();
        } else if (mode == evmInsert) {
            EnterInsertMode();
        } else if (mode == evmVisual) {
            EnterVisualMode();
        } else if (mode == evmCommand) {
            EnterCommandMode();
        } else if (mode == evmOperatorPending) {
            EnterOperatorPendingMode();
        }
    };
    bool IsInNormalMode() const { return currentMode == evmNormal; };
    bool IsInInsertMode() const { return currentMode == evmInsert; };
    bool IsInVisualMode() const { return currentMode == evmVisual || currentMode == evmVisualLine || currentMode == evmVisualBlock; };
    bool IsInCommandMode() const { return currentMode == evmCommand; };
    bool IsInOperatorPendingMode() const { return currentMode == evmOperatorPending; };
    void EnterNormalMode();
    void EnterInsertMode();
    void EnterVisualMode(EVimMode visualType = evmVisual);
    void EnterCommandMode();
    void EnterOperatorPendingMode();
    void ExitCurrentMode();
    
    // 主要輸入處理
    bool ProcessKeyInput(wxKeyEvent& event);
    bool ProcessCharInput(char ch);
    bool ProcessKeyCode(int keyCode, bool shift = false, bool ctrl = false, bool alt = false);
    void CancelCurrentCommand();
    void CompleteCurrentCommand();
    
    // 指令解析與執行
    bool ParseCommand(char ch);
    bool ExecuteCommand(const StVimCmd& cmd);
    void ExecuteImmediateCommand(char ch);
    bool IsImmediateCommand(char ch, EVimMode mode) const;
    bool RequiresObject(EVimCmd cmd) const;
    EVimCmd CharToCommand(char ch, EVimMode mode) const;
    EVimObj CharToObject(char ch) const;
    
    // 指令狀態管理
    EVimCmdStage GetCommandStage() const;
    const StVimCmd& GetCurrentCommand() const;
    void ClearCurrentCommand();
    bool IsCommandComplete() const;
    bool IsWaitingForObject() const;
    std::string GetCommandString() const;
    
    // 計數處理
    void AppendCount(int digit);
    void ClearCount();
    int GetCurrentCount() const;
    int GetEffectiveCount() const;
    bool HasCount() const;
    
    // 游標操作
    StCursorPos GetCursorPosition() const;
    void SetCursorPosition(const StCursorPos& pos);
    void MoveCursor(long deltaLine, long deltaCol);
    void MoveCursorToLine(long line);
    void MoveCursorToColumn(long column);
    void MoveCursorToPosition(long position);
    
    // 文字操作
    std::string GetCurrentLine() const;
    std::string GetLine(long lineNumber) const;
    std::string GetText(const StCursorPos& start, const StCursorPos& end) const;
    std::string GetSelectedText() const;
    void InsertText(const std::string& text);
    void DeleteText(const StCursorPos& start, const StCursorPos& end);
    void ReplaceText(const StCursorPos& start, const StCursorPos& end, const std::string& newText);
    long GetLineCount() const;
    long GetLineLength(long line) const;
    
    // 移動指令
    void MoveLeft(int count = 1);
    void MoveRight(int count = 1);
    void MoveUp(int count = 1);
    void MoveDown(int count = 1);
    void MoveWordForward(int count = 1, bool bigWord = false);
    void MoveWordBackward(int count = 1, bool bigWord = false);
    void MoveWordEnd(int count = 1, bool bigWord = false);
    void MoveLineStart();
    void MoveLineEnd();
    void MoveLineFirstNonBlank();
    void MoveToLine(long line);
    void MoveToTop();
    void MoveToBottom();
    void MoveToMiddle();
    void MoveParagraphForward(int count = 1);
    void MoveParagraphBackward(int count = 1);
    void MoveSentenceForward(int count = 1);
    void MoveSentenceBackward(int count = 1);
    
    // 搜尋功能
    bool FindChar(char ch, bool forward = true, bool till = false);
    bool RepeatCharFind(bool forward = true);
    bool Search(const std::string& pattern, bool forward = true, bool wholeWord = false);
    bool SearchNext();
    bool SearchPrevious();
    bool SearchUnderCursor(bool forward = true);
    void SetLastSearchPattern(const std::string& pattern);
    std::string GetLastSearchPattern() const;
    
    // 編輯指令
    void DeleteCharacter(int count = 1, bool forward = true);
    void DeleteLine(int count = 1);
    void DeleteToLineEnd();
    void DeleteWord(int count = 1, bool bigWord = false);
    void DeleteRange(const StCursorPos& start, const StCursorPos& end);
    void ChangeCharacter(int count = 1);
    void ChangeLine(int count = 1);
    void ChangeToLineEnd();
    void ChangeWord(int count = 1, bool bigWord = false);
    void ChangeRange(const StCursorPos& start, const StCursorPos& end);
    void YankLine(int count = 1);
    void YankToLineEnd();
    void YankWord(int count = 1, bool bigWord = false);
    void YankRange(const StCursorPos& start, const StCursorPos& end);
    void Paste(bool after = true, int count = 1);
    void ReplaceCharacter(char ch, int count = 1);
    void SubstituteLine(int count = 1);
    void JoinLines(int count = 1);
    void IndentLines(int count = 1);
    void UnindentLines(int count = 1);
    void ToggleCase(int count = 1);
    
    // 插入模式操作
    void InsertAtCursor();
    void InsertAtLineStart();
    void AppendAtCursor();
    void AppendAtLineEnd();
    void OpenLineBelow();
    void OpenLineAbove();
    void EnterReplaceMode();
    
    // 視覺模式操作
    void StartVisualSelection(EVimMode visualType = evmVisual);
    void ExtendVisualSelection();
    void EndVisualSelection();
    StVisualRange GetVisualRange() const;
    void SetVisualRange(const StVisualRange& range);
    bool HasVisualSelection() const;
    void ClearVisualSelection();
    void VisualDelete();
    void VisualYank();
    void VisualChange();
    void VisualIndent();
    void VisualUnindent();
    void VisualToggleCase();
    
    // 撤銷與重做
    //TODO: not support currently
    // void Undo(int count = 1);
    // void Redo(int count = 1);
    // void RepeatLastCommand();
    // bool CanUndo() const;
    // bool CanRedo() const;
    
    // 標記功能
    //TODO: not support currently
    // void SetMark(char markChar);
    // bool GoToMark(char markChar);
    // bool GoToMarkLine(char markChar);
    // void ClearMark(char markChar);
    // void ClearAllMarks();
    // bool IsValidMarkChar(char ch) const;
    
    // 寄存器操作
    //TODO: not support currently
    // void SetRegister(char regChar, const std::string& content, bool linewise = false, bool blockwise = false);
    // StVimRegister GetRegister(char regChar) const;
    // void ClearRegister(char regChar);
    // void ClearAllRegisters();
    // bool IsValidRegisterChar(char ch) const;
    // void SelectRegister(char regChar);
    // char GetSelectedRegister() const;
    
    // 巨集功能
    //TODO: not support currently
    // void StartRecordingMacro(char macroChar);
    // void StopRecordingMacro();
    // void PlayMacro(char macroChar, int count = 1);
    // bool IsRecordingMacro() const;
    // bool HasMacro(char macroChar) const;
    // void ClearMacro(char macroChar);
    // void ClearAllMacros();
    
    // 狀態查詢
    bool IsCommandPending() const { return (mCmdStage != evsIdle); };
    bool IsModified() const { return mMthVim->IsModified(); };
    std::string GetStatusString() const;
    std::string GetModeString() const;
    int GetLastCommandCount() const;
    
    // 設定與選項
    //TODO: not support currently
    // void SetOption(const std::string& name, const std::string& value);
    // std::string GetOption(const std::string& name) const;
    // void SetNumberOption(const std::string& name, int value);
    // int GetNumberOption(const std::string& name) const;
    // void SetBoolOption(const std::string& name, bool value);
    // bool GetBoolOption(const std::string& name) const;
    
    // 事件回調
    //TODO: not support currently
    // void SetOnModeChanged(std::function<void(EVimMode, EVimMode)> callback);
    // void SetOnCommandExecuted(std::function<void(const StVimCmd&)> callback);
    // void SetOnCursorMoved(std::function<void(const StCursorPos&, const StCursorPos&)> callback);
    // void SetOnTextChanged(std::function<void()> callback);
    // void SetOnStatusChanged(std::function<void(const std::string&)> callback);
    
    // 調試與診斷
    //TODO: not support currently
    // void EnableDebugMode(bool enable = true);
    // bool IsDebugMode() const;
    // void LogDebugInfo(const std::string& info);
    // std::vector<std::string> GetDebugLog() const;
    // void ClearDebugLog();

protected:
    // 內部輔助方法
    void InitializeCommandMaps();
    void InitializeObjectMaps();
    void InitializeDefaultSettings();
    void UpdateCursorDisplay();
    void UpdateStatusDisplay();
    void NotifyModeChanged(EVimMode oldMode, EVimMode newMode);
    void NotifyCommandExecuted(const StVimCmd& cmd);
    void NotifyCursorMoved(const StCursorPos& oldPos, const StCursorPos& newPos);
    void NotifyTextChanged();
    void NotifyStatusChanged();
    
    // 指令驗證
    bool IsValidCommand(EVimCmd cmd, EVimMode mode) const;
    bool IsValidObject(EVimObj obj) const;
    bool IsValidCombination(EVimCmd cmd, EVimObj obj) const;
    
    // 範圍計算
    StCursorPos CalculateObjectRange(EVimObj obj, int count, bool& isLinewise) const;
    StCursorPos CalculateMotionEnd(EVimCmd motion, int count) const;
    std::pair<StCursorPos, StCursorPos> GetTextObjectRange(EVimObj obj) const;
    
    // 文字處理輔助
    bool IsWordChar(char ch) const;
    bool IsWORDChar(char ch) const;
    bool IsBlankLine(long line) const;
    long FindNextWord(long pos, bool bigWord = false) const;
    long FindPrevWord(long pos, bool bigWord = false) const;
    long FindWordEnd(long pos, bool bigWord = false) const;
    long FindLineStart(long line) const;
    long FindLineEnd(long line) const;
    long FindFirstNonBlank(long line) const;

private:
    // 核心成員變數
    KMthVim& mMthVim; // 文字編輯器介面
    
    // 狀態管理
    EVimMode mCurMode=evmInsert;  // at initial the editor is in insert mode
    EVimMode mPreMode=evmInsert;
    EVimCmdStage mCmdStage=evsIdle; // stage 0
    StVimCmd mCurCmd;
    StVimCmd mLastCmd;
    
    // 計數與參數
    std::string mCountBuf; 
    std::string mParamBuf;
    bool mHasCount=false;
    
    // 游標與選擇
    StCursorPos mCurPos;
    StCursorPos mLastPos;
    StVisualRange mVisualRange;
    
    // 搜尋相關
    std::string mLastSchPattern;
    bool mLastSchAhead=true; 
    char mLastFindChar=0;
    bool mLastFindAhead=true;
    bool mLastFindTill=false;
    
    // 寄存器與標記
    //TODO: not support currently
    // std::map<char, StVimRegister> m_registers;
    // std::map<char, StVimMark> m_marks;
    // char m_selectedRegister;
    
    // 巨集系統
    //TODO: not support currently
    // std::map<char, std::string> m_macros;
    // std::string m_currentMacro;
    // char m_recordingMacroChar;
    // bool m_isRecordingMacro;
    
    // 指令映射
    std::map<char, StVimCmdInfo> mNormCmdMap;
    std::map<char, StVimCmdInfo> mVisualCmdMap;
    std::map<char, EVimObj> mObjMap;
    // std::vector<char> m_immediateNormalCommands; // not used: use mNormStg0Chars
    // std::vector<char> m_immediateVisualCommands; // not used: use mVisuStg0Chars
    
    // 設定選項
    //TODO: not support currently
    // std::map<std::string, std::string> m_stringOptions;
    // std::map<std::string, int> m_numberOptions;
    // std::map<std::string, bool> m_boolOptions;
    
    // 事件回調
    //TODO: not support currently
    // std::function<void(EVimMode, EVimMode)> m_onModeChanged;
    // std::function<void(const StVimCmd&)> m_onCommandExecuted;
    // std::function<void(const StCursorPos&, const StCursorPos&)> m_onCursorMoved;
    // std::function<void()> m_onTextChanged;
    // std::function<void(const std::string&)> m_onStatusChanged;
    
    // 調試支援
    //TODO: not support currently
    // bool m_debugMode;
    // std::vector<std::string> m_debugLog;
    
    // 內部狀態標記
    bool mIsInit=false;; 
    bool mIsModified=false;
    bool mIsInProcCmd=false;
    
    // 常數定義
    static const char* mNormStg0Chars=gVimNormalCmdCharStage0;
    static const char* mVisuStg0Chars=gVimVisualCmdCharStage0;
    static const char* mNormCmdChars=gVimNormalCmdChar;
    static const char* mVisuCmdChars=gVimVisualCmdChar;
    static const int CMaxCountLength=4;
    static const int CMaxParamLength=1; //TODO: may support longer Param in future for utf8 char
};
