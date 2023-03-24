CXX := g++
CC := gcc

FLAGS := -g\
		 -DDEBUG=1
FLAGS += $(addprefix -I, $(INCLUDE_PATH))

CCFLAGS := $(FLAGS)
CXXFLAGS := $(FLAGS)
OBJCCFLAGS := $(CCFLAGS)\
			 -DASN_PDU_COLLECTION\
			 -c
OBJCXXFLAGS := $(CXXFLAGS)\
			 -c

ifdef BUILD_DLIB # 是否需要编译为动态链接库
OBJCXXFLAGS += -fPIC
endif

ifdef ASN_MODULE_CFLAGS # 是否需要编译为动态链接库
OBJCCFLAGS += ${ASN_MODULE_CFLAGS} -DASN_PDU_COLLECTION -w -fPIC
endif

vpath %.cpp src
vpath %.c src

.PHONY: build-sub-project
build-sub-project: show-info mkdirs $(OBJS_FILES)

.PHONY: show-info
show-info:
	@echo 🐢 Building $(SUB_PROJECT_NAME)...

.PHONY: mkdirs
mkdirs: $(BUILD_PATH)
$(BUILD_PATH):
	@mkdir -p $@

.PHONY: clean
clean:
	@rm -rf $(BUILD_PATH)
	@echo "✅ $(SUB_PROJECT_NAME) cleaned"

$(BUILD_PATH)%.o: %.cpp
	@$(CXX) $(OBJCXXFLAGS) $< -o $@

$(BUILD_PATH)%.o: %.c
	@$(CC) $(OBJCCFLAGS) $< -o $@