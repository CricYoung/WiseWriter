// KHotKey.cpp
#include "KHotKey.h"
#include <wx/strconv.h>
#include <sstream>

bool WxKeyCodeToString(int keyCode, std::string& outStr) {

  // printable
  if(wxIsascii(keyCode) && wxIsprint(keyCode)) {
    outStr=std::string(1, static_cast<char>(keyCode));
    return true;
  }

  // numpad 0 ~ 9
  if(keyCode >= WXK_NUMPAD0 && keyCode <= WXK_NUMPAD9) {
    std::string tStr;
    tStr += "NumPad";
    tStr += std::to_string(keyCode - WXK_NUMPAD0);
    outStr=tStr;
    return true;
  }

  // F1 ~ F24
  if(keyCode >= WXK_F1 && keyCode <= WXK_F24) {
    outStr="F" + std::to_string(keyCode - WXK_F1 + 1);
    return true;
  }

  //others
  switch (keyCode) {
    case WXK_ADD: outStr="+"; break;
    case WXK_SUBTRACT: outStr= "-"; break;
    case WXK_MULTIPLY: outStr= "*"; break;
    case WXK_DIVIDE: outStr= "/"; break;
    case WXK_NUMPAD_ADD: outStr= "NumPad+"; break;
    case WXK_NUMPAD_SUBTRACT: outStr= "NumPad-"; break;
    case WXK_NUMPAD_MULTIPLY: outStr= "NumPad*"; break;
    case WXK_NUMPAD_DIVIDE: outStr= "NumPad/"; break;
    case WXK_NUMPAD_DECIMAL: outStr= "NumPad."; break;
    case WXK_NUMPAD_ENTER: outStr= "NumPadEnter"; break;
    case WXK_SPACE: outStr= "Space"; break;
    case WXK_RETURN: outStr= "Enter"; break;
    case WXK_BACK: outStr= "Backspace"; break;
    case WXK_TAB: outStr= "Tab"; break;
    case WXK_ESCAPE: outStr= "Escape"; break;
    case WXK_DELETE: outStr= "Delete"; break;
    case WXK_HOME: outStr= "Home"; break;
    case WXK_END: outStr= "End"; break;
    case WXK_PAGEUP: outStr= "PageUp"; break;
    case WXK_PAGEDOWN: outStr= "PageDown"; break;
    case WXK_LEFT: outStr= "Left"; break;
    case WXK_RIGHT: outStr= "Right"; break;
    case WXK_UP: outStr= "Up"; break;
    case WXK_DOWN: outStr= "Down"; break;
    default:   // 1 ~ 0x1A
      if(keyCode >= 1  && keyCode <= 0x1A){
        std::string tStr;
        tStr += "Ctrl+";
        tStr += 'A' + (keyCode - 1);
        outStr= tStr;
        break;
      }   
      return false; // Unknown key
  }
  return true;
}

bool Key2Str(const StKey& key, std::string& outStr) {
    std::ostringstream oss;

    if (key.bCtrl)  oss << "Ctrl+";  
    if (key.bCmd)   oss << "Cmd+";  
    if (key.bAlt)   oss << "Alt+";
    if (key.bShift) oss << "Shift+";

    std::string keyPart;
    if(!WxKeyCodeToString(key.keyCode, keyPart)) return false; // Unknown key

    oss << keyPart;
    outStr = oss.str();
    return true;
}

bool StringToWxKeyCode(const wxString& keyStr, int& keyCodeOut) {
    if(keyStr.IsEmpty()) return false;
    //printable
    wxString keyUpperStr = keyStr.Upper();
    if(keyUpperStr.Len() == 1 && wxIsascii(keyUpperStr[0]) && wxIsprint(keyUpperStr[0])) {
        keyCodeOut = static_cast<int>(keyUpperStr[0].GetValue());
        return true;
    }

    // 'F1' ~ 'F24'
    if (keyUpperStr.StartsWith("F") && keyUpperStr.Len() >= 2) {
        long fnum;
        if (keyUpperStr.Mid(1).ToLong(&fnum) && fnum >= 1 && fnum <= 24) {
            keyCodeOut = WXK_F1 + (fnum - 1);
            return true;
        }
    }

    // 'NUMPAD0' ~ 'NUMPAD9'
    if(keyUpperStr.StartsWith("NUMPAD") && keyUpperStr.Len() >= 7) {
        long fnum;
        if (keyUpperStr.Mid(6).ToLong(&fnum) && fnum >= 0 && fnum <= 9) {
            keyCodeOut = WXK_NUMPAD0 + (fnum);
            return true;
        }
    }

    // others
    if (keyUpperStr == "NUMPAD+")  keyCodeOut = WXK_ADD;
    else if (keyUpperStr == "NUMPAD-")  keyCodeOut = WXK_SUBTRACT;
    else if (keyUpperStr == "NUMPAD*")  keyCodeOut = WXK_MULTIPLY;
    else if (keyUpperStr == "NUMPAD/")  keyCodeOut = WXK_DIVIDE;
    else if (keyUpperStr == "NUMPADENTER")  keyCodeOut = WXK_RETURN;
    else if (keyUpperStr == "NUMPAD.")  keyCodeOut = WXK_NUMPAD_DECIMAL;
    else if (keyUpperStr == "SPACE")      keyCodeOut = WXK_SPACE;
    else if (keyUpperStr == "ENTER")      keyCodeOut = WXK_RETURN;
    else if (keyUpperStr == "RETURN")      keyCodeOut = WXK_RETURN;
    else if (keyUpperStr == "BACKSPACE")  keyCodeOut = WXK_BACK;
    else if (keyUpperStr == "TAB")        keyCodeOut = WXK_TAB;
    else if (keyUpperStr == "ESCAPE")     keyCodeOut = WXK_ESCAPE;
    else if (keyUpperStr == "ESC")     keyCodeOut = WXK_ESCAPE;
    else if (keyUpperStr == "DELETE")     keyCodeOut = WXK_DELETE;
    else if (keyUpperStr == "DEL")     keyCodeOut = WXK_DELETE;
    else if (keyUpperStr == "HOME")       keyCodeOut = WXK_HOME;
    else if (keyUpperStr == "END")        keyCodeOut = WXK_END;
    else if (keyUpperStr == "PAGEUP")     keyCodeOut = WXK_PAGEUP;
    else if (keyUpperStr == "PGUP")       keyCodeOut = WXK_PAGEUP;
    else if (keyUpperStr == "PAGEDOWN")   keyCodeOut = WXK_PAGEDOWN;
    else if (keyUpperStr == "PAGEDN")     keyCodeOut = WXK_PAGEDOWN;
    else if (keyUpperStr == "PGDN")       keyCodeOut = WXK_PAGEDOWN;
    else if (keyUpperStr == "UP")         keyCodeOut = WXK_UP;
    else if (keyUpperStr == "DOWN")       keyCodeOut = WXK_DOWN;
    else if (keyUpperStr == "DN")       keyCodeOut = WXK_DOWN;
    else if (keyUpperStr == "LEFT")       keyCodeOut = WXK_LEFT;
    else if (keyUpperStr == "RIGHT")      keyCodeOut = WXK_RIGHT;
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
