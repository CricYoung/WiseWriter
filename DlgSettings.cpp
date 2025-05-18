
#include <wx/colordlg.h> // For wxColourDialog
#include <wx/fontdlg.h> // For wxFontDialog
#include <wx/variant.h>
#include "DlgSettings.h"
#include "KHotKey.h"
#include "KLang.hpp"

// --- listHotKeys Fields ---
const StField DefFields[] = {
	{ "說明描述（Describe)", 300 },
	{ "熱鍵（Hotkey）",  100 },
	// { "Label",    150 },
};
 
const size_t CListHotKeysFieldCount = sizeof(DefFields) / sizeof(StField);


DlgSettings::DlgSettings(wxWindow* parent, const wxString& title):
DlgSettingsBase(parent, wxID_ANY, title, wxDefaultPosition, wxSize(669, 441), wxDEFAULT_DIALOG_STYLE)
{
  //--- Set the font and colors for the demo text ---
  m_font = wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
  m_LightBackgroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
  m_LightTextColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  m_DarkBackgroundColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW);
  m_DarkTextColor = wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT);
  textDemo->SetBackgroundColour(m_LightBackgroundColor);
  textDemo->SetForegroundColour(m_LightTextColor);
  textDemo->SetFont(m_font);
  //set value for demo text include English Chinese Japanese Hindi
  wxString tDemoText = wxString::Format("This is a demo text.\nYou can change the font and color.\n");
  tDemoText += wxString::Format("這是一段示範文字。\n你可以改變字型和顏色。\n");
  tDemoText += wxString::Format("これはデモテキストです。\nフォントと色を変更できます。\n");
  tDemoText += wxString::Format("यह एक डेमो टेक्स्ट है।\nआप फ़ॉन्ट और रंग बदल सकते हैं。\n");
  tDemoText += "...^_^...Copilot is a great tool for coding.\n";
  tDemoText += wxString::Format("...^_^...Copilot 是一個很棒的編碼工具。\n");
  tDemoText += wxString::Format("...^_^...Copilot は素晴らしいコーディングツールです。\n");
  tDemoText += wxString::Format("...^_^...Copilot एक बेहतरीन कोडिंग टूल है।\n");
  textDemo->SetValue(tDemoText);
  //--- Set choice for language selection ---
  choLanguages->Clear();
  for(auto it = gLangMap.begin(); it != gLangMap.end(); ++it) {
		choLanguages->Append(it->first);
	};
	CheckWxChoseLangAvailable(choLanguages); 
	choLanguages->SetSelection(0); 
  //--- list Hotkey Settings ---
  ::InitListColumns(listHotKeys, DefFields, CListHotKeysFieldCount);
  // ::SetListHotkeySetsRows(listHotKeys, DefHotkeySets, CDefHotkeyCount);
  listHotKeys->Bind(wxEVT_DATAVIEW_ITEM_ACTIVATED, &DlgSettings::OnItemActivated, this);
 
};

void DlgSettings::IniVar(wxFont& tFont, wxColor& tLightBackgroundColor, wxColor& tLightTextColor, wxColor& tDarkBackgroundColor, wxColor& tDarkTextColor){
  m_font = tFont;
  m_LightBackgroundColor = tLightBackgroundColor;
  m_LightTextColor = tLightTextColor;
  m_DarkBackgroundColor = tDarkBackgroundColor;
  m_DarkTextColor = tDarkTextColor;
  SetTextDemo();
};

void DlgSettings::OnChoseFont(wxMouseEvent& ) { 
  wxFontData fontData;
  fontData.SetInitialFont(m_font);
  wxFontDialog fontDialog(this, fontData);
  if (fontDialog.ShowModal() == wxID_OK)
  {
    m_font = fontDialog.GetFontData().GetChosenFont();
    textDemo->SetFont(m_font);
  }
};

void DlgSettings::OnLargerFont(wxMouseEvent& ) {
  m_font.SetPointSize(m_font.GetPointSize() + 1);
  textDemo->SetFont(m_font);
}; 
void DlgSettings::OnSmallerFont(wxMouseEvent& ) {
  if(m_font.GetPointSize() <= 1) return; // Prevent font size from going below 1
  m_font.SetPointSize(m_font.GetPointSize() - 1);
  textDemo->SetFont(m_font);
};
void DlgSettings::OnActDarkMode(wxMouseEvent& ) {
  SetDarkMode(IsActDarkMode());
};

void DlgSettings::OnChoseLightBackgroundColor(wxMouseEvent& ) {
  wxColourData data;
  data.SetChooseFull(true);
  data.SetColour(m_LightBackgroundColor);
  wxColourDialog dialog(this, &data);
  if (dialog.ShowModal() != wxID_OK) return;
  if(IsActDarkMode()) SetDarkMode(false);
  m_LightBackgroundColor = dialog.GetColourData().GetColour();
  textDemo->SetBackgroundColour(m_LightBackgroundColor); 
};

void DlgSettings::OnChoseLightForegroundColor(wxMouseEvent& ) {
  wxColourData data;
  data.SetChooseFull(true);
  data.SetColour(m_LightTextColor);
  wxColourDialog dialog(this, &data);
  if (dialog.ShowModal() != wxID_OK) return;
  if(IsActDarkMode()) SetDarkMode(false);
  m_LightTextColor = dialog.GetColourData().GetColour();
  textDemo->SetForegroundColour(m_LightTextColor); 
};
void DlgSettings::OnChoseDarkForegroundColor(wxMouseEvent& ) {
  wxColourData data;
  data.SetChooseFull(true);
  data.SetColour(m_DarkTextColor);
  wxColourDialog dialog(this, &data);
  if (dialog.ShowModal() != wxID_OK) return;
  if(!IsActDarkMode()) SetDarkMode(true); ;
  m_DarkTextColor = dialog.GetColourData().GetColour();
  textDemo->SetForegroundColour(m_DarkTextColor); 
};
void DlgSettings::OnChoseDarkBackgroundColor(wxMouseEvent& ) {
  wxColourData data;
  data.SetChooseFull(true);
  data.SetColour(m_DarkBackgroundColor);
  wxColourDialog dialog(this, &data);
  if (dialog.ShowModal() != wxID_OK) return;
  if(!IsActDarkMode()) SetDarkMode(true); ;
  m_DarkBackgroundColor = dialog.GetColourData().GetColour();
  textDemo->SetBackgroundColour(m_DarkBackgroundColor); 
};

void DlgSettings::DeleteNotAvailableLang() {
  CheckWxChoseLangAvailable(choLanguages);
};

void DlgSettings::SetDarkMode(bool tDarkMode) {
  if (tDarkMode) {
    textDemo->SetBackgroundColour(m_DarkBackgroundColor);
    textDemo->SetForegroundColour(m_DarkTextColor);
  } else {
    textDemo->SetBackgroundColour(m_LightBackgroundColor);
    textDemo->SetForegroundColour(m_LightTextColor);
  }
};

void DlgSettings::OnOkRestart(wxCommandEvent& ) { 
  EndModal(wxID_RESET); 
  bOkRestart = true;
};

void DlgSettings::OnLeaveAutoSaveCount(wxFocusEvent& ){
  wxString tText = edAutoSaveKeyCount->GetValue();
  long tValue;
  if (tText.ToLong(&tValue) == false || tValue < CMinAutoSaveKeyCount) edAutoSaveKeyCount->SetValue(CMinAutoSaveKeyCountStr);
  tText = edAutoSaveSecCount->GetValue();
  if (tText.ToLong(&tValue) == false || tValue < CMinAutoSaveSecCount) edAutoSaveSecCount->SetValue(CMinAutoSaveAfterSecondsStr);
};

void DlgSettings::GetLanguageName(wxString& tLang) const {
  int tIndex = choLanguages->GetSelection();
  if (tIndex == wxNOT_FOUND) return;
  tLang = choLanguages->GetString(tIndex);
};

void DlgSettings::TakeEffect(wxTextCtrl* tInput){
  if(!tInput) tInput = textDemo;
  tInput->SetFont(m_font);
  if (IsInDarkMode()) {
    tInput->SetBackgroundColour(m_DarkBackgroundColor);
    tInput->SetForegroundColour(m_DarkTextColor);
  } else {
    tInput->SetBackgroundColour(m_LightBackgroundColor);
    tInput->SetForegroundColour(m_LightTextColor);
  }
}

void DlgSettings::SetHotkeySets(const StHotkeySet* tHotkeySets, size_t tRowCount){
  if (!tHotkeySets || tRowCount == 0) return;
  m_HotkeySets = tHotkeySets;
  m_HotkeySetCount = tRowCount;
  listHotKeys->DeleteAllItems();
  for (size_t i = 0; i < tRowCount; ++i) {
    wxVector<wxVariant> row;
    row.push_back(wxVariant(tHotkeySets[i].describe));
    row.push_back(wxVariant(tHotkeySets[i].hotkey));
    wxString translated = wxGetTranslation(tHotkeySets[i].label);
    // row.push_back(wxVariant(tHotkeySets[i].label));
    row.push_back(wxVariant(translated));
    listHotKeys->AppendItem(row);
  }
}

void DlgSettings::OnItemActivated(wxDataViewEvent& event) {
  wxDataViewItem item = event.GetItem();
  unsigned int row = listHotKeys->ItemToRow(item);
  if (row == wxNOT_FOUND)
      return;

  wxVariant labelVar;
  listHotKeys->GetValue(labelVar, row, 2);
  wxString label = labelVar.GetString();

  HotkeyDialog dlg(this);
  wxPoint mousePos = wxGetMousePosition();
  dlg.SetPosition(mousePos);
  if (dlg.ShowModal() == wxID_OK) {
      wxString newKey = dlg.hotkeyResult;
      listHotKeys->SetValue(wxVariant(newKey), row, 1);
  }
}

bool DlgSettings::GetHotkeySets(StHotkeySet* tHotkeySets, size_t tRowCount){
  if (!tHotkeySets || tRowCount == 0) return false;
  for (size_t i = 0; i < tRowCount; ++i) {
    wxVector<wxVariant> row;
    wxVariant tVar;
    listHotKeys->GetValue(tVar,i, 2);
    tHotkeySets[i].label=tVar.GetString();    
    listHotKeys->GetValue(tVar,i, 0);
    tHotkeySets[i].describe=tVar.GetString();
    listHotKeys->GetValue(tVar,i, 1);
    tHotkeySets[i].hotkey=tVar.GetString();
  }
  return true;
}

//=== HotkeyDialog ===
HotkeyDialog::HotkeyDialog(wxWindow* parent)
: wxDialog(parent, wxID_ANY, "Press a hotkey", wxDefaultPosition, wxSize(300, 100)) {
  wxTextCtrl* tEdit=new wxTextCtrl( this, wxID_ANY, "...", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxWANTS_CHARS );
  tEdit->Bind(wxEVT_KEY_DOWN, &HotkeyDialog::OnKeyDown, this);
}

void HotkeyDialog::OnKeyDown(wxKeyEvent& event) {
  StKey tKey;
  tKey.keyCode = event.GetKeyCode();
  if(tKey.keyCode == WXK_ESCAPE) {
      EndModal(wxID_CANCEL);
      return;
  }
  // tKey.bCtrl = event.ControlDown();
  // the ControlDown() method is not reliable on macOS,and when CmdDown() is true, it will also set Ctrl down flag
  // so here we check the keyCode 396 to determine if Ctrl key is pressed
  if(tKey.keyCode == 396){ bCtrlKey = true; event.Skip(); return; }; // bCtrlKey will be set to false when key up
  tKey.bAlt = event.AltDown();
  tKey.bShift = event.ShiftDown();
  tKey.bCmd = event.CmdDown(); // wxWidgets on macOS uses Cmd as Ctrl, Cmd down will also set Ctrl down flag
  tKey.bCtrl = bCtrlKey; // Use the global variable to check if Ctrl key is pressed

  // fprintf(stderr, "keyCode=%i Ctrl=%i Alt=%i Shift=%i Cmd=%i\n", tKey.keyCode, tKey.bCtrl, tKey.bAlt, tKey.bShift, tKey.bCmd);
  std::string hotkeyStr;
  if (Key2Str(tKey, hotkeyStr) == false) {
      event.Skip();
      return;
  }
  hotkeyResult = hotkeyStr;
  EndModal(wxID_OK);
  return;
}