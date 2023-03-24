SUB_PROJECT_NAME := Dian Protocol ASN

include Makefile.am.libasncodec
TARGET := libdianprotocol_asn.so

INCLUDE_PATH := include/ \
				../socket/include

SRC_DIR := src/
BUILD_PATH := build/

SRC_FILES = $(foreach file,$(ASN_MODULE_SRCS),$(SRC_DIR)/$(file))
OBJS_FILES = $(foreach file,$(ASN_MODULE_SRCS),$(BUILD_PATH)$(file:.c=.o))

all: build-asn show-info $(BUILD_PATH) $(TARGET)
# @g++ -shared $(OBJS_FILES) -o $(TARGET)
	@echo "\t ✅ $(SUB_PROJECT_NAME) Build Finished"

.PHONY: build-asn
build-asn: 
	@bash build_asn.sh

$(TARGET): build-sub-project


include ../../config/submake.mk