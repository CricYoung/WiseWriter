# 建置系統與任務說明（BUILD.md）

本專案支援兩種建置方式：一是使用 Makefile，二是透過 VS Code 的 tasks 系統。

## 🛠️ VS Code 任務系統

VS Code 的 `.vscode/tasks.json` 中定義了多組任務，結合以下組合：

- `A.` 輸出至來源目錄的 Objs/
- `B.` 輸出至工作目錄的 Objs/
- `W / E` 分別代表除錯模式與最佳化模式
- `D / S` 分別代表動態與靜態連結

任務命名如：`A.W.D Build to Src Objs`，表示輸出至原始目錄，除錯模式，動態連結。

## 🧱 Makefile 支援的參數

Makefile 接受以下參數進行任務組合建構：

- `mode=W/E`：除錯 / 最佳化
- `link=D/S`：動態 / 靜態連結
- `out=A/B`：輸出至來源 / 工作目錄

範例：
```bash
make mode=W link=D out=B
```

完整的 build 邏輯集中於 Makefile 中，VS Code 的任務只是傳遞參數的 wrapper。

---
以下為 Makefile 及 tasks.json 範例內容節錄：

### 📄 Makefile 範例
```make
# 編譯器與共用參數
CXX := /usr/bin/clang++

# --- User Provided Variables (from tasks.json) ---
# FILE = path/to/current/file.cpp  (Provided by tasks.json)
# SCOPE = file | dir               (Provided by tasks.json)
# OUT = local | work               (Provided by tasks.json)
# MODE = debug | release           (Provided by tasks.json)
# LINK = none | dynamic | static   (Provided by tasks.json)

# --- Base Flags ---
# 注意：原始的 -D YccInSingleFileTest${fileBasenameNoExtension}=2 可能無法如預期般運作，
# 因為 ${fileBasenameNoExtension} 是 VS Code 變數，不是 make 變數。
# 或許應該是 -D YccInSingleFileTest$(STEM)=2，但此處暫不修改。
BASE_FLAGS = -std=gnu++17 -fobjc-arc -finput-charset=UTF-8 -fexec-charset=UTF-8 -fPIC -Wall -Wunused-parameter -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations -DWXSUPPRESS_SIZER_FLAGS_CHECK -D YccInSingleFileTest${fileBasenameNoExtension}=2 -D_FILE_OFFSET_BITS=64
WX_CFLAGS = -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -mmaco
```

### 📄 tasks.json 範例
```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "AW.Compile Current File (Debug)",
      "type": "shell",
      "command": "make",
      "args": [
        "build",
        "FILE=${file}",
        "SCOPE=file",
        "OUT=local",
        "MODE=debug",
        "LINK=none"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "presentation": {
        "panel": "shared"
      },
      "problemMatcher": [
        "$gcc"
      ],
      "group": {
        "kind": "build",
        "isDefault": false
      },
      "detail": "AW. obj->./Objs (Debug)"
    },
    {
      "label": "AWD.Compile and Link Current File (Debug+Dynamic Link)",
      "type": "shell",
      "command": "make",
      "args": [
        "build",
        "FILE=${file}",
        "SCOPE=file",
        "OUT=local",
        "MODE=debug",
        "LINK=dynamic"
      ],
      "options": {
        "cwd": "${workspaceFolder}"
      },
      "presentation": {
        "panel": "shared"
      },
```