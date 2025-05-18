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
WX_CFLAGS = -isysroot /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk -mmacosx-version-min=11.00 -I /Users/cric/wxWidgets/MacDyn/lib/wx/include/osx_cocoa-unicode-3.2 -I /Users/cric/wxWidgets/include -DwxDEBUG_LEVEL=0 -D__WXOSX_COCOA__  -fno-common
WX_LDFLAGS = -framework IOKit -framework Carbon -framework Cocoa -framework QuartzCore -framework AudioToolbox \
  -framework System -framework OpenGL -framework WebKit \
  -lwx_osx_cocoau_html-3.2 -lwx_baseu_xml-3.2 -lexpat \
  -lwx_osx_cocoau_core-3.2 -lwx_baseu-3.2 -lwxtiff-3.2 -lwx_osx_cocoau_webview-3.2 \
  -lwxjpeg-3.2 -lwxpng-3.2 -lwxregexu-3.2 -lwxscintilla-3.2 -lwx_osx_cocoau_stc-3.2 -lz \
  -framework Security -lpthread -liconv

# --- Mode Dependent Flags ---
ifeq ($(MODE),release)
CFLAGS := -O2 -DNDEBUG
else
CFLAGS := -g -DDEBUG
endif
CFLAGS += $(BASE_FLAGS) $(WX_CFLAGS)
CFLAGS += -MMD -MP # <--- 加入此行以產生相依性檔案

# --- Linker Dependent Flags ---
ifeq ($(LINK),static)
LDFLAGS := $(WX_LDFLAGS) -L/Users/cric/wxWidgets/Mac/lib 
else ifeq ($(LINK),dynamic)
LDFLAGS := $(WX_LDFLAGS) -L/Users/cric/wxWidgets/MacDyn/lib
else
LDFLAGS :=
endif

# --- Determine Paths and Files ---
# 確保 FILE 變數已設定 (由 tasks.json 提供)
ifndef FILE
	$(error FILE variable is not set. Please provide it, e.g., make build FILE=src/main.cpp)
endif

# 從 FILE 變數推導路徑和檔名
SRC_DIR := $(shell dirname "$(FILE)")
BASE_NAME := $(shell basename "$(FILE)")
#STEM := $(basename $(BASE_NAME)) # 檔案主檔名 (無副檔名)
STEM := WiseWriter # 檔案主檔名 (無副檔名)

# 決定 Object 檔案的輸出目錄
ifeq ($(OUT),local)
OBJ_DIR := $(SRC_DIR)/Objs
else
OBJ_DIR := Objs
endif

# 決定最終執行檔的名稱 (放在執行 make 的當前目錄)
TARGET := $(STEM)

# 根據 SCOPE 決定來源檔案
ifeq ($(SCOPE),file)
SOURCES := $(FILE)
else
# 找出來源目錄下的所有 .cpp 和 .mm 檔案
SOURCES := $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(SRC_DIR)/*.mm)
endif

# 從來源檔案推導 Object 檔案列表
# 1. 取得來源檔案的基本名稱 (不含路徑)
# 2. 將 .cpp 或 .mm 副檔名替換為 .o
# 3. 在前面加上 Object 目錄路徑
OBJECTS := $(addprefix $(OBJ_DIR)/, $(patsubst %.cpp,%.o,$(patsubst %.mm,%.o,$(notdir $(SOURCES)))))

# --- Targets ---
.PHONY: build clean all

# 預設目標
all: build

# Build 目標:
# 如果需要連結 (LINK != none)，則依賴最終的執行檔 $(TARGET)
# 如果不需要連結 (LINK == none)，則僅依賴 Object 檔案 $(OBJECTS)
ifeq ($(LINK),none)
build: $(OBJECTS)
	@echo "編譯完成 (未連結)."
else
build: $(TARGET)
	@echo "建置完成: $(TARGET)"
endif

# 連結規則: 從 Object 檔案建立最終的執行檔
# 只有當 build 目標依賴 $(TARGET) 時 (即 LINK != none)，此規則才會被觸發
$(TARGET): $(OBJECTS)
	@echo "正在連結 $@ 從 $(filter %.o,$^)..."
	$(CXX) $(filter %.o,$^) $(LDFLAGS) -o $@

# 確保 Object 目錄存在 (Order-only prerequisite)
# 這表示 $(OBJECTS) 的規則會在 $(OBJ_DIR) 規則執行後才執行，但 $(OBJ_DIR) 的時間戳不影響 $(OBJECTS) 是否需要重新生成
$(OBJECTS): | $(OBJ_DIR)
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

# 編譯規則 for .cpp files
# $< 代表第一個依賴項目 (來源檔案)
# $@ 代表目標檔案 (Object 檔案)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "正在編譯 $< -> $@..."
	$(CXX) $(CFLAGS) -c $< -o $@ -MF $(patsubst %.o,%.d,$@)

# 編譯規則 for .mm files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.mm
	@echo "正在編譯 $< -> $@..."
	$(CXX) $(CFLAGS) -c $< -o $@ -MF $(patsubst %.o,%.d,$@)

# 清理目標: 刪除 Object 檔案和最終執行檔
clean:
	@echo "正在清理 Object 檔案和目標執行檔..."
	rm -rf $(OBJ_DIR) $(TARGET)

# 包含由編譯器產生的相依性檔案
# 將 OBJECTS 列表中的 .o 替換為 .d
DEPFILES := $(OBJECTS:.o=.d)
-include $(DEPFILES)
