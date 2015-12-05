#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

#
# NEED TO ADD: debugbuild
#

#Verbose all!

#Using /bin/sh as predef. shell
SHELL=/bin/sh

#Jasm core daemon
JASM_CORE_DIR=src/main/jasm/core/
JASM_CORE_BINOUT=src/main/jasm/core/jasm
JASM_CORE_BINOUT_DEBUG=src/main/jasm/core/jasm-debug

#Jasm Command line interfacing
JASM_CLI_DIR=src/main/interfacing/cli
JASM_CLI_BINOUT=src/main/interfacing/cli/jasmcli
JASM_CLI_BINOUT_DEBUG=src/main/interfacing/cli/jasmcli-debug

#Jasm GTK+3 graphical interfacing
JASM_GTK_GUI_DIR=src/main/interfacing/gui
JASM_GTK_GUI_BINOUT=src/main/interfacing/gui/jasmgtk
JASM_GTK_GUI_BINOUT_DEBUG=src/main/interfacing/gui/jasmgtk-debug

#(all) target destination for binaries
JASM_BINOUT_DIR=bin/
JASM_BINOUT_DIR_DEBUG=bin-debug/

#Data log
TMP_JASM_DATA_LOG_OUTPUT=data/log/jasm.log

#Makeflags options (such as j3)
#From cmdline
MAKE_FLAGS=$@

install:
	cd $(JASM_CORE_DIR) && make $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make $(MAKE_FLAGS) || exit 2
	cd $(JASM_GTK_GUI_DIR) && make $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT) ] && mv $(JASM_CORE_BINOUT) $(JASM_BINOUT_DIR) || exit 4
	[ -f $(JASM_CLI_BINOUT) ] && mv $(JASM_CLI_BINOUT) $(JASM_BINOUT_DIR) || exit 4
	[ -f $(JASM_GTK_GUI_BINOUT) ] && mv $(JASM_GTK_GUI_BINOUT) $(JASM_BINOUT_DIR) || exit 4

debug:
	cd $(JASM_CORE_DIR) && make debug $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make debug $(MAKE_FLAGS) || exit 2
	cd $(JASM_GTK_GUI_DIR) && make debug $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT_DEBUG) ] && mv $(JASM_CORE_BINOUT_DEBUG) $(JASM_BINOUT_DIR_DEBUG) || exit 4
	[ -f $(JASM_CLI_BINOUT_DEBUG) ] && mv $(JASM_CLI_BINOUT_DEBUG) $(JASM_BINOUT_DIR_DEBUG) || exit 4
	[ -f $(JASM_GTK_GUI_BINOUT_DEBUG) ] && mv $(JASM_GTK_GUI_BINOUT_DEBUG) $(JASM_BINOUT_DIR_DEBUG) || exit 4

clean:
	cd $(JASM_CORE_DIR) && make clean $(MAKE_FLAGS) || exit 3
	cd $(JASM_CLI_DIR) && make clean $(MAKE_FLAGS) || exit 3
	cd $(JASM_GTK_GUI_DIR) && make clean $(MAKE_FLAGS) || exit 3
	[ -f $(JASM_DATA_LOG_OUTPUT) ] && rm -fv $(TMP_JASM_DATA_LOG_OUTPUT) || echo ""

cleanbin:
	cd $(JASM_BINOUT_DIR) && rm -fv jasm* || exit 1
	cd $(JASM_BINOUT_DIR_DEBUG) && rm -fv *-debug || exit 1

.PHONY: install,debug,clean,cleanbin
