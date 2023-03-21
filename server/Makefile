SUB_PROJECT_NAME := Dian Socket Server
TARGET = server

DIAN_SOCKET_DLIB := ../socket/build/libdiansocket.so
INCLUDE_PATH := include/ \
				../socket/include
BUILD_PATH := build/
SRC_FILES := $(wildcard src/*.cpp)
OBJS_FILES := $(subst src/, $(BUILD_PATH), $(patsubst %.cpp, %.o, $(SRC_FILES)))
all: $(TARGET)
	@g++ $(OBJS_FILES) $(DIAN_SOCKET_DLIB) -o $(BUILD_PATH)$^
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"

$(TARGET): build-sub-project

.PHONY: clean
clean:
	@rm -rf $(BUILD_PATH)
	@echo "✅ $(SUB_PROJECT_NAME) cleaned"

include ../config/submake.global