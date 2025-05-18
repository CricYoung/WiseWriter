
#pragma once

#include "LinkOS.h" // For StKey
#include <wx/wx.h>
#include <wx/defs.h>

// for Key mapping and hotkey
struct StKey {
  bool bCtrl = false;
  bool bAlt = false;
  bool bShift = false;
  bool bCmd = false;
  int keyCode = WXK_NONE; // 直接用 wxWidgets 的 key code
  StKey(bool tCtrl = false, bool tAlt = false, bool tShift = false, bool tCmd = false, int tKeyCode = WXK_NONE) :
    bCtrl(tCtrl), bAlt(tAlt), bShift(tShift), bCmd(tCmd), keyCode(tKeyCode) {}
  bool operator==(const StKey& tKey) const {
    return (bCtrl == tKey.bCtrl && bAlt == tKey.bAlt && bShift == tKey.bShift && bCmd == tKey.bCmd && keyCode == tKey.keyCode);
  }
  bool operator!=(const StKey& tKey) const {
    return !(*this == tKey);
  }
  bool IsEmpty() const {
    return (keyCode == WXK_NONE);
  }
  int GetModifier() const {
    int tModifier = 0;
    if (bCtrl) tModifier |= EOptionKey::eCtrl;
    if (bAlt) tModifier |= EOptionKey::eAlt;
    if (bShift) tModifier |= EOptionKey::eShift;
    if (bCmd) tModifier |= EOptionKey::eCmd;
    return tModifier;
  }
  int GetCarbonModifier() const {
    return StKeyModifier2CarbonModifier(GetModifier());
  }
};

std::string WxKeyCodeToString(int keyCode) ;

bool Key2Str(const StKey& key, std::string& outStr) ;
bool StringToWxKeyCode(const wxString& keyStr, int& keyCodeOut) ;
bool Str2Key(const std::string& inputStr, StKey& outKey); 
