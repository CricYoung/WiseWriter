
#pragma once
#include <wx/wx.h>
#include <wx/textctrl.h>
class KHelp : public wxDialog
{
public:
  KHelp(wxWindow* parent, const wxString& title);
  bool LoadFile(const wxString& fileName) {
    if (fileName.IsEmpty()) return false;
    return txtHelp->LoadFile(fileName);
  }

private:
    wxTextCtrl* txtHelp;
};
