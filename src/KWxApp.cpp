//KWxApp.cpp
#include <wx/stdpaths.h> // For wxStandardPaths
#include <wx/filename.h> // For wxFileName

#include "KWxApp.hpp"
#include "WiseWriter.hpp" // For MyFrame
#include "KGlobal.hpp" // For global functions
#include "LinkOS.h" // For LinkOS functions
#include "KLang.hpp" // For language functions
int VimTestsMain(int argc, char **argv) ;

bool MyApp::IniLocalize()
{
	// find os language
	int tLang = wxLocale::GetSystemLanguage(); 
	if(tLang == wxLANGUAGE_UNKNOWN) 
	  tLang = wxLANGUAGE_ENGLISH; // Default to English if unknown
		
	wxString tIniFileName;
	GetAvailableIniFullName(tIniFileName,true); // Get default ini file name
	int tPreferLang = GetPreferLang(tIniFileName); // Get prefer language from ini file
	if(tPreferLang != -1) 
	  tLang = tPreferLang; // Use prefer language if available

	if(::SetAppLangByLangCode(tLang,false) == 0) {
		wxString tMsg = wxString::Format(_("Language %s is not available."),wxLocale::GetLanguageName(tLang));
		wxMessageBox(tMsg, _("WiseWriter"), wxOK | wxICON_INFORMATION);
		return false;
	};
	return true;

	// wxLocale* locale = ::wxGetLocale();
	// bool tSysLocale=true; 
	// if(!locale) {
	//   locale = new wxLocale(tLang); 
	// 	tSysLocale=false;
	// }
	// else locale->Init(tLang); // Initialize with the system language

	// if (!locale->IsOk())
	// {
	// 	wxLogError("Failed to set locale to %s.", wxLocale::GetLanguageName(tLang));
	//   if(!tSysLocale)	delete locale; // Clean up
	// 	return false;
	// };

	// //try App folder
	// wxFileName tAppPath = wxStandardPaths::Get().GetExecutablePath();
	// wxString tAppDir = tAppPath.GetPath();
	// wxString tLocalePath = tAppDir + "/../Resources/locale";
	// locale->AddCatalogLookupPathPrefix(tLocalePath); 
	// bool tOsLangOk=locale->AddCatalog(CWXLangName);
	
	// wxString tAppName = tAppPath.GetName();
	// bool tWiseLangOk=locale->AddCatalog(tAppName);
	// if(tWiseLangOk && tOsLangOk) return true;

	// // for like "zh_TW.mo" or "ja.mo" ... files in the locale folder
	// wxString tLangFileName;
	// if(!ConvertCodeToLangFile(tLang,tLangFileName)) return false; // Convert code to language file name
	// bool tLangFileOk=locale->AddCatalog(tLangFileName);
	// if(tLangFileOk) return true; // If the catalog is found, return true
	// // for like "WiseWriter_zh_TW.mo" or "WiseWriter_ja.mo" ... files in the locale folder
	// wxString tAppName=wxStandardPaths::Get().GetExecutableName();
	// tLangFileName = tAppName + "_" + tLangFileName;
	// tLangFileOk=locale->AddCatalog(tLangFileName);
	// if(tLangFileOk) return true; // If the catalog is found, return true
	// wxLogMessage("Failed to add KLang(and WiseWriter) catalog: %s.",tLocalePath.ToStdString().c_str()); 
	// if(!tSysLocale)	delete locale; // Clean up
	// return false;
}

bool MyApp::OnInit()
{
	IniLocalize(); // Initialize localization
	frame = new MyFrame(); // Create the frame


	if(!CheckIfOsTrusted()){
    wxMessageBox(_("WiseWriter needs system permission to PASTE-BACK to other APP. Please go to System Preferences > Security & Privacy > Privacy > Accessibility and add WiseWriter to the list."),
			_("WiseWriter needs system permission"), wxOK | wxICON_INFORMATION);
  };
	frame->LoadAppPrefer(); // Load app Prefeif(bSummonKeyRegistered) ::UnregisterSummonKey(); // Unregister the previous hotkey
	frame->RegistSummonKey(); // Register the summon key	
	frame->Show(); // Optional: Show the frame immediately
	// get parameter
	MyApp* app = dynamic_cast<MyApp*>(wxApp::GetInstance());
	wxString tCmdLine;
	if (app && app->argc > 1) {
		tCmdLine = app->argv[1];
		if(tCmdLine == "--restart") {
			ActivateAppWindow(); // Bring the app to front
		};
	}
	VimTestsMain(app->argc, app->argv); // Run tests if needed
	return true; // Initialization successful
}

int MyApp::OnExit()
{
	if(appLocale) delete appLocale; // Clean up locale
  return wxApp::OnExit(); // Call base class implementation
};
// Static Hotkey Handler - Called by Carbon via LinkOS.mm registration
// void MyApp::SummonHotkeyHandler(void *userData)
// {
// 	MyFrame *frame = static_cast<MyFrame *>(userData);
// 	if (frame)
// 	{
// 		// Check if the frame is already visible (e.g., user clicked away and hit hotkey again)
// 		if (!frame->IsShown())
// 		{
// 			::RememberFrontApp(); // Remember the app that was active *before* we activate
// 		}
// 		// Use ObjC function (from LinkOS module) to bring *this app* to front
// 		::ActivateAppWindow();
// 		// Now show and raise the C++ window
// 		frame->Show();
// 		frame->Raise();
// 		// Set focus specifically to the text input
// 		frame->input->SetFocus();
// 		// Optionally reset mode to Edit when shown
// 	  frame->EnterEditMode();
// 	  frame->ShowStatusHistoryIndex();
// 	}
// 	else
// 	{
// 		wxLogError("SummonHotkeyHandler called with null userData!");
// 	}
// 	// No return value needed for the generic callback
// }


wxIMPLEMENT_APP(MyApp);
