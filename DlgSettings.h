//DlgSettings.h
#pragma once
#include <wx/stc/stc.h>
#include "Dlgs.h"
#include "KGlobal.h"
#include <wx/choice.h>
#include <wx/config.h>

const int CMinAutoSaveKeyCount = 100; // Minimum auto save key count
const std::string CMinAutoSaveKeyCountStr = "100"; // Minimum auto save key count string
const int CMinAutoSaveSecCount = 100; // Minimum auto save after seconds
const std::string CMinAutoSaveAfterSecondsStr = "100"; // Minimum auto save after seconds string

// === Class DlgSettings ===
class DlgSettings : public DlgSettingsBase
{
public:
  DlgSettings(wxWindow* parent, const wxString& title=_("Settings"));
	virtual ~DlgSettings(){};

  void IniVar(wxFont& tFont, wxColor& tLightBackgroundColor, wxColor& tLightTextColor, wxColor& tDarkBackgroundColor, wxColor& tDarkTextColor);
  void OnChoseFont(wxMouseEvent& ) override ;
	void OnActDarkMode(wxMouseEvent& event) override;
	void OnChoseLightBackgroundColor(wxMouseEvent& event) override; 
	void OnChoseLightForegroundColor(wxMouseEvent& event) override;
	void OnChoseDarkForegroundColor(wxMouseEvent& event) override;
	void OnChoseDarkBackgroundColor(wxMouseEvent& event) override;
	void OnLargerFont(wxMouseEvent& event) override;
	void OnSmallerFont(wxMouseEvent& event) override;
	void OnOkRestart(wxCommandEvent& event) override;
	void OnLeaveAutoSaveCount(wxFocusEvent& event) override;
	// helper functions
	void DeleteNotAvailableLang();
	void SetDarkMode(bool tDark);  
  void TakeEffect(wxTextCtrl* tInput=nullptr);
	void SetTextDemo(){ TakeEffect(textDemo); };
	bool IsActDarkMode() const { return btnDarkMode->GetValue(); };
	void GetPreferFont(wxFont& tFont) const { tFont = m_font; };
	void GetLightBackgroundColor(wxColor& tColor) const { tColor = m_LightBackgroundColor; };
	void GetLightTextColor(wxColor& tColor) const { tColor = m_LightTextColor; };
	void GetDarkBackgroundColor(wxColor& tColor) const { tColor = m_DarkBackgroundColor; };
	void GetDarkTextColor(wxColor& tColor) const { tColor = m_DarkTextColor; };
	void GetLanguageName(wxString& tLang) const;
	void GetIsAutoSaveByKeyCount(bool& tAutoSave) const { tAutoSave = chkAutoSaveAfterKeyCount->GetValue(); };
	void GetAutoSaveKeyCount(int& tKeyCount) const { int tVal; edAutoSaveKeyCount->GetValue().ToInt(&tVal); tKeyCount = tVal; };
	void GetIsAutoSaveBySecCount(bool& tAutoSaveSec) const { tAutoSaveSec = chkAutoSaveAfterSecCount->GetValue(); };
	void GetAutoSaveSecCount(int& tSeconds) const { int tVal; edAutoSaveSecCount->GetValue().ToInt(&tVal); tSeconds = tVal; };
	void SetPreferFont(const wxFont& tFont) { m_font = tFont; };
	void SetPreferBackgroundColor(const wxColor& tColor) { m_LightBackgroundColor = tColor; };
	void SetPreferTextColor(const wxColor& tColor) { m_LightTextColor = tColor; };
	void SetPreferBackgroundColorDark(const wxColor& tColor) { m_DarkBackgroundColor = tColor; };
	void SetPreferTextColorDark(const wxColor& tColor) { m_DarkTextColor = tColor; };
  bool SetLanguageName(const wxString& tLang) { return choLanguages->SetStringSelection(tLang); };
	void SetIsAutoSaveByKeyCount(bool tAutoSave) { chkAutoSaveAfterKeyCount->SetValue(tAutoSave); };
	void SetAutoSaveKeyCount(int tKeyCount) { edAutoSaveKeyCount->SetValue(wxString::Format("%i", tKeyCount)); };
	void SetIsAutoSaveBySecCount(bool tAutoSaveSec) { chkAutoSaveAfterSecCount->SetValue(tAutoSaveSec); };
	void SetAutoSaveSecCount(int tSeconds) { edAutoSaveSecCount->SetValue(wxString::Format("%i", tSeconds)); };
	//hotkey
	void SetHotkeySets(const StHotkeySet* tHotkeySets, size_t tRowCount);
  void OnItemActivated(wxDataViewEvent& event);
	bool GetHotkeySets(StHotkeySet* tHotkeySets, size_t tRowCount);
	bool IsOkRestart() const { return bOkRestart; };
private:
	wxFont m_font;
  wxColor m_LightBackgroundColor;
	wxColor m_LightTextColor;
	wxColor m_DarkBackgroundColor;
	wxColor m_DarkTextColor;
	const StHotkeySet* m_HotkeySets;
	size_t m_HotkeySetCount;
	bool bOkRestart = false; // Flag: Restart app after settings changed
};

class HotkeyDialog : public wxDialog {
	public:
		bool bCtrlKey = false;
		wxString hotkeyResult;
		HotkeyDialog(wxWindow* parent);
		// the event.ControlDown() method is not reliable on macOS-wxWidgets,and when CmdDown() is true, it will also set Ctrl down flag
    // so here we check the keyCode 396 to determine if Ctrl key is pressed
		void OnKeyDown(wxKeyEvent& event);
		// set bCtrlKey to false when key up
		void OnKeyUp(wxKeyEvent& event){
			bCtrlKey = false;
			event.Skip();
		};
};
	