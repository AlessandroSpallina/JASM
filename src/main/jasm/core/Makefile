#!/usr/bin/make -f

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g
#-DLOGPATH=/var/log/jasm.log
CFLAGS=-O2 -pipe -Wall -std=c11 -D_XOPEN_SOURCE=700 $(DEBUG)
LIBS=
MODULES_SRC_DIRECTORY=../modules
BINOUT=jasm
####    SOURCES & RULES    ####

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS_MODULES:=$(patsubst %c,%o,$(wildcard $(MODULES_SRC_DIRECTORY)/*.c))

install:$(OBJS_MODULES) $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BINOUT) $(OBJS) $(OBJS_MODULES)

clean:
	rm -fv *.o
	rm -fv $(MODULES_SRC_DIRECTORY)/*.o

.PHONY: install,clean
