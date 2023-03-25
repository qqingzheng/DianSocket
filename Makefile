# Common Settings
SUB_PROJECT := socket protocol server client


all: show-info build-sub-project
	@echo Build Finished 
	@echo "\033[33m----- Welcome to Dian Intel -----\033[0m"

.PHONY: build-socket
build-socket: socket

.PHONY: build-server
build-server: server	

.PHONY: build-protocol
build-server: protocol

.PHONY: show-info
show-info:
	@echo "\033[33m----- Welcome to Dian -----\033[0m"
	@echo Building Project DianSocket

.PHONY: build-sub-project
build-sub-project: $(SUB_PROJECT)

.PHONY: $(SUB_PROJECT)
$(SUB_PROJECT):
	@make -C $@ -s

.PHONY: clean
clean: 
	@for SUB_PROJ in $(SUB_PROJECT); do\
		make -C $$SUB_PROJ -s clean;\
	done