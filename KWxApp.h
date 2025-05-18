//KWxApp.h

#pragma once
#include <wx/wx.h>
#include <wx/uilocale.h> // For wxLocale

class MyFrame; // Forward declaration of MyFrame

class MyApp : public wxApp
{
protected:
 wxLocale *appLocale=nullptr;
public:
	MyFrame *frame = nullptr;
	bool IniLocalize();
	virtual bool OnInit() override;
	// Declare OnExit
	virtual int OnExit() override;
	// Static handler now uses the generic signature
	// static void SummonHotkeyHandler(void *userData);
};
