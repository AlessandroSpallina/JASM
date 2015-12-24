#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g -DDEBUG
LIBS=
BUILD_DATE_CLI="$(shell LANG="en_US.UTF-8" date "+%a %d/%m/%Y %H:%M")"
CFLAGS=-O2 -pipe -Wall -std=c11 -pthread -D_POSIX_C_SOURCE -DBUILD_DATE_CLI=\"$(BUILD_DATE_CLI)\" $(LIBS)
CFLAGS_DEBUG=-Wall -std=c11 -pthread -D_POSIX_C_SOURCE -DBUILD_DATE_CLI=\"$(BUILD_DATE_CLI)\" $(DEBUG) $(LIBS)
BINOUT=jasmcli
BINOUT_DEBUG=jasmcli-debug

####    SOURCES & RULES    ####

debug: CFLAGS=$(CFLAGS_DEBUG)

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT) $(OBJS)

debug: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT_DEBUG) $(OBJS)

clean:
	rm -fv *.o

#make * (opt) means all
.PHONY: install,clean,debug
