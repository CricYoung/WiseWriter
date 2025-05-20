
#pragma once

#include <string>

// This module is for macOS-specific functionality.

// Don't include CoreServices here directly if possible,
// keep platform specifics in LinkOS.mm

// Forward declare opaque types if absolutely necessary, but prefer void*
// struct EventHotKeyRef__; typedef struct EventHotKeyRef__* EventHotKeyRef;
// struct OpaqueEventRef; typedef struct OpaqueEventRef* EventRef;
// struct OpaqueEventHandlerCallRef; typedef struct OpaqueEventHandlerCallRef* EventHandlerCallRef;
// typedef signed int OSStatus; // Be careful with this


// Existing functions
bool CheckIfOsTrusted();
void RememberFrontApp();
void RestoreFocusToPreviousApp();
void SetClipboardText (const std::string& text);
void SendPasteGemini();
void SendPaste();
void SendCmdTab();

// New functions for Objective-C parts

// Define a generic C-style callback function pointer type
typedef void (*GenericAppHotKeyCallback)(void *userData);

// Registers the global hotkey. Needs the generic C callback and user data.
// Returns true on success, false on failure.
enum EOptionKey{
  eCtrl=0x1,
  eCmd=0x2,
  eShift=0x4,
  eAlt=0x8,
};

int StKeyModifier2CarbonModifier(int tAlter);
bool RegisterAppHotKey(int tKey,int tAlter,GenericAppHotKeyCallback callback, void* userData);

// Unregisters the global hotkey.
void UnregisterAppHotKey();

// Activates the application window (can be called from the generic callback).
void ActivateAppWindow();


int Wxk2MacKey(int wxk); 
int MacKey2Wxk(int keycode);

void HideCurrentApp();
void ShowCurrentApp();