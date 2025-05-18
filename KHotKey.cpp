// KHotKey.cpp
#include "KHotKey.h"
#include <wx/strconv.h>
#include <sstream>

std::string WxKeyCodeToString(int keyCode) {
  if(wxIsascii(keyCode) && wxIsprint(keyCode)) {
    return std::string(1, static_cast<char>(keyCode));
  }
//   if (keyCode >= 'A' && keyCode <= 'Z') return std::string(1, static_cast<char>(keyCode));
//   if (keyCode >= '0' && keyCode <= '9') return std::string(1, static_cast<char>(keyCode));
  if(keyCode >= WXK_NUMPAD0 && keyCode <= WXK_NUMPAD9) {
    std::string tStr;
    tStr += "NumPad";
    tStr += std::to_string(keyCode - WXK_NUMPAD0);
    return tStr;
  }
  if(keyCode >= WXK_F1 && keyCode <= WXK_F24) {
    return "F" + std::to_string(keyCode - WXK_F1 + 1);
  }
  switch (keyCode) {
    case WXK_ADD: return "+";
    case WXK_SUBTRACT: return "-";
    case WXK_MULTIPLY: return "*";
    case WXK_DIVIDE: return "/";
    // case WXK_NUMPAD0: return "NumPad0";
    // case WXK_NUMPAD1: return "NumPad1";
    // case WXK_NUMPAD2: return "NumPad2";
    // case WXK_NUMPAD3: return "NumPad3";
    // case WXK_NUMPAD4: return "NumPad4";
    // case WXK_NUMPAD5: return "NumPad5";
    // case WXK_NUMPAD6: return "NumPad6";
    // case WXK_NUMPAD7: return "NumPad7";
    // case WXK_NUMPAD8: return "NumPad8";
    // case WXK_NUMPAD9: return "NumPad9";
    case WXK_NUMPAD_ADD: return "NumPad+";
    case WXK_NUMPAD_SUBTRACT: return "NumPad-";
    case WXK_NUMPAD_MULTIPLY: return "NumPad*";
    case WXK_NUMPAD_DIVIDE: return "NumPad/";
    case WXK_NUMPAD_DECIMAL: return "NumPad.";
    case WXK_NUMPAD_ENTER: return "NumPadEnter";
    case WXK_SPACE: return "Space";
    case WXK_RETURN: return "Enter";
    case WXK_BACK: return "Backspace";
    case WXK_TAB: return "Tab";
    case WXK_ESCAPE: return "Escape";
    case WXK_DELETE: return "Delete";
    case WXK_HOME: return "Home";
    case WXK_END: return "End";
    case WXK_PAGEUP: return "PageUp";
    case WXK_PAGEDOWN: return "PageDown";
    case WXK_LEFT: return "Left";
    case WXK_RIGHT: return "Right";
    case WXK_UP: return "Up";
    case WXK_DOWN: return "Down";
    // case WXK_F1: return "F1";
    // case WXK_F2: return "F2";
    // case WXK_F3: return "F3";
    // case WXK_F4: return "F4";
    // case WXK_F5: return "F5";
    // case WXK_F6: return "F6";
    // case WXK_F7: return "F7";
    // case WXK_F8: return "F8";
    // case WXK_F9: return "F9";
    // case WXK_F10: return "F10";
    // case WXK_F11: return "F11";
    // case WXK_F12: return "F12";
    default:   // 1 ~ 0x1A
      if(keyCode >= 1  && keyCode <= 0x1A){
        std::string tStr;
        tStr += "Ctrl+";
        tStr += 'A' + (keyCode - 1);
        return tStr;
      }   
      return ""; // Unknown key
  }
}

bool Key2Str(const StKey& key, std::string& outStr) {
    std::ostringstream oss;

    if (key.bCtrl)  oss << "Ctrl+";  
    if (key.bCmd)   oss << "Cmd+";  
    if (key.bAlt)   oss << "Alt+";
    if (key.bShift) oss << "Shift+";

    std::string keyPart = WxKeyCodeToString(key.keyCode);
    if (keyPart.empty()) return false;

    oss << keyPart;
    outStr = oss.str();
    return true;
}

bool StringToWxKeyCode(const wxString& keyStr, int& keyCodeOut) {
    if(keyStr.IsEmpty()) return false;
    wxString k = keyStr.Upper();
    if(k.Len() == 1 && wxIsascii(k[0]) && wxIsprint(k[0])) {
        keyCodeOut = static_cast<int>(k[0].GetValue());
        return true;
    }
    // '0' ~ '9' and 'A' ~ 'Z'
    // if (k.Len() == 1 && wxIsalnum(k[0])) {
        // keyCodeOut = static_cast<int>(k[0].GetValue());
        // return true;
    // }
    // 'F1' ~ 'F24'
    if (k.StartsWith("F") && k.Len() >= 2) {
        long fnum;
        if (k.Mid(1).ToLong(&fnum) && fnum >= 1 && fnum <= 24) {
            keyCodeOut = WXK_F1 + (fnum - 1);
            return true;
        }
    }

    // 'NUMPAD0' ~ 'NUMPAD9'
    if(k.StartsWith("NUMPAD") && k.Len() >= 7) {
        long fnum;
        if (k.Mid(6).ToLong(&fnum) && fnum >= 0 && fnum <= 9) {
            keyCodeOut = WXK_NUMPAD0 + (fnum);
            return true;
        }
    }

    // if (k == "NUMPAD0")    keyCodeOut = WXK_NUMPAD0;
    // else if (k == "NUMPAD1")    keyCodeOut = WXK_NUMPAD1;
    // else if (k == "NUMPAD2")    keyCodeOut = WXK_NUMPAD2;
    // else if (k == "NUMPAD3")    keyCodeOut = WXK_NUMPAD3;
    // else if (k == "NUMPAD4")    keyCodeOut = WXK_NUMPAD4;
    // else if (k == "NUMPAD5")    keyCodeOut = WXK_NUMPAD5;
    // else if (k == "NUMPAD6")    keyCodeOut = WXK_NUMPAD6;
    // else if (k == "NUMPAD7")    keyCodeOut = WXK_NUMPAD7;
    // else if (k == "NUMPAD8")    keyCodeOut = WXK_NUMPAD8;
    // else if (k == "NUMPAD9")    keyCodeOut = WXK_NUMPAD9;
    if (k == "NUMPAD+")  keyCodeOut = WXK_ADD;
    else if (k == "NUMPAD-")  keyCodeOut = WXK_SUBTRACT;
    else if (k == "NUMPAD*")  keyCodeOut = WXK_MULTIPLY;
    else if (k == "NUMPAD/")  keyCodeOut = WXK_DIVIDE;
    else if (k == "NUMPADENTER")  keyCodeOut = WXK_RETURN;
    else if (k == "NUMPAD.")  keyCodeOut = WXK_NUMPAD_DECIMAL;
    else if (k == "SPACE")      keyCodeOut = WXK_SPACE;
    else if (k == "ENTER")      keyCodeOut = WXK_RETURN;
    else if (k == "RETURN")      keyCodeOut = WXK_RETURN;
    else if (k == "BACKSPACE")  keyCodeOut = WXK_BACK;
    else if (k == "TAB")        keyCodeOut = WXK_TAB;
    else if (k == "ESCAPE")     keyCodeOut = WXK_ESCAPE;
    else if (k == "ESC")     keyCodeOut = WXK_ESCAPE;
    else if (k == "DELETE")     keyCodeOut = WXK_DELETE;
    else if (k == "DEL")     keyCodeOut = WXK_DELETE;
    else if (k == "HOME")       keyCodeOut = WXK_HOME;
    else if (k == "END")        keyCodeOut = WXK_END;
    else if (k == "PAGEUP")     keyCodeOut = WXK_PAGEUP;
    else if (k == "PGUP")       keyCodeOut = WXK_PAGEUP;
    else if (k == "PAGEDOWN")   keyCodeOut = WXK_PAGEDOWN;
    else if (k == "PAGEDN")     keyCodeOut = WXK_PAGEDOWN;
    else if (k == "PGDN")       keyCodeOut = WXK_PAGEDOWN;
    else if (k == "UP")         keyCodeOut = WXK_UP;
    else if (k == "DOWN")       keyCodeOut = WXK_DOWN;
    else if (k == "DN")       keyCodeOut = WXK_DOWN;
    else if (k == "LEFT")       keyCodeOut = WXK_LEFT;
    else if (k == "RIGHT")      keyCodeOut = WXK_RIGHT;
    else return false;

    return true;
}

bool Str2Key(const std::string& inputStr, StKey& outKey) {
    wxString wxInput(inputStr);
    wxArrayString parts = wxSplit(wxInput, '+');
    outKey = StKey();

    for (auto& part : parts) {
        part = part.Trim().Trim(false).Upper();

        if      (part == "CTRL")  outKey.bCtrl  = true;
        else if (part == "ALT")   outKey.bAlt   = true;
        else if (part == "SHIFT") outKey.bShift = true;
        else if (part == "CMD" || part == "META") outKey.bCmd = true;
        else {
            int keyCode = WXK_NONE;
            if (!StringToWxKeyCode(part, keyCode)) return false;
            outKey.keyCode = keyCode;
        }
    }

    return outKey.keyCode != WXK_NONE;
}
