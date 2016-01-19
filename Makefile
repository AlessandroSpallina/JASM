#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

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

#Jasm Shared Objects
JASM_LIB_SOUT=src/main/jasm/core/libjasm.so
JASM_SOUT_DIR=lib/
JASM_SO_NM=libjasm.so

#(all) target destination for binaries
JASM_BINOUT_DIR=bin/
JASM_BINOUT_DIR_DEBUG=bin-debug/

#Data log
JASM_DATA_PATTERN=data/jasm*

#Makeflags options (such as j3)
#From cmdline
MAKE_FLAGS=$@

install:
	cd $(JASM_CORE_DIR) && make $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT) ] && mv $(JASM_CORE_BINOUT) $(JASM_BINOUT_DIR) || exit 4
	[ -f $(JASM_CLI_BINOUT) ] && mv $(JASM_CLI_BINOUT) $(JASM_BINOUT_DIR) || exit 4
	[ -f $(JASM_LIB_SOUT) ] && mv $(JASM_LIB_SOUT) $(JASM_SOUT_DIR) || exit 4

debug:
	cd $(JASM_CORE_DIR) && make debug $(MAKE_FLAGS) || exit 2
	cd $(JASM_CLI_DIR) && make debug $(MAKE_FLAGS) || exit 2
	[ -f $(JASM_CORE_BINOUT_DEBUG) ] && mv $(JASM_CORE_BINOUT_DEBUG) $(JASM_BINOUT_DIR_DEBUG) || exit 4
	[ -f $(JASM_CLI_BINOUT_DEBUG) ] && mv $(JASM_CLI_BINOUT_DEBUG) $(JASM_BINOUT_DIR_DEBUG) || exit 4
	[ -f $(JASM_LIB_SOUT) ] && mv $(JASM_LIB_SOUT) $(JASM_SOUT_DIR) || exit 4

clean:
	cd $(JASM_CORE_DIR) && make clean $(MAKE_FLAGS) || exit 3
	cd $(JASM_CLI_DIR) && make clean $(MAKE_FLAGS) || exit 3
	rm -rfv $(JASM_DATA_PATTERN) || echo ""
	cd $(JASM_SOUT_DIR) && rm -fv $(JASM_SO_NM) || echo ""

cleanbin:
	cd $(JASM_BINOUT_DIR) && rm -fv jasm* || exit 1
	cd $(JASM_BINOUT_DIR_DEBUG) && rm -fv *-debug || exit 1

.PHONY: install,debug,clean,cleanbin
