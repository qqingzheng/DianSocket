SUB_PROJECT_NAME := Dian Protocol Dynamic Library

include Makefile.am.libasncodec
TARGET := libdianprotocol.so

INCLUDE_PATH := include/ \
				../socket/include

SRC_DIR := src/
BUILD_DIR := build/

SRC_FILES = $(foreach file,$(ASN_MODULE_SRCS),$(SRC_DIR)/$(file))
OBJS_FILES = $(foreach file,$(ASN_MODULE_SRCS),$(BUILD_DIR)/$(file:.c=.o))

all: show-info $(BUILD_DIR) $(TARGET)
	@g++ -shared $(OBJS_FILES) -o $(TARGET)
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"

$(TARGET): build-sub-project


include ../config/submake.mk