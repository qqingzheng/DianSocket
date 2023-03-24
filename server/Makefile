SUB_PROJECT_NAME := Dian Socket Server
TARGET = server

DLIBS := ../socket/libdiansocket.so\
		 ../dian_protocol/libdianprotocol.so
INCLUDE_PATH := include/ \
				../socket/include
LDFLAGS := -lsctp
BUILD_PATH := build/
SRC_FILES := $(wildcard src/*.cpp)
OBJS_FILES := $(subst src/, $(BUILD_PATH), $(patsubst %.cpp, %.o, $(SRC_FILES)))

all: $(TARGET)
	@g++ $(OBJS_FILES) $(DLIBS) -o $^ $(LDFLAGS)
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"

$(TARGET): build-sub-project

include ../config/submake.mk