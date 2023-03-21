SUB_PROJECT_NAME := Socket Dynamic Library
TARGET := libdiansocket.so

BUILD_DLIB := true # 编译为动态链接库，其实这里不是true也可以～

INCLUDE_PATH := include/
BUILD_PATH := build/
SRC_FILES := $(wildcard src/*.cpp)
OBJS_FILES := $(subst src/, $(BUILD_PATH), $(patsubst %.cpp, %.o, $(SRC_FILES)))
all: $(TARGET)
	@g++ -shared $(OBJS_FILES) -o $(BUILD_PATH)$^
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"
$(TARGET): build-sub-project

.PHONY: clean
clean:
	@rm -rf $(BUILD_PATH)
	@echo "✅ $(SUB_PROJECT_NAME) cleaned"

include ../config/submake.global