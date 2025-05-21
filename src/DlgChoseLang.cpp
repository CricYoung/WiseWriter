// ==== DlgChoseLang ====
#include "DlgChoseLang.hpp"

DlgChoseLang::DlgChoseLang(wxWindow* parent, const wxString& title)
	: wxDialog(parent, wxID_ANY, title, wxDefaultPosition, wxSize(300, 200))
{
	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
	m_choice = new wxChoice(this, wxID_ANY);
	for(auto it = gLangMap.begin(); it != gLangMap.end(); ++it) {
		m_choice->Append(it->first);
	};

	CheckWxChoseLangAvailable(m_choice);
	m_choice->SetSelection(0); 
	sizer->Add(m_choice, 0, wxALL | wxEXPAND, 5);
	SetSizer(sizer);
	//add ok and cancel button
	wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
	buttonSizer->Add(new wxButton(this, wxID_OK, "OK"), 0, wxALL, 5);
	buttonSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxALL, 5);
	sizer->Add(buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL);
	SetSizerAndFit(sizer);
	SetTitle(title);
	Bind(wxEVT_BUTTON, &DlgChoseLang::OnOK, this, wxID_OK);
	Bind(wxEVT_BUTTON, &DlgChoseLang::OnCancel, this, wxID_CANCEL);
	Bind(wxEVT_CLOSE_WINDOW, &DlgChoseLang::OnClose, this);
}

void DlgChoseLang::OnOK(wxCommandEvent& )
{
	m_lang = m_choice->GetStringSelection();
	bIsChosed=true;
	EndModal(wxID_OK);
}
void DlgChoseLang::OnCancel(wxCommandEvent& )
{
	m_lang = "";
	bIsChosed=false;
	EndModal(wxID_CANCEL);
}

bool DlgChoseLang::ChoseLang(wxString& tLang)
{
	if (tLang.empty()) return false; // No language chosen
	int tNdx=m_choice->FindString(tLang);
	if (tNdx == wxNOT_FOUND) return false; // Language not found
	m_choice->SetSelection(tNdx);
	m_lang = tLang;
	bIsChosed=true;
	return true; // Language chosen
}
