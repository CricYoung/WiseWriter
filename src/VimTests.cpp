#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <iomanip> // For std::setw

// It's assumed that WiseWriter.h includes everything needed for StVimCmd, EVimCmd, EVimObj
// and MyFrame. MyFrame constructor might need a display if wxApp is not running.
// For testing, we might need to conditionally compile out UI specific parts of MyFrame
// or ensure a wxApp instance exists if wxControl/wxTextCtrl needs it.
// For now, we proceed assuming MyFrame can be instantiated.

// Temporarily make MyFrame members public for testing, or use friend class.
// This is often done by putting test code in a separate build config or using #ifdef for tests.
// For this exercise, we'll assume we can access necessary members for assertion.
// One common way is to make the test functions friends of MyFrame.
// Let's assume MyFrame.h will be modified to make `VimTests` a friend or specific functions.
// (This modification to MyFrame.h is outside current step but important for real testability)
#define private public
#define protected public
#include "WiseWriter.h"
#include "KWxApp.h" // Required for wxAppConsole
#undef private
#undef protected


// Test Suite Runner
static int total_tests = 0;
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_ASSERT(condition) \
    total_tests++; \
    if (condition) { \
        tests_passed++; \
        std::cerr << "."; \
    } else { \
        tests_failed++; \
        std::cerr << "F"; \
        std::cerr << "\nAssertion failed in " << __func__ << " at " << __FILE__ << ":" << __LINE__ << ": " #condition << std::endl; \
    }

#define TEST_ASSERT_EQUAL(expected, actual, message) \
    total_tests++; \
    if (expected == actual) { \
        tests_passed++; \
        std::cerr << "."; \
    } else { \
        tests_failed++; \
        std::cerr << "F"; \
        std::cerr << "\nAssertion failed in " << __func__ << " at " << __FILE__ << ":" << __LINE__ << ": " << message \
                  << " - Expected: " << expected << ", Actual: " << actual << std::endl; \
    }


// Forward declaration for test functions
void TestVimCompiler();
void TestVimExeCmd(); // Will be challenging due to wxTextCtrl

// Global wxApp object for tests that might need it for wxString, etc.
// wxAppConsole* testApp = nullptr; // wxApp should be created only once.

// --- TestVimCompiler ---
void TestVimCompiler() {
    std::cerr << "\nRunning TestVimCompiler..." << std::endl;
    MyFrame frame; // MyFrame constructor creates a wxTextCtrl. This might need a valid wxApp.
    StVimCmd cmd;
    std::string cmdStr;

    // Case 1: "2dw"
    cmdStr = "2dw";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(2, cmd.count, "2dw count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "2dw action");
    TEST_ASSERT_EQUAL(evoWord, cmd.object, "2dw object");

    // Case 2: "y$"
    cmdStr = "y$";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "y$ count");
    TEST_ASSERT_EQUAL(evcYield, cmd.action, "y$ action");
    TEST_ASSERT_EQUAL(evoToLineEnd, cmd.object, "y$ object");

    // Case 3: "rX"
    cmdStr = "rX";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "rX count");
    TEST_ASSERT_EQUAL(evcReplaceChar, cmd.action, "rX action");
    TEST_ASSERT_EQUAL('X', cmd.param, "rX param");
    TEST_ASSERT_EQUAL(evoUndefined, cmd.object, "rX object");


    // Case 4: "dd"
    cmdStr = "dd";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "dd count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "dd action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "dd object");
    
    // Case 5: "2dd"
    cmdStr = "2dd";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(2, cmd.count, "2dd count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "2dd action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "2dd object");

    // Case 6: "cc"
    cmdStr = "cc";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "cc count");
    TEST_ASSERT_EQUAL(evcChange, cmd.action, "cc action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "cc object");

    // Case 7: "fx" (find x)
    cmdStr = "fx";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "fx count");
    TEST_ASSERT_EQUAL(evcFind, cmd.action, "fx action");
    TEST_ASSERT_EQUAL(evoFindCharForward, cmd.object, "fx object");
    TEST_ASSERT_EQUAL('x', cmd.param, "fx param");
    
    // Case 8: "3fx" (find 3rd x)
    cmdStr = "3fx";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(3, cmd.count, "3fx count");
    TEST_ASSERT_EQUAL(evcFind, cmd.action, "3fx action");
    TEST_ASSERT_EQUAL(evoFindCharForward, cmd.object, "3fx object");
    TEST_ASSERT_EQUAL('x', cmd.param, "3fx param");

    // Case 9: "s" (substitute char)
    cmdStr = "s";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "s count");
    TEST_ASSERT_EQUAL(evcSubstitute, cmd.action, "s action");
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "s object");

    // Case 10: "3s" (substitute 3 chars)
    cmdStr = "3s";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(3, cmd.count, "3s count");
    TEST_ASSERT_EQUAL(evcSubstitute, cmd.action, "3s action");
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "3s object");

    // Case 11: "S" (substitute line)
    cmdStr = "S";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "S count");
    TEST_ASSERT_EQUAL(evcSubstitute, cmd.action, "S action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "S object");
    
    // Case 12: "2S" (substitute 2 lines)
    cmdStr = "2S";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(2, cmd.count, "2S count");
    TEST_ASSERT_EQUAL(evcSubstitute, cmd.action, "2S action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "2S object");

    // Case 13: "x" (delete character)
    cmdStr = "x";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "x count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "x action");
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "x object");

    // Case 14: "3x" (delete 3 characters)
    cmdStr = "3x";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(3, cmd.count, "3x count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "3x action");
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "3x object");

    // Case 15: "dw" (delete word, count 1 implied)
    cmdStr = "dw";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "dw count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "dw action");
    TEST_ASSERT_EQUAL(evoWord, cmd.object, "dw object");
    
    // Case 16: "0" (move to beginning of line - as a command itself if not part of other)
    // Note: The current VimCompiler might not produce evcMove for "0" alone.
    // It parses '0' as part of count, or as an object for d,y,c.
    // Standalone motions like "0", "w", "$" are typically handled by VimExeCmd with evcMove.
    // Let's test "d0"
    cmdStr = "d0";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "d0 count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "d0 action");
    TEST_ASSERT_EQUAL(evoToLineStart0, cmd.object, "d0 object");

    // Case 17: "Y" (yank line)
    cmdStr = "Y";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "Y count");
    TEST_ASSERT_EQUAL(evcYield, cmd.action, "Y action");
    TEST_ASSERT_EQUAL(evoLine, cmd.object, "Y object");

    // Case 18: "D" (delete to end of line)
    cmdStr = "D";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "D count");
    TEST_ASSERT_EQUAL(evcDelete, cmd.action, "D action");
    TEST_ASSERT_EQUAL(evoToLineEnd, cmd.object, "D object");

    // Case 19: "C" (change to end of line)
    cmdStr = "C";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(1, cmd.count, "C count");
    TEST_ASSERT_EQUAL(evcChange, cmd.action, "C action");
    TEST_ASSERT_EQUAL(evoToLineEnd, cmd.object, "C object");

    // Case 20: "3l" (move 3 chars right)
    cmdStr = "3l";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(3, cmd.count, "3l count");
    TEST_ASSERT_EQUAL(evcMove, cmd.action, "3l action"); // Assuming 'l' is compiled to a move action
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "3l object");
    TEST_ASSERT_EQUAL(1, cmd.param, "3l param (direction)");


    // Case 21: "2h" (move 2 chars left)
    cmdStr = "2h";
    frame.VimCompiler(cmdStr, cmd);
    TEST_ASSERT_EQUAL(2, cmd.count, "2h count");
    TEST_ASSERT_EQUAL(evcMove, cmd.action, "2h action"); // Assuming 'h' is compiled to a move action
    TEST_ASSERT_EQUAL(evoCharacter, cmd.object, "2h object");
    TEST_ASSERT_EQUAL(-1, cmd.param, "2h param (direction)");

    std::cerr << std::endl;
}

// --- TestVimExeCmd ---
// This function will be more challenging due to wxTextCtrl dependency.
// We'll try to simulate its behavior.
void TestVimExeCmd() {
    std::cerr << "\nRunning TestVimExeCmd..." << std::endl;
    MyFrame frame; // This creates the frame and the 'input' wxTextCtrl
    StVimCmd cmd;

    // Preparatory: ensure input is not null
    if (!frame.input) {
        std::cerr << "FATAL: frame.input is null. wxTextCtrl could not be created." << std::endl;
        std::cerr << "This might be due to missing wxApp instance or other UI setup issues." << std::endl;
        // total_tests++; tests_failed++; // Mark as failure
        // For now, we'll let it try and potentially crash to see wxWidgets error messages.
        // A proper test setup would mock wxTextCtrl or ensure wxApp is running.
    }
    TEST_ASSERT(frame.input != nullptr);
    if (!frame.input) return; // Stop if no input control


    // --- dw on "hello world" ---
    frame.input->SetValue("hello world");
    frame.input->SetInsertionPoint(0);
    cmd.count = 1; cmd.action = evcDelete; cmd.object = evoWord; cmd.param = 0;
    frame.VimExeCmd(cmd);
    // Depending on GetCurPhrasePos, "hello " might be deleted or just "hello".
    // Assuming GetCurPhrasePos selects "hello " (word + trailing space if any)
    // Current GetCurPhrasePos seems to select word then spaces after it.
    // If cursor at 'h', phrase is "hello ". Remove("hello ") -> "world"
    TEST_ASSERT_EQUAL(std::string("world"), frame.input->GetValue().ToStdString(), "dw on 'hello world'");
    TEST_ASSERT_EQUAL(0, frame.input->GetInsertionPoint(), "dw cursor check");


    // --- dd on "line1\nline2\nline3" (cursor on line1) ---
    frame.input->SetValue("line1\nline2\nline3");
    frame.input->SetInsertionPoint(0); // Cursor at start of line1
    cmd.count = 1; cmd.action = evcDelete; cmd.object = evoLine; cmd.param = 0;
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("line2\nline3"), frame.input->GetValue().ToStdString(), "dd on 3 lines (1)");
    TEST_ASSERT_EQUAL(0, frame.input->GetInsertionPoint(), "dd cursor check (1)");


    // --- 2dd on "line1\nline2\nline3" (cursor on line1) ---
    frame.input->SetValue("line1\nline2\nline3");
    frame.input->SetInsertionPoint(0); // Cursor at start of line1
    cmd.count = 2; cmd.action = evcDelete; cmd.object = evoLine; cmd.param = 0;
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("line3"), frame.input->GetValue().ToStdString(), "2dd on 3 lines");
    TEST_ASSERT_EQUAL(0, frame.input->GetInsertionPoint(), "2dd cursor check");
    

    // --- Yank and Put ---
    // Test yl (yank char)
    frame.input->SetValue("testing YL");
    frame.input->SetInsertionPoint(0); // Cursor on 't'
    cmd.count = 1; cmd.action = evcYield; cmd.object = evoCharacter; cmd.param = 0; // yl (param for l/h not used by GetTextForMotion here)
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("t"), frame.m_vimYankBuffer, "yl yank char");
    TEST_ASSERT_EQUAL(0, frame.input->GetInsertionPoint(), "yl cursor after yank"); // Cursor shouldn't move for yank

    // Test p (put char after cursor)
    frame.input->SetInsertionPoint(6); // Cursor on 'g' of "testing YL"
    cmd.count = 1; cmd.action = evcPut; cmd.object = evoUndefined; cmd.param = 0;
    frame.VimExeCmd(cmd); // Should become "testing tYL" (depends on put logic: after cursor)
    TEST_ASSERT_EQUAL(std::string("testing t YL"), frame.input->GetValue().ToStdString(), "p put char");
    // Cursor position after put needs to be verified based on precise put logic.
    // Current put logic for charwise: input->SetInsertionPoint(currentPos + 1); then WriteText.
    // So if cursor was on 'g' (pos 6), it becomes pos 7, then 't' inserted. "testing tYL". Cursor after 't'. (pos 8)
    // TEST_ASSERT_EQUAL(8, frame.input->GetInsertionPoint(), "p cursor after put char"); Let's check text only.

    // Test yy (yank line)
    frame.input->SetValue("lineA\nlineB");
    frame.input->SetInsertionPoint(0); // Cursor on lineA
    cmd.count = 1; cmd.action = evcYield; cmd.object = evoLine; cmd.param = 0;
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("lineA\n"), frame.m_vimYankBuffer, "yy yank line"); // Yank includes newline
    TEST_ASSERT_EQUAL(0, frame.input->GetInsertionPoint(), "yy cursor after yank");

    // Test P (put line after current line)
    frame.input->SetInsertionPoint(frame.input->GetValue().find('B')); // Cursor on lineB
    cmd.count = 1; cmd.action = evcPut; cmd.object = evoUndefined; cmd.param = 0;
    frame.VimExeCmd(cmd); // Should become "lineA\nlineA\nlineB" (put adds \n before pasted if linewise)
                          // Current put logic: MoveToLineBottom, then \n, then paste.
                          // If on LineB, EOL, then \n, then "lineA\n". Result: lineA\nlineB\nlineA\n
    TEST_ASSERT_EQUAL(std::string("lineA\nlineB\nlineA\n"), frame.input->GetValue().ToStdString(), "P put line");


    // --- Test 'x' (delete character) ---
    frame.input->SetValue("abcde");
    frame.input->SetInsertionPoint(1); // Cursor on 'b'
    cmd.count = 1; cmd.action = evcDelete; cmd.object = evoCharacter; cmd.param = 0;
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("acde"), frame.input->GetValue().ToStdString(), "x delete char");
    TEST_ASSERT_EQUAL(1, frame.input->GetInsertionPoint(), "x cursor after delete");

    // --- Test '3x' (delete 3 characters) ---
    frame.input->SetValue("abcde");
    frame.input->SetInsertionPoint(1); // Cursor on 'b'
    cmd.count = 3; cmd.action = evcDelete; cmd.object = evoCharacter; cmd.param = 0;
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("ae"), frame.input->GetValue().ToStdString(), "3x delete 3 chars");
    TEST_ASSERT_EQUAL(1, frame.input->GetInsertionPoint(), "3x cursor after delete");
    
    // --- Test 'rX' (replace char) ---
    frame.input->SetValue("apple");
    frame.input->SetInsertionPoint(2); // Cursor on 'p'
    cmd.count = 1; cmd.action = evcReplaceChar; cmd.object = evoUndefined; cmd.param = 'X';
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("apXle"), frame.input->GetValue().ToStdString(), "rX replace char");
    TEST_ASSERT_EQUAL(2, frame.input->GetInsertionPoint(), "rX cursor"); // should be on the replaced char

    // --- Test '3rX' (replace 3 chars with X) ---
    frame.input->SetValue("applepie");
    frame.input->SetInsertionPoint(1); // Cursor on 'p'
    cmd.count = 3; cmd.action = evcReplaceChar; cmd.object = evoUndefined; cmd.param = 'X';
    frame.VimExeCmd(cmd);
    TEST_ASSERT_EQUAL(std::string("aXXXepie"), frame.input->GetValue().ToStdString(), "3rX replace 3 chars");
    TEST_ASSERT_EQUAL(1+3-1, frame.input->GetInsertionPoint(), "3rX cursor");


    // --- Test 's' (substitute char) ---
    frame.input->SetValue("banana");
    frame.input->SetInsertionPoint(1); // Cursor on 'a'
    cmd.count = 1; cmd.action = evcSubstitute; cmd.object = evoCharacter; cmd.param = 0;
    frame.VimExeCmd(cmd); // Deletes 'a', enters insert mode. State of text after this is "bnana", cursor at 1.
    TEST_ASSERT_EQUAL(std::string("bnana"), frame.input->GetValue().ToStdString(), "s substitute char (text part)");
    TEST_ASSERT_EQUAL(1, frame.input->GetInsertionPoint(), "s cursor after delete");
    TEST_ASSERT_EQUAL(false, frame.bInViewMode, "s should enter insert mode");
    frame.SetViewMod(true); // Reset for next test

    // --- Test 'S' (substitute line) ---
    frame.input->SetValue("first line\nsecond line\nthird line");
    frame.input->SetInsertionPoint(frame.input->GetValue().find("second")); // Cursor on "second line"
    cmd.count = 1; cmd.action = evcSubstitute; cmd.object = evoLine; cmd.param = 0;
    frame.VimExeCmd(cmd); // Deletes "second line\n", inserts a newline, enters insert mode.
    // Expected: "first line\n\nthird line", cursor on the blank line.
    TEST_ASSERT_EQUAL(std::string("first line\n\nthird line"), frame.input->GetValue().ToStdString(), "S substitute line (text part)");
    TEST_ASSERT_EQUAL(std::string("first line\n").length(), frame.input->GetInsertionPoint(), "S cursor after delete/insert");
    TEST_ASSERT_EQUAL(false, frame.bInViewMode, "S should enter insert mode");
    frame.SetViewMod(true); // Reset

    std::cerr << std::endl;
}


int main(int argc, char **argv) {
    // For tests involving wxWidgets classes like wxString or wxTextCtrl,
    // a wxAppConsole instance is often needed to initialize the library.
    // wxApp::SetInstance( new wxAppConsole ); // This is one way
    // wxEntryStart( argc, argv );             // Initializes wxWidgets
    // testApp = new wxAppConsole();
    // wxApp::SetInstance(testApp);
    // wxDISABLE_DEBUG_SUPPORT(); // Optional: Supress wx debug dialogs if any pop up
    
    // A more robust way for GUI tests is to have a proper wxApp.
    // For console tests, wxAppConsole is minimal.
    // The MyFrame constructor itself creates a wxFrame and wxTextCtrl. This is problematic
    // without a proper wxWidgets event loop and wxApp instance.
    // The #defines for private/protected are a hack. Friend class is better.
    
    // Hack to try and get wxEntry to work for basic wxString etc.
    // This is not a full GUI app init.
    if (!wxEntryStart(argc, argv)) {
        std::cerr << "wxEntryStart failed. Some tests might not work correctly." << std::endl;
    }


    TestVimCompiler();
    TestVimExeCmd(); // This will likely have issues if wxTextCtrl is not properly usable

    std::cerr << "\n--- Test Summary ---" << std::endl;
    std::cerr << "Total Tests: " << total_tests << std::endl;
    std::cerr << "Passed: " << tests_passed << std::endl;
    std::cerr << "Failed: " << tests_failed << std::endl;
    std::cerr << "--------------------" << std::endl;

    if (wxTheApp) { // wxTheApp is a global pointer to the wxApp instance
        wxTheApp->OnExit(); // Clean up wxWidgets
        wxEntryCleanup();
    }
    
    return (tests_failed == 0) ? 0 : 1;
}

// To compile manually (assuming wxWidgets is installed and configured for g++):
// g++ -std=c++11 VimTests.cpp WiseWriter.cpp KGlobal.cpp KHotKey.cpp LinkOS.mm KWxColor.cpp DlgChoseLang.cpp DlgSettings.cpp Dlgs.cpp KHelp.cpp -o VimTests `wx-config --cppflags --libs base,core,osx_cocoa` -framework AppKit -ObjC++
// Note: LinkOS.mm and KWxApp.cpp might be needed depending on MyFrame dependencies.
// The list of cpp files might grow.
// The order of linking libraries and files can matter.
// The osx_cocoa might be different on other systems (e.g. gtk3).
// For a simple test, might try to compile only VimTests.cpp and WiseWriter.cpp if dependencies allow.
// g++ -std=c++11 VimTests.cpp WiseWriter.cpp KGlobal.cpp KHotKey.cpp KWxColor.cpp KHelp.cpp -o VimTests `wx-config --cppflags --libs base,core`
// This assumes MyFrame and its methods don't pull in too many UI specific parts from other files.
// The `private public` hack is used to allow test access to MyFrame members like `input`.
// A better solution is friend class or specific accessor methods for testing.
// `wx-config --libs` includes UI libs by default usually. `base,core` are more minimal.
// If `MyFrame` constructor fails due to UI, these tests for VimExeCmd will largely fail.
// VimCompiler tests should be fine as they don't directly use UI elements.
