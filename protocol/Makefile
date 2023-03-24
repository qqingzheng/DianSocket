SUB_PROJECT_NAME := Socket Dynamic Library
TARGET := libdianprotocol.so

BUILD_DLIB := # 编译为动态链接库

INCLUDE_PATH := include/
BUILD_PATH := build/

SRC_FILES := $(wildcard src/*.cpp)
OBJS_FILES := $(SRC_FILES:.cpp=.o)
OBJS_ASN :=  $(wildcard dian_protocol_asn/build/*.o)

all: $(TARGET)
	@g++ -shared $(OBJS_FILES) $(OBJS_ASN) -o $^
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"
$(TARGET): build-sub-project

include ../config/submake.mk