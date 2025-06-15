
#pragma once
#include "KMthVim.hpp"
#include <wx/textctrl.h>

/**
 * KMthVim Adapter - wxTextCtrl 的適配器實作
 * 
 * 這個類別將 wxTextCtrl 適配到 KMthVim 介面，
 * 讓現有的 wxTextCtrl 可以直接與 KVim 配合使用。
 */
class KMthWxEdit : public KMthVim {
public:
    KMthWxEdit() = default;
    KMthWxEdit(wxTextCtrl& tEditor){
        mEditor = tEditor;
    };
    virtual  ~KMthWxEdit() = default;
    void SetEditor(wxTextCtrl& tEditor) {
        mEditor = tEditor;
    };
    
    // 實作所有 KMthVim 的純虛擬方法
    wxString GetValue() const override { return mEditor.GetValue();} ;
    void SetValue(const wxString& value) override { mEditor.SetValue(value);}; 
    void WriteText(const wxString& text) override { mEditor.WriteText(text);};
    
    long GetInsertionPoint() const override { return mEditor.GetInsertionPoint();} ;
    void SetInsertionPoint(long pos) override { mEditor.SetInsertionPoint(pos);};
    long GetLastPosition() const override { return mEditor.GetLastPosition();} ;
    
    bool PositionToXY(long pos, long* col, long* line) const override { return mEditor.PositionToXY(pos, col, line);};
    long XYToPosition(long col, long line) const override { return mEditor.XYToPosition(col, line);};
    
    int GetLineLength(long lineNo) const override { return mEditor.GetLineLength(lineNo);} ; 
    wxString GetLineText(long lineNo) const override { return mEditor.GetLineText(lineNo);} ;
    int GetNumberOfLines() const override { return mEditor.GetNumberOfLines();} ;
    
    wxString GetRange(long from, long to) const override { return mEditor.GetRange(from, to);} ;
    void Remove(long from, long to)  override { mEditor.Remove(from, to);};
    void Replace(long from, long to, const wxString& value)  override { mEditor.Replace(from, to, value);};
    
    void SetSelection(long from, long to)  override { mEditor.SetSelection(from, to);};
    long GetSelectionStart() const override { long tStart; mEditor.GetSelection(&tStart,0); return tStart; } ; 
    long GetSelectionEnd() const override { long tEnd; mEditor.GetSelection(0,&tEnd); return tEnd; } ; 
    wxString GetSelectedText() const override { long tStart, tEnd; mEditor.GetSelection(&tStart, &tEnd); return mEditor.GetRange(tStart, tEnd); } ;
    
    bool IsModified() const override { return mEditor.IsModified();} ;
    void MarkDirty() override { mEditor.MarkDirty();} ;
    void DiscardEdits() override { mEditor.DiscardEdits();} ;
    
    bool CanUndo() const override { return mEditor.CanUndo();} ;
    bool CanRedo() const override { return mEditor.CanRedo();} ;
    void Undo() override { mEditor.Undo();} ;
    void Redo() override { mEditor.Redo();} ;
    
    void SetFocus() override { mEditor.SetFocus();} ;
    bool HasFocus() const override { return mEditor.HasFocus();} ;
    void Refresh() override { mEditor.Refresh();} ;
    
    wxUniChar GetCharAt(long tPos) const  override {  
        wxUniChar tChar;
        wxString tStr = mEditor.GetRange(tPos, tPos + 1);
        if (tStr.length() > 0) {
            tChar = wxUniChar(tStr[0]);
        } else {
            tChar = wxUniChar(0);
        }
        return tChar;
    } ;
    bool IsValidPosition(long pos) const override {
        return (pos >= 0 && pos < mEditor.GetLastPosition());
    };

private:
    wxTextCtrl& mEditor;
};