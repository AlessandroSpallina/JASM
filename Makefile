#!/usr/bin/make -f

SHELL=/bin/sh

JASM_CORE_DIR=src/main/jasm/core/
JASM_CORE_BINOUT=src/main/jasm/core/jasm

JASM_CLI_DIR=src/main/gui/cli
JASM_CLI_BINOUT=src/main/gui/cli/jasmcli

JASM_BINOUT_DIR=bin/

MAKE_FLAGS=

install:
	cd $(JASM_CORE_DIR) && make $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT) ] && mv $(JASM_CORE_BINOUT) bin/ || exit 4
	[ -f $(JASM_CLI_BINOUT) ] && mv $(JASM_CLI_BINOUT) bin/ || exit 4

clean:
	cd $(JASM_CORE_DIR) && make clean $(MAKE_FLAGS) || exit 3
	cd $(JASM_CLI_DIR) && make clean $(MAKE_FLAGS) || exit 3

cleanbin:
	cd $(JASM_BINOUT_DIR) && rm -fv * || exit 1

.PHONY: install,clean,cleanbin

