SUB_PROJECT_NAME := Dian Protocol Dynamic Library
TARGET := libdianprotocol.so

BUILD_DLIB := # 编译为动态链接库

INCLUDE_PATH := include/
BUILD_PATH := build/

SRC_FILES := $(wildcard src/*.cpp)
OBJS_FILES := $(SRC_FILES:.cpp=.o)
OBJS_ASN :=  $(wildcard dian_protocol_asn/build/*.o)


all: build-asn $(TARGET)
	@g++ -shared $(OBJS_FILES) $(OBJS_ASN) -o $(TARGET)
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"
$(TARGET): build-sub-project

.PHONY: build-asn
build-asn:
	@make -C dian_protocol_asn -s

include ../config/submake.mk