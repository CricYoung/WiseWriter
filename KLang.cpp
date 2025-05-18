
#include "KLang.hpp"
#include "KGlobal.h"
#include <wx/stdpaths.h> // For wxStandardPaths
#include <wx/config.h>


const std::map<std::string, int> gLangMap = {
	{"繁體中文", wxLANGUAGE_CHINESE_TRADITIONAL},
	{"简体中文", wxLANGUAGE_CHINESE_SIMPLIFIED},
	{"English", wxLANGUAGE_ENGLISH},
	{"日本語", wxLANGUAGE_JAPANESE},
	{"한국어", wxLANGUAGE_KOREAN},
	{"Français", wxLANGUAGE_FRENCH},
	{"Español", wxLANGUAGE_SPANISH},
	{"Deutsch", wxLANGUAGE_GERMAN},
	{"Italiano", wxLANGUAGE_ITALIAN},
	{"Português", wxLANGUAGE_PORTUGUESE},
	{"Русский", wxLANGUAGE_RUSSIAN},
	// {"Tiếng Việt", wxLANGUAGE_VIETNAMESE},
	// {"Bahasa Indonesia", wxLANGUAGE_INDONESIAN},
	{"Türkçe", wxLANGUAGE_TURKISH},
	{"Polski", wxLANGUAGE_POLISH},
	{"Nederlands", wxLANGUAGE_DUTCH},
	{"Svenska", wxLANGUAGE_SWEDISH},
	{"Dansk", wxLANGUAGE_DANISH},
	{"Suomi", wxLANGUAGE_FINNISH},
	// {"Norsk", wxLANGUAGE_NORWEGIAN},
	{"Hrvatski", wxLANGUAGE_CROATIAN},
	{"Slovenčina", wxLANGUAGE_SLOVAK},
	{"Slovenščina", wxLANGUAGE_SLOVENIAN},
	{"Magyar", wxLANGUAGE_HUNGARIAN},
	{"Čeština", wxLANGUAGE_CZECH},
	{"Ελληνικά", wxLANGUAGE_GREEK},
	{"Български", wxLANGUAGE_BULGARIAN},
	{"Română", wxLANGUAGE_ROMANIAN},
	{"עברית", wxLANGUAGE_HEBREW},
	{"العربية", wxLANGUAGE_ARABIC},
	{"हिन्दी", wxLANGUAGE_HINDI},
	// {"ไทย", wxLANGUAGE_THAI},
	// {"Bahasa Melayu", wxLANGUAGE_MALAY},
	// {"Filipino", wxLANGUAGE_FILIPINO}
};

//TODO:these sort names may not be correct
const std::map<std::string, int> gLangFileMap = {
	{"zh_TW", wxLANGUAGE_CHINESE_TRADITIONAL},
	{"zh_CN", wxLANGUAGE_CHINESE_SIMPLIFIED},
	{"en",    wxLANGUAGE_ENGLISH},
	{"ja",    wxLANGUAGE_JAPANESE},
	{"ko",    wxLANGUAGE_KOREAN},
	{"fr",    wxLANGUAGE_FRENCH},
	{"es",    wxLANGUAGE_SPANISH},
	{"de",    wxLANGUAGE_GERMAN},
	{"it",    wxLANGUAGE_ITALIAN},
	{"pt",    wxLANGUAGE_PORTUGUESE},
	{"ru",    wxLANGUAGE_RUSSIAN},
	// {"vi", wxLANGUAGE_VIETNAMESE},
	// {"id", wxLANGUAGE_INDONESIAN},
	{"tr",    wxLANGUAGE_TURKISH},
	{"pl",    wxLANGUAGE_POLISH},
	{"nl",    wxLANGUAGE_DUTCH},
	{"sv",    wxLANGUAGE_SWEDISH},
	{"da",    wxLANGUAGE_DANISH},
	{"fi",    wxLANGUAGE_FINNISH},
	// {"no", wxLANGUAGE_NORWEGIAN},
	{"hr",    wxLANGUAGE_CROATIAN},
	{"sk",    wxLANGUAGE_SLOVAK},
	{"sl",    wxLANGUAGE_SLOVENIAN},
	{"hu",    wxLANGUAGE_HUNGARIAN},
	{"cs",    wxLANGUAGE_CZECH},
	{"el",    wxLANGUAGE_GREEK},
	{"bg",    wxLANGUAGE_BULGARIAN},
	{"ro",    wxLANGUAGE_ROMANIAN},
	{"he",    wxLANGUAGE_HEBREW},
	{"ar",    wxLANGUAGE_ARABIC},
	{"hi",    wxLANGUAGE_HINDI},
	// {"th", wxLANGUAGE_THAI},
	// {"ms", wxLANGUAGE_MALAY},
	// {"fil", wxLANGUAGE_FILIPINO}
};

const std::map<int, std::string> gLangFileMapRev = {
	{wxLANGUAGE_CHINESE_TRADITIONAL, "zh_TW"},
	{wxLANGUAGE_CHINESE_SIMPLIFIED, "zh_CN"},
	{wxLANGUAGE_ENGLISH, "en"},
	{wxLANGUAGE_JAPANESE, "ja"},
	{wxLANGUAGE_KOREAN, "ko"},
	{wxLANGUAGE_FRENCH, "fr"},
	{wxLANGUAGE_SPANISH, "es"},
	{wxLANGUAGE_GERMAN, "de"},
	{wxLANGUAGE_ITALIAN, "it"},
	{wxLANGUAGE_PORTUGUESE, "pt"},
	{wxLANGUAGE_RUSSIAN, "ru"},
	// {wxLANGUAGE_VIETNAMESE, "vi"},
	// {wxLANGUAGE_INDONESIAN, "id"},
	{wxLANGUAGE_TURKISH, "tr"},
	{wxLANGUAGE_POLISH, "pl"},
	{wxLANGUAGE_DUTCH, "nl"},
	{wxLANGUAGE_SWEDISH, "sv"},
	{wxLANGUAGE_DANISH, "da"},
	{wxLANGUAGE_FINNISH, "fi"},
	// {wxLANGUAGE_NORWEGIAN, "no"},
	{wxLANGUAGE_CROATIAN, "hr"},
	{wxLANGUAGE_SLOVAK, "sk"},
	{wxLANGUAGE_SLOVENIAN, "sl"},
	{wxLANGUAGE_HUNGARIAN, "hu"},
	{wxLANGUAGE_CZECH, "cs"},
	{wxLANGUAGE_GREEK, "el"},
	{wxLANGUAGE_BULGARIAN, "bg"},
	{wxLANGUAGE_ROMANIAN, "ro"},
	{wxLANGUAGE_HEBREW, "he"},
	{wxLANGUAGE_ARABIC, "ar"},
	{wxLANGUAGE_HINDI, "hi"}
};

bool ConvertLangToCode(const wxString& tLang, int& tCode){
	tCode=-1; // Default to unknown language
	auto it = gLangMap.find(tLang.ToStdString().c_str());
	if (it == gLangMap.end()) return false; // Language not found
	tCode = it->second;
	return true;	
};

bool ConvertCodeToLangDirShortName(int tCode, wxString& tLang){
	wxLocale tLocale(tCode);
	if (!tLocale.IsOk()) return false; // Language not found
	tLang=tLocale.GetCanonicalName(); 
	return true;
	// auto it = gLangFileMapRev.find(tCode);
	// if (it == gLangFileMapRev.end()) return false; // Language not found
	// tLang = it->second;
	// return true;	
};


bool CheckOsLangAvailable(const wxString& tLang){
	int tLangCode;
	if(!ConvertLangToCode(tLang,tLangCode)) return 0; 
	if(tLangCode == wxLANGUAGE_UNKNOWN) return 0;
	wxLocale tLocale(tLangCode);
	if (!tLocale.IsOk()) return 0; // Language not found
	return 0x1; // App Language ok
	// return SetAppLangByLangCode(tLangCode,true); 
};

int CheckLangLocaleFilePathByLangCode(const int tLangCode){
	wxFileName tAppPath = wxStandardPaths::Get().GetExecutablePath();
	wxString tAppDir = tAppPath.GetPath();
	wxString tLocalePath = tAppDir + "/../Resources/locale";
	wxString tShortDirName;
	if(!ConvertCodeToLangDirShortName(tLangCode,tShortDirName)) return 0; 
	int tAns=0x1; // OS ok
	wxString tLangFileName=tLocalePath + "/" + tShortDirName + "/wxstd.mo";
	if (wxFileName::FileExists(tLangFileName)) tAns|=0x2; // wxLocale ok
	tLangFileName=tLocalePath + "/" + tShortDirName + "/" + tAppPath.GetName() + ".mo";
 	if (wxFileName::FileExists(tLangFileName)) tAns|=0x4; // AppLocale ok
	return tAns;
};

// if tIsAppLocale is true, tPath=fullpath of {AppName}.mo
// if tIsAppLocale is false, tPath=fullpath of wxstd.mo
// return true if file exists, false if not
bool GetLangLocaleFilePathByLangCode(const int tLangCode, wxString& tPath,bool tIsAppLocale){
	wxFileName tAppPath = wxStandardPaths::Get().GetExecutablePath();
	wxString tAppDir = tAppPath.GetPath();
	wxString tLocalePath = tAppDir + "/../Resources/locale";
	wxString tShortDirName;
	if(!ConvertCodeToLangDirShortName(tLangCode,tShortDirName)) return false; 
	if(tIsAppLocale) {
		tPath=tLocalePath + "/" + tShortDirName + "/" + tAppPath.GetName() + ".mo";
		return wxFileExists(tPath);
	}
	tPath=tLocalePath + "/" + tShortDirName + "/wxstd.mo";
	return wxFileName::FileExists(tPath);
};

int CheckAppLangAvailable(const wxString& tLang){
	int tLangCode;
	if(!ConvertLangToCode(tLang,tLangCode)) return 0; 
	if(tLangCode == wxLANGUAGE_UNKNOWN) return 0;
	return CheckLangLocaleFilePathByLangCode(tLangCode); // check if language is available
};

// check if language is available for current App
// An ORed int is returned, 
// 0: false 
// bit0(0x1): OS ok. 
// bit1(0x2): wxLocale ok, 
// bit2(0x4): AppLocale ok
int SetAppLangByLangCode(const int tLangCode,bool tOnlyCheck){
	wxLocale* tpLocale; 
	bool tSysLocale=false; 
	if(tOnlyCheck) {
	  tpLocale= new wxLocale;
		tpLocale->Init(tLangCode, wxLOCALE_DONT_LOAD_DEFAULT);
	}
  else{
		tpLocale = ::wxGetLocale();
  	tSysLocale=true; 
  	if(!tpLocale) {
  	  tpLocale = new wxLocale;  
  		tSysLocale=false;
  	};
  	tpLocale->Init(tLangCode); // Initialize with the system language
	};

	if (!tpLocale->IsOk())
	{
	  if(!tSysLocale)	delete tpLocale; // Clean up
		return 0;
	};

	int tAns=0x1; // OS ok
	wxFileName tAppPath = wxStandardPaths::Get().GetExecutablePath();
	wxString tAppDir = tAppPath.GetPath();
	wxString tLocalePath = tAppDir + "/../Resources/locale";
	tpLocale->AddCatalogLookupPathPrefix(tLocalePath); 
	//--- Check WxLocale ---
	// for like: Resources/locale/zh_TW/wxstd.mo or Resources/locale/ja/wxstd.mo ... files in the locale folder
	wxString tFileName=CWXLangName;
	if(tpLocale->AddCatalog(tFileName)) tAns|=0x2;
	// for like: Resources/locale/zh_TW/{AppName}.mo or Resources/locale/ja/{AppName}.mo ... files in the locale folder
	wxString tAppName=tAppPath.GetName();
	if(tpLocale->AddCatalog(tAppName)) tAns|=0x4;
	return tAns;
};

// check wxChose for language is available
bool CheckWxChoseLangAvailable(wxChoice* tChoice)
{
	//keep original language
	wxString tIniFile;
	::GetDefUsrIniFileName(tIniFile);
	int tLangIndex = GetPreferLang(tIniFile);

	for (size_t i = 0; i < tChoice->GetCount(); ++i) {
		wxString tLang = tChoice->GetString(i);
		if (CheckAppLangAvailable(tLang) > 0x1 ) continue; // App or WxLocale ok
		tChoice->Delete(i);
		i--; // Adjust index after deletion
	}
	// set back to original language
	::SetAppLangByLangCode(tLangIndex,true); // Set App Language
	return true;
}

int GetPreferLang(wxString& tUsrIniFileName)
{
	// Check if the file exists
	if (!wxFileName::FileExists(tUsrIniFileName)) {
		return -1; // File does not exist
	}
	// Load user preferences
	wxFileConfig tConfig("WiseWriter","Cric.Dev",tUsrIniFileName,wxEmptyString,wxCONFIG_USE_LOCAL_FILE);
	tConfig.SetPath("/Preferences");
	// Load language
	wxString LanguageName;
	bool tSetted = tConfig.Read("LanguageName", &LanguageName);
	if(!tSetted) return -1; // Language not set
	// Check if language is available
	int tLangIndex;
	if(ConvertLangToCode(LanguageName,tLangIndex) == false) 
		return -1;
	return tLangIndex;
}  
 
std::string GetWxPathByWxConfig() {
    FILE* pipe = popen("wx-config --prefix", "r");
    if (!pipe) return "";
    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    // 移除可能的換行符
    result.erase(std::remove(result.begin(), result.end(), '\n'), result.end());
    return result;
}
