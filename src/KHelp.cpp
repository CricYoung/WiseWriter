
#include "KHelp.hpp"

KHelp::KHelp(wxWindow* parent, const wxString& title)
    : wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  // 建立多行唯讀的 wxTextCtrl
  txtHelp = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY);

  // 建立一個垂直的 Box Sizer 來管理 TextCtrl
  wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
  mainSizer->Add(txtHelp, 1, wxEXPAND | wxALL, 5); // TextCtrl 佔用所有空間

  // 建立一個標準的 OK 按鈕
  wxButton* okButton = new wxButton(this, wxID_OK, "OK");
  wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(okButton, 0, wxALL, 5);
  mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5); // 按鈕置中
  SetSizer(mainSizer);
  SetSizerAndFit(mainSizer);
};
