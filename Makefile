#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

#Verbose all!

#Using /bin/sh as predef. shell
SHELL=/bin/sh

#Jasm core daemon
JASM_CORE_DIR=src/main/jasm/core/
JASM_CORE_BINOUT=src/main/jasm/core/jasm

#Jasm Command line interfacing
JASM_CLI_DIR=src/main/interfacing/cli
JASM_CLI_BINOUT=src/main/interfacing/cli/jasmcli

#Jasm GTK+3 graphical interfacing
#JASM_GTK_GUI_DIR=src/main/interfacing/gui
#JASM_GTK_GUI_BINOUT=src/main/interfacing/gui/jasmgtk

#(all) target destination for binaries
JASM_BINOUT_DIR=bin/

JASM_DATA_LOG_OUTPUT=data/log/jasm.log

#Makeflags options (such as j3)
#From cmdline
MAKE_FLAGS=$@

install:
	cd $(JASM_CORE_DIR) && make $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT) ] && mv $(JASM_CORE_BINOUT) bin/ || exit 4
	[ -f $(JASM_CLI_BINOUT) ] && mv $(JASM_CLI_BINOUT) bin/ || exit 4

clean:
	cd $(JASM_CORE_DIR) && make clean $(MAKE_FLAGS) || exit 3
	cd $(JASM_CLI_DIR) && make clean $(MAKE_FLAGS) || exit 3
	[ -f $(JASM_DATA_LOG_OUTPUT) ] && rm -fv $(JASM_DATA_LOG_OUTPUT) || echo ""

cleanbin:
	cd $(JASM_BINOUT_DIR) && rm -fv jasm* || exit 1

.PHONY: install,clean,cleanbin

