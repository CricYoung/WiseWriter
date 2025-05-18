///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "Dlgs.h"

///////////////////////////////////////////////////////////////////////////

DlgSettingsBase::DlgSettingsBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	m_notebook3 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	pnlGeneral = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer( wxVERTICAL );

	m_staticText2 = new wxStaticText( pnlGeneral, wxID_ANY, _("Chose Language ( need restart to take effect )"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	bSizer121->Add( m_staticText2, 0, wxALL, 5 );

	wxArrayString choLanguagesChoices;
	choLanguages = new wxChoice( pnlGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, choLanguagesChoices, 0 );
	choLanguages->SetSelection( 0 );
	bSizer121->Add( choLanguages, 0, wxALL|wxEXPAND, 5 );


	bSizer8->Add( bSizer121, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );

	chkAutoSaveAfterKeyCount = new wxCheckBox( pnlGeneral, wxID_ANY, _("Auto Save after key counts ( >= 100 )"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( chkAutoSaveAfterKeyCount, 0, wxALL|wxEXPAND, 5 );

	edAutoSaveKeyCount = new wxTextCtrl( pnlGeneral, wxID_ANY, _("100"), wxDefaultPosition, wxDefaultSize, 0 );
	edAutoSaveKeyCount->SetMinSize( wxSize( 80,-1 ) );

	bSizer13->Add( edAutoSaveKeyCount, 0, wxALL, 5 );

	chkAutoSaveAfterSecCount = new wxCheckBox( pnlGeneral, wxID_ANY, _("Auto Save after seconds ( >= 100 )"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( chkAutoSaveAfterSecCount, 0, wxALL, 5 );

	edAutoSaveSecCount = new wxTextCtrl( pnlGeneral, wxID_ANY, _("100"), wxDefaultPosition, wxDefaultSize, 0 );
	edAutoSaveSecCount->SetMinSize( wxSize( 80,-1 ) );

	bSizer13->Add( edAutoSaveSecCount, 0, wxALL, 5 );

	chkShowTimeCountDown = new wxCheckBox( pnlGeneral, wxID_ANY, _("Show Time Countdown"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( chkShowTimeCountDown, 0, wxALL, 5 );

	chkShowKeyCountDown = new wxCheckBox( pnlGeneral, wxID_ANY, _("Show Keystroke Countdown"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer13->Add( chkShowKeyCountDown, 0, wxALL, 5 );


	bSizer8->Add( bSizer13, 1, wxEXPAND, 5 );


	bSizer71->Add( bSizer8, 1, wxEXPAND, 5 );


	pnlGeneral->SetSizer( bSizer71 );
	pnlGeneral->Layout();
	bSizer71->Fit( pnlGeneral );
	m_notebook3->AddPage( pnlGeneral, _("General"), false );
	pnlAppearence = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxHORIZONTAL );

	m_button41 = new wxButton( pnlAppearence, wxID_ANY, _("Chose Font"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button41, 0, wxALL, 5 );

	m_button18 = new wxButton( pnlAppearence, wxID_ANY, _("+"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button18, 0, wxALL, 5 );

	m_button19 = new wxButton( pnlAppearence, wxID_ANY, _("-"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_button19, 0, wxALL, 5 );

	btnDarkMode = new wxToggleButton( pnlAppearence, wxID_ANY, _("Dark Mode"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( btnDarkMode, 0, wxALL, 5 );


	bSizer2->Add( bSizer6, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxHORIZONTAL );

	textDemo = new wxTextCtrl( pnlAppearence, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB|wxWANTS_CHARS );
	bSizer7->Add( textDemo, 1, wxEXPAND, 5 );


	bSizer2->Add( bSizer7, 6, wxEXPAND, 5 );


	bSizer1->Add( bSizer2, 1, wxEXPAND, 5 );

	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( pnlAppearence, wxID_ANY, _("Light Mode") ), wxVERTICAL );

	m_button3 = new wxButton( sbSizer1->GetStaticBox(), wxID_ANY, _("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_button3, 0, wxALL, 5 );

	m_button4 = new wxButton( sbSizer1->GetStaticBox(), wxID_ANY, _("Foreground Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer1->Add( m_button4, 0, wxALL, 5 );


	bSizer3->Add( sbSizer1, 1, wxEXPAND, 5 );

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( pnlAppearence, wxID_ANY, _("Dark Mode") ), wxVERTICAL );

	m_button31 = new wxButton( sbSizer2->GetStaticBox(), wxID_ANY, _("Background Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_button31, 0, wxALL, 5 );

	m_button42 = new wxButton( sbSizer2->GetStaticBox(), wxID_ANY, _("Foreground Colour"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer2->Add( m_button42, 0, wxALL, 5 );


	bSizer3->Add( sbSizer2, 1, wxEXPAND, 5 );


	bSizer1->Add( bSizer3, 1, wxEXPAND, 5 );


	pnlAppearence->SetSizer( bSizer1 );
	pnlAppearence->Layout();
	bSizer1->Fit( pnlAppearence );
	m_notebook3->AddPage( pnlAppearence, _("Appearence"), false );
	pgHotKey = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxVERTICAL );

	m_staticText11 = new wxStaticText( pgHotKey, wxID_ANY, _("Double Click ( or Ctrl+Enter ) to set hotkey."), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	bSizer15->Add( m_staticText11, 0, wxALL, 5 );


	bSizer14->Add( bSizer15, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxVERTICAL );

	listHotKeys = new wxDataViewListCtrl( pgHotKey, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_ROW_LINES );
	bSizer16->Add( listHotKeys, 1, wxALL|wxEXPAND, 5 );


	bSizer14->Add( bSizer16, 1, wxEXPAND, 5 );


	pgHotKey->SetSizer( bSizer14 );
	pgHotKey->Layout();
	bSizer14->Fit( pgHotKey );
	m_notebook3->AddPage( pgHotKey, _("Hotkeys"), false );
	pnlAbout = new wxPanel( m_notebook3, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer( wxVERTICAL );

	m_staticText33 = new wxStaticText( pnlAbout, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText33->Wrap( -1 );
	m_staticText33->SetFont( wxFont( 18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText33, 0, wxALL, 5 );

	m_staticText3 = new wxStaticText( pnlAbout, wxID_ANY, _("WiseWriter"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText3->Wrap( -1 );
	m_staticText3->SetFont( wxFont( 18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText3, 0, wxALL|wxEXPAND, 5 );

	m_staticText32 = new wxStaticText( pnlAbout, wxID_ANY, _("Version 1.0.0 Beta"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText32->Wrap( -1 );
	m_staticText32->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText32, 0, wxALL|wxEXPAND, 5 );

	m_staticText35 = new wxStaticText( pnlAbout, wxID_ANY, _("© 2025 Cric. All rights reserved."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText35->Wrap( -1 );
	m_staticText35->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText35, 0, wxALL|wxEXPAND, 5 );

	m_staticText351 = new wxStaticText( pnlAbout, wxID_ANY, _("Built with wxWidgets (LGPL), wxFormBuilder (BSD 2)."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText351->Wrap( -1 );
	m_staticText351->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText351, 0, wxALL|wxEXPAND, 5 );

	m_staticText352 = new wxStaticText( pnlAbout, wxID_ANY, _("Develop Enviorment: Mac OS, C++, Object-C, Clang++, VS Code."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText352->Wrap( -1 );
	m_staticText352->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText352, 0, wxALL|wxEXPAND, 5 );

	m_staticText3511 = new wxStaticText( pnlAbout, wxID_ANY, _("Thank you~😍 open source community."), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL );
	m_staticText3511->Wrap( -1 );
	m_staticText3511->SetFont( wxFont( 24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );

	bSizer11->Add( m_staticText3511, 0, wxALL|wxEXPAND, 5 );


	pnlAbout->SetSizer( bSizer11 );
	pnlAbout->Layout();
	bSizer11->Fit( pnlAbout );
	m_notebook3->AddPage( pnlAbout, _("About"), false );

	bSizer4->Add( m_notebook3, 6, wxEXPAND | wxALL, 5 );

	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );

	m_button11 = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( m_button11, 0, wxALL, 5 );

	m_button12 = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( m_button12, 0, wxALL, 5 );

	m_button13 = new wxButton( this, wxID_RESET, _("OK and Restart"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( m_button13, 0, wxALL, 5 );


	bSizer4->Add( bSizer101, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer4 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	edAutoSaveKeyCount->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( DlgSettingsBase::OnLeaveAutoSaveCount ), NULL, this );
	edAutoSaveSecCount->Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( DlgSettingsBase::OnLeaveAutoSaveCount ), NULL, this );
	m_button41->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgSettingsBase::OnChoseFont ), NULL, this );
	m_button18->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( DlgSettingsBase::OnLargerFont ), NULL, this );
	m_button19->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( DlgSettingsBase::OnSmallerFont ), NULL, this );
	btnDarkMode->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( DlgSettingsBase::OnActDarkMode ), NULL, this );
	m_button3->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgSettingsBase::OnChoseLightBackgroundColor ), NULL, this );
	m_button4->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgSettingsBase::OnChoseLightForegroundColor ), NULL, this );
	m_button31->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgSettingsBase::OnChoseDarkBackgroundColor ), NULL, this );
	m_button42->Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DlgSettingsBase::OnChoseDarkForegroundColor ), NULL, this );
	m_button13->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgSettingsBase::OnOkRestart ), NULL, this );
}

DlgSettingsBase::~DlgSettingsBase()
{
}
