// == Chose Language ==
#pragma once
#include <wx/wx.h>
#include <wx/choice.h>
#include <wx/dialog.h> 
#include "KLang.hpp" // For global functions
class DlgChoseLang : public wxDialog
{
public:
	DlgChoseLang(wxWindow* parent, const wxString& title);
	void OnOK(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnClose(wxCloseEvent& ){ bIsChosed = false; m_lang=""; EndModal(wxID_CANCEL); };
	wxString GetLang() const { return m_lang; }
	bool IsChosed() const { return bIsChosed; }
	void DeleteNotAvailableLang(){ CheckWxChoseLangAvailable(m_choice); }; 
	bool ChoseLang(wxString& tLang);
private:
	bool bIsChosed = false;
	wxString m_lang;
	wxChoice* m_choice;
};

