#!/usr/bin/make -f

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g
LIBS=
CFLAGS=-O2 -pipe -Wall -std=c11 -D_POSIX_C_SOURCE $(LIBS)
CFLAGS_DEBUG=-Wall -std=c11 -D_POSIX_C_SOURCE $(LIBS)
BINOUT=jasmcli
BINOUT_DEBUG=jasmcli-debug

####    SOURCES & RULES    ####

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT) $(OBJS)

debug: $(OBJS)
	$(CC) $(CFLAGS_DEBUG) $(DEBUG) -o $(BINOUT_DEBUG) $(OBJS)

clean:
	rm -fv *.o

#make * (opt) means all
.PHONY: install,clean,debug
