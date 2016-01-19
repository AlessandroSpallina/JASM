#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g -DDEBUG
#-DLOGPATH=/var/log/jasm.log
LIBS=
BUILD_DATE_CORE="$(shell LANG="en_US.UTF-8" date "+%a %d/%m/%Y %H:%M")"
CFLAGS=-fPIC -O2 -pipe -DBUILD_DATE_CORE=\"$(BUILD_DATE_CORE)\" -Wall -std=c11 -pthread -D_XOPEN_SOURCE=700 $(LIBS)
CFLAGS_DEBUG=-fPIC -Wall -pthread -std=c11 -DBUILD_DATE_CORE=\"$(BUILD_DATE_CORE)\" -D_XOPEN_SOURCE=700 $(DEBUG) $(LIBS)
SHARED_OBJECTS_FLAGS=-shared
SHARED_OBJECT=libjasm.so
OBJECTS=modules_list.o modules.o miscellaneous.o
MODULES_SRC_DIRECTORY=../modules
BINOUT=jasm
BINOUT_DEBUG=jasm-debug
####    SOURCES & RULES    ####

debug: CFLAGS=$(CFLAGS_DEBUG)

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))
OBJS_MODULES:=$(patsubst %c,%o,$(wildcard $(MODULES_SRC_DIRECTORY)/*.c))


install:$(OBJS_MODULES) $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT) $(OBJS) $(OBJS_MODULES)
	$(CC) $(SHARED_OBJECTS_FLAGS) -o $(SHARED_OBJECT) $(OBJECTS)

debug:$(OBJS_MODULES) $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT_DEBUG) $(OBJS) $(OBJS_MODULES)
	$(CC) $(SHARED_OBJECTS_FLAGS) -o $(SHARED_OBJECT) $(OBJECTS)

clean:
	rm -fv *.o
	rm -fv $(MODULES_SRC_DIRECTORY)/*.o

.PHONY: install,clean,debug
