#import <Cocoa/Cocoa.h>
#import <Carbon/Carbon.h> // Keep Carbon for Hotkeys
#import <ApplicationServices/ApplicationServices.h>
#import <wx/defs.h>

#import "LinkOS.h"

// Global/static storage for hotkey reference and callback
static EventHotKeyRef gAppHotKeyRef = nullptr;
static EventHotKeyID gAppHotKeyID = {'htk1', 1};
static GenericAppHotKeyCallback gGenericCallback = nullptr;
static void* gCallbackUserData = nullptr;
static EventHandlerRef gAppEventHandlerRef = nullptr; // Keep track of the handler

// 放在 global 區或 class 裡
static pid_t g_previousAppPID = 0;

int StKeyModifier2CarbonModifier(int tAlter) {
    int carbonModifier = 0;
    if (tAlter & eCtrl) {
        carbonModifier |= controlKey;
    }
    if (tAlter & eAlt) {
        carbonModifier |= optionKey;
    }
    if (tAlter & eShift) {
        carbonModifier |= shiftKey;
    }
    if (tAlter & eCmd) {
        carbonModifier |= cmdKey;
    }
    return carbonModifier;
}

// Check if the app is trusted for accessibility
bool CheckIfOsTrusted() {
  bool trusted = AXIsProcessTrusted();

  return trusted;
};

bool CheckIfOsTrustedWithOption() {
  CFDictionaryRef options = (__bridge CFDictionaryRef)@{(__bridge id)kAXTrustedCheckOptionPrompt: @YES};
  bool trusted = AXIsProcessTrustedWithOptions(options);
  return trusted;
};
// --- Internal Carbon Hotkey Handler ---
// This is the actual function registered with Carbon.
OSStatus InternalCarbonHotKeyHandler(EventHandlerCallRef , EventRef theEvent, void *) {
    // Extract the hot key ID
    EventHotKeyID hotKeyID;
    GetEventParameter(theEvent, kEventParamDirectObject, typeEventHotKeyID, NULL, sizeof(hotKeyID), NULL, &hotKeyID);

    // Check if it's our hotkey
    if (hotKeyID.signature == gAppHotKeyID.signature && hotKeyID.id == gAppHotKeyID.id) {
        // Call the generic callback that was registered
        if (gGenericCallback) {
            // Use the stored user data
            gGenericCallback(gCallbackUserData);
        }
        return noErr; // Handled
    } else {
        // Not our hotkey, pass it on
        return eventNotHandledErr;
        // Or CallNextEventHandler(nextHandler, theEvent); if needed
    }
}

// --- Existing Function Implementations ---
void RememberFrontApp() {
    // Ensure this app itself isn't remembered if it's frontmost when called
    NSRunningApplication* currentApp = [NSRunningApplication currentApplication];
    NSRunningApplication* frontApp = [[NSWorkspace sharedWorkspace] frontmostApplication];
    if (frontApp && frontApp.processIdentifier != currentApp.processIdentifier) {
         g_previousAppPID = frontApp.processIdentifier;
         // NSLog(@"Remembered App PID: %d", g_previousAppPID);
    } else {
         // NSLog(@"Not remembering self or no front app.");
         // Optionally clear or keep the last valid PID
         // g_previousAppPID = 0;
    }
}

void RestoreFocusToPreviousApp() {
    if (g_previousAppPID != 0) {
        NSRunningApplication* app = [NSRunningApplication runningApplicationWithProcessIdentifier:g_previousAppPID];
        if (app) {
            // NSLog(@"Restoring focus to PID: %d", g_previousAppPID);
            [app activateWithOptions:(NSApplicationActivateIgnoringOtherApps)];
            // usleep might not be needed here, test without it first
            // usleep(200000);
        } else {
             // NSLog(@"Failed to find app for PID: %d", g_previousAppPID);
        }
    } else {
        // NSLog(@"No previous app PID to restore focus to.");
    }
}

void SetClipboardText(const std::string& text) {
    NSPasteboard* pasteboard = [NSPasteboard generalPasteboard];
    [pasteboard clearContents];
    NSString* nsStr = [NSString stringWithUTF8String:text.c_str()];
    NSArray* objectsToCopy = @[nsStr];
    [pasteboard writeObjects:objectsToCopy];
}

void SendPasteGemini() {
    // Use newer API if possible, but CGEvent works
    CGEventSourceRef source = CGEventSourceCreate(kCGEventSourceStateHIDSystemState);

    CGEventRef cmdDown = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_Command, true);
    CGEventRef vDown   = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_ANSI_V, true);
    CGEventSetFlags(vDown, kCGEventFlagMaskCommand); // Add Command flag to V down
    CGEventRef vUp     = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_ANSI_V, false);
     CGEventSetFlags(vUp, kCGEventFlagMaskCommand); // Add Command flag to V up
    CGEventRef cmdUp   = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_Command, false);

    // Post Cmd+V
    CGEventPost(kCGHIDEventTap, cmdDown);
    CGEventPost(kCGHIDEventTap, vDown);
    CGEventPost(kCGHIDEventTap, vUp);
    CGEventPost(kCGHIDEventTap, cmdUp);

    // Optional: Add a small delay between Paste and Enter if needed
    usleep(50000); // 50ms delay

    // Post Enter
    // CGEventRef enterDown = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_Return, true);
    // CGEventRef enterUp   = CGEventCreateKeyboardEvent(source, (CGKeyCode)kVK_Return, false);
    // CGEventPost(kCGHIDEventTap, enterDown);
    // CGEventPost(kCGHIDEventTap, enterUp);

    CFRelease(cmdDown);
    CFRelease(vDown);
    CFRelease(vUp);
    CFRelease(cmdUp);
    // CFRelease(enterDown);
    // CFRelease(enterUp);
    CFRelease(source);
}

//sometimes it doesn't work, so use SendCmdTab instead
void SendCmdTabGemini() {
     // This is generally unreliable for switching back programmatically.
     // activateWithOptions is preferred.
     NSLog(@"SendCmdTab is often unreliable; using activateWithOptions is better.");
    // CGEventRef cmdDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)kVK_Command, true);
    // CGEventRef tabDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)kVK_Tab, true);
    // CGEventSetFlags(tabDown, kCGEventFlagMaskCommand);
    // CGEventRef tabUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)kVK_Tab, false);
    // CGEventSetFlags(tabUp, kCGEventFlagMaskCommand);
    // CGEventRef cmdUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)kVK_Command, false);

    // CGEventPost(kCGHIDEventTap, cmdDown);
    // CGEventPost(kCGHIDEventTap, tabDown);
    // usleep(50000); // Short delay while holding tab
    // CGEventPost(kCGHIDEventTap, tabUp);
    // CGEventPost(kCGHIDEventTap, cmdUp);
    // CFRelease(cmdDown);
    // CFRelease(tabDown);
    // CFRelease(tabUp);
    // CFRelease(cmdUp);
}
// ==== 模擬 Cmd+V ====
void SendPaste() {
    CGEventRef cmdDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x38, true); // Cmd
    CGEventRef vDown   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)9, true);   // V
    CGEventRef vUp     = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)9, false);
    CGEventRef cmdUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x38, false);
    
    CGEventSetFlags(vDown, kCGEventFlagMaskCommand);
    CGEventSetFlags(vUp,   kCGEventFlagMaskCommand);
    
    CGEventPost(kCGHIDEventTap, cmdDown);
    CGEventPost(kCGHIDEventTap, vDown);
    CGEventPost(kCGHIDEventTap, vUp);
    CGEventPost(kCGHIDEventTap, cmdUp);

    // usleep(100000); // 等一下再送 Enter
    
    // CGEventRef enterDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)36, true); // Enter
    // CGEventRef enterUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)36, false);
    // CGEventPost(kCGHIDEventTap, enterDown);
    // CGEventPost(kCGHIDEventTap, enterUp);
}
// 模擬 Cmd+Tab 切回去
void SendCmdTab() {
    CGEventRef cmdDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x37, true); // Cmd
    CGEventRef tabDown = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x30, true); // Tab
    CGEventRef tabUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x30, false);
    CGEventRef cmdUp   = CGEventCreateKeyboardEvent(NULL, (CGKeyCode)0x37, false);

    CGEventSetFlags(tabDown, kCGEventFlagMaskCommand);
    CGEventSetFlags(tabUp, kCGEventFlagMaskCommand);

    CGEventPost(kCGHIDEventTap, cmdDown);
    CGEventPost(kCGHIDEventTap, tabDown);
    CGEventPost(kCGHIDEventTap, tabUp);
    CGEventPost(kCGHIDEventTap, cmdUp);
}


// --- Public Function Implementations ---

bool RegisterAppHotKey(int tKey,int tAlter,GenericAppHotKeyCallback callback, void* userData) {
    // Store the generic callback and user data
    gGenericCallback = callback;
    gCallbackUserData = userData; // Store the user data passed by the caller

    EventTypeSpec eventType;
    eventType.eventClass = kEventClassKeyboard;
    eventType.eventKind = kEventHotKeyPressed;

    // Install the *internal* Carbon handler
    OSStatus err = InstallApplicationEventHandler(&InternalCarbonHotKeyHandler, 1, &eventType,
                                                NULL, // Pass NULL here, we use gCallbackUserData
                                                &gAppEventHandlerRef); // Store the handler reference
    if (err != noErr) {
        NSLog(@"Failed to install application event handler, error: %d", err);
        gGenericCallback = nullptr;
        gCallbackUserData = nullptr;
        return false;
    }

    // Register the hotkey (Control+Return)
    // err = RegisterEventHotKey(kVK_Return, controlKey, gAppHotKeyID,
    err = RegisterEventHotKey(tKey, tAlter, gAppHotKeyID,
                              GetApplicationEventTarget(), 0, &gAppHotKeyRef);
    if (err != noErr) {
        NSLog(@"Failed to register hotkey (Ctrl+Return), error: %d", err);
        // Clean up installed handler if registration fails
        if (gAppEventHandlerRef) {
            RemoveEventHandler(gAppEventHandlerRef);
            gAppEventHandlerRef = nullptr;
        }
        gGenericCallback = nullptr;
        gCallbackUserData = nullptr;
        return false;
    }
    NSLog(@"Hotkey registered successfully.");
    return true;
}

void UnregisterAppHotKey() {
    if (gAppHotKeyRef != nullptr) {
        OSStatus err = UnregisterEventHotKey(gAppHotKeyRef);
         if (err != noErr) {
             NSLog(@"Error unregistering hotkey: %d", err);
         } else {
             NSLog(@"Hotkey unregistered.");
         }
        gAppHotKeyRef = nullptr; // Reset the reference
    }
    // Also remove the application event handler
    if (gAppEventHandlerRef) {
        RemoveEventHandler(gAppEventHandlerRef);
        gAppEventHandlerRef = nullptr;
        NSLog(@"Application event handler removed.");
    }
    // Clear stored callback info
    gGenericCallback = nullptr;
    gCallbackUserData = nullptr;
}

void ActivateAppWindow() {
    // NSLog(@"Activating app window.");
    [NSApp activateIgnoringOtherApps:YES];
    // Optional: Bring the main window to the front explicitly if needed
    // [[[[NSApplication sharedApplication] windows] firstObject] makeKeyAndOrderFront:nil];
}

// 將 wxWidgets 的 wxkRETURN 等轉換成 macOS Carbon 的 kVK_* 鍵碼
int Wxk2MacKey(int wxk) {
  switch (wxk) {
    case WXK_RETURN:         return kVK_Return;
    case WXK_ESCAPE:         return kVK_Escape;
    case WXK_BACK:           return kVK_Delete;             // Backspace
    case WXK_TAB:            return kVK_Tab;
    case WXK_SPACE:          return kVK_Space;
    case WXK_DELETE:         return kVK_ForwardDelete;      // Delete
    case WXK_START:          return kVK_Home;
    case WXK_END:            return kVK_End;
    case WXK_HOME:           return kVK_Home;
    case WXK_LEFT:           return kVK_LeftArrow;
    case WXK_UP:             return kVK_UpArrow;
    case WXK_RIGHT:          return kVK_RightArrow;
    case WXK_DOWN:           return kVK_DownArrow;
    case WXK_PAGEUP:         return kVK_PageUp;
    case WXK_PAGEDOWN:       return kVK_PageDown;
    case WXK_F1:             return kVK_F1;
    case WXK_F2:             return kVK_F2;
    case WXK_F3:             return kVK_F3;
    case WXK_F4:             return kVK_F4;
    case WXK_F5:             return kVK_F5;
    case WXK_F6:             return kVK_F6;
    case WXK_F7:             return kVK_F7;
    case WXK_F8:             return kVK_F8;
    case WXK_F9:             return kVK_F9;
    case WXK_F10:            return kVK_F10;
    case WXK_F11:            return kVK_F11;
    case WXK_F12:            return kVK_F12;
    case '0':               return kVK_ANSI_0;
    case '1':               return kVK_ANSI_1;
    case '2':               return kVK_ANSI_2;
    case '3':               return kVK_ANSI_3;
    case '4':               return kVK_ANSI_4;
    case '5':               return kVK_ANSI_5;
    case '6':               return kVK_ANSI_6;
    case '7':               return kVK_ANSI_7;
    case '8':               return kVK_ANSI_8;
    case '9':               return kVK_ANSI_9;
    default:
        if (wxk >= 'A' && wxk <= 'Z') return kVK_ANSI_A + (wxk - 'A');
        if (wxk >= 'a' && wxk <= 'z') return kVK_ANSI_A + (wxk - 'a');
        return -1; // 未對應
  }
}

// 將 macOS Carbon 的 kVK_* 鍵碼轉換成 wxWidgets 的 WXK_* 鍵碼
// return -1 if not mapped
int MacKey2Wxk(int keycode) {
  switch (keycode) {
    case kVK_Return:         return WXK_RETURN;
    case kVK_Escape:         return WXK_ESCAPE;
    case kVK_Delete:         return WXK_BACK;             // Backspace
    case kVK_Tab:            return WXK_TAB;
    case kVK_Space:          return WXK_SPACE;
    case kVK_ForwardDelete:  return WXK_DELETE;           // Delete
    case kVK_Home:           return WXK_HOME;
    case kVK_End:            return WXK_END;
    case kVK_LeftArrow:      return WXK_LEFT;
    case kVK_UpArrow:        return WXK_UP;
    case kVK_RightArrow:     return WXK_RIGHT;
    case kVK_DownArrow:      return WXK_DOWN;
    case kVK_PageUp:         return WXK_PAGEUP;
    case kVK_PageDown:       return WXK_PAGEDOWN;
    case kVK_F1:             return WXK_F1;
    case kVK_F2:             return WXK_F2;
    case kVK_F3:             return WXK_F3;
    case kVK_F4:             return WXK_F4;
    case kVK_F5:             return WXK_F5;
    case kVK_F6:             return WXK_F6;
    case kVK_F7:             return WXK_F7;
    case kVK_F8:             return WXK_F8;
    case kVK_F9:             return WXK_F9;
    case kVK_F10:            return WXK_F10;
    case kVK_F11:            return WXK_F11;
    case kVK_F12:            return WXK_F12;
    case kVK_ANSI_0: return '0';
    case kVK_ANSI_1: return '1';
    case kVK_ANSI_2: return '2';
    case kVK_ANSI_3: return '3';
    case kVK_ANSI_4: return '4';
    case kVK_ANSI_5: return '5';
    case kVK_ANSI_6: return '6';
    case kVK_ANSI_7: return '7';
    case kVK_ANSI_8: return '8';
    case kVK_ANSI_9: return '9';
    default:
      if ((keycode >= kVK_ANSI_A) && (keycode <= kVK_ANSI_Z))
          return 'A' + (keycode - kVK_ANSI_A);
      return -1; // 未對應
  }
}

void HideCurrentApp() {
    // Hide the current application
    NSRunningApplication* currentApp = [NSRunningApplication currentApplication];
    if (currentApp) {
        [currentApp hide];
        // NSLog(@"Current app hidden.");
    } else {
        // NSLog(@"No current app to hide.");
    }
}
void ShowCurrentApp() {
    // Show the current application
    NSRunningApplication* currentApp = [NSRunningApplication currentApplication];
    if (currentApp) {
        [currentApp unhide];
        // NSLog(@"Current app shown.");
    } else {
        // NSLog(@"No current app to show.");
    }
}