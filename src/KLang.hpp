
#pragma once
// === KLang ===
#include <map>
#include <wx/wx.h>
#include <wx/choice.h>
// --- Lang Constants ---
const std::string CWXLangName="wxstd"; // Catalog name for wxLocale
const std::string CAppLocaleRefPathInAppBundle="/../Resources/locale"; // App locale path ( in a App bundle)
const std::string CAppLocaleRefPathInExeDir="/Resources/locale"; // Exe locale path (refer to executable file)
// --- Lang Global Variables ---
extern const std::map<std::string, int> gLangMap;
extern const std::map<std::string, int> gLangFileMap;
extern const std::map<int, std::string> gLangFileMapRev;
// --- Lang Global Functions ---
int GetPreferLang(wxString& tIniFileName); // get prefer language from ini file
bool ConvertLangToCode(const wxString& tLang, int& tCode); // convert language to code (wxLANGUAGE_XXX) ...^_^... Copilot You are so smart!
bool ConvertCodeToLangDirShortName(int tCode, wxString& tLang); // convert code to language file name
// check if language is supported by OS
bool CheckOsLangAvailable(const wxString& tLang); 
// if tIsAppLocale is true, tPath=fullpath of {AppName}.mo
// if tIsAppLocale is false, tPath=fullpath of wxstd.mo
// return true if file exists, false if not
// tRefPath may be CAppLocalePathInAppBundle or CAppLocalePathInExeDir
bool GetLangLocaleFilePathByLangCode(const int tLangCode, wxString& tPath,bool tIsAppLocale,const wxString& tRefPath); // get language locale file path
// check if language is available for current App
// An ORed int is returned, 
// 0: false 
// bit0(0x1): OS ok. 
// bit1(0x2): wxLocale ok, 
// bit2(0x4): AppLocale ok
int CheckLangLocaleFilePathByLangCode(const int tLangCode); // check if language locale file path is available
// check if language is available for current App
// An ORed int is returned, 
// 0: false 
// bit0(0x1): OS ok. 
// bit1(0x2): wxLocale ok, 
// bit2(0x4): AppLocale ok
int SetAppLangByLangCode(const int tLangCode,bool tOnlyCheck); // Set App Language or check if language is available for current App
bool CheckWxChoseLangAvailable(wxChoice* tChoice); // delete unavailable language in wxChoice 
int GetPreferLang(wxString& tIniFileName); // get prefer language from ini file