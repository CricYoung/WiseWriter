///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/statbox.h>
#include <wx/dataview.h>
#include <wx/notebook.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DlgSettingsBase
///////////////////////////////////////////////////////////////////////////////
class DlgSettingsBase : public wxDialog
{
	private:

	protected:
		wxNotebook* m_notebook3;
		wxPanel* pnlGeneral;
		wxStaticText* m_staticText2;
		wxChoice* choLanguages;
		wxCheckBox* chkAutoSaveAfterKeyCount;
		wxTextCtrl* edAutoSaveKeyCount;
		wxCheckBox* chkAutoSaveAfterSecCount;
		wxTextCtrl* edAutoSaveSecCount;
		wxPanel* pnlAppearence;
		wxButton* m_button41;
		wxButton* m_button18;
		wxButton* m_button19;
		wxToggleButton* btnDarkMode;
		wxTextCtrl* textDemo;
		wxButton* m_button3;
		wxButton* m_button4;
		wxButton* m_button31;
		wxButton* m_button42;
		wxPanel* pgHotKey;
		wxStaticText* m_staticText11;
		wxDataViewListCtrl* listHotKeys;
		wxPanel* pnlAbout;
		wxStaticText* m_staticText33;
		wxStaticText* m_staticText3;
		wxStaticText* m_staticText32;
		wxStaticText* m_staticText35;
		wxStaticText* m_staticText351;
		wxStaticText* m_staticText352;
		wxStaticText* m_staticText3511;
		wxButton* m_button11;
		wxButton* m_button12;
		wxButton* m_button13;

		// Virtual event handlers, override them in your derived class
		virtual void OnLeaveAutoSaveCount( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnChoseFont( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnLargerFont( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnSmallerFont( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnActDarkMode( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnChoseLightBackgroundColor( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnChoseLightForegroundColor( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnChoseDarkBackgroundColor( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnChoseDarkForegroundColor( wxMouseEvent& event ) { event.Skip(); }
		virtual void OnOkRestart( wxCommandEvent& event ) { event.Skip(); }


	public:

		DlgSettingsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 594,388 ), long style = wxDEFAULT_DIALOG_STYLE );

		~DlgSettingsBase();

};

