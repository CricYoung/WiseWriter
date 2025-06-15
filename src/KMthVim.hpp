#pragma once

#include <wx/wx.h>
#include <string>

class KMthVim {
public:
    virtual ~KMthVim() = default;
    virtual wxString GetValue() const = 0;
    virtual void SetValue(const wxString& value) = 0;
    virtual void WriteText(long pos,const wxString& text) = 0;
    virtual long GetInsertionPoint() const = 0;
    virtual void SetInsertionPoint(long pos) = 0;
    virtual long GetLastPosition() const = 0;
    // ==================== 行列座標轉換 ====================
    virtual bool PositionToXY(long pos, long* col, long* line) const = 0;
    virtual long XYToPosition(long col, long line) const = 0;
    // ==================== 行操作 ====================
    virtual int GetLineLength(long lineNo) const = 0;
    virtual wxString GetLineText(long lineNo) const = 0;
    virtual int GetNumberOfLines() const = 0;
    // ==================== 選擇與範圍操作 ====================
    virtual wxString GetRange(long from, long to) const = 0;
    virtual void Remove(long from, long to) = 0;
    virtual void Replace(long from, long to, const wxString& value) = 0;
    virtual void SetSelection(long from, long to) = 0;
    virtual long GetSelectionStart() const = 0;
    virtual long GetSelectionEnd() const = 0;
    virtual wxString GetSelectedText() const = 0;
    // ==================== 修改狀態 ====================
    virtual bool IsModified() const = 0;
    virtual void MarkDirty() = 0;
    virtual void DiscardEdits() = 0;
    // ==================== 撤銷/重做支援 ====================
    virtual bool CanUndo() const = 0;
    virtual bool CanRedo() const = 0;
    virtual void Undo() = 0;
    virtual void Redo() = 0;
    // ==================== 顯示與焦點 ====================
    virtual void SetFocus() = 0;
    virtual bool HasFocus() const = 0;
    virtual void Refresh() = 0;
    // ==================== 字符檢查輔助 ====================
    virtual wxUniChar GetCharAt(long pos) const = 0;
    virtual bool IsValidPosition(long pos) const = 0;
};

