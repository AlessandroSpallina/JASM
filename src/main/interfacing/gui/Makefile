#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

SHELL=/bin/sh

CC=gcc
LIBS=$(shell pkg-config --cflags gtk+-2.0 --libs gtk+-2.0)
DEBUG=-g -DDEBUG
BUILD_DATE_GUI=$(shell LANG="en_US.UTF-8" date "+%a %d/%m/%Y %H:%M")
BINOUT=jasmgtk
BINOUT_DEBUG=jasmgtk-debug
CFLAGS=-O2 -pipe -std=c11 -Wall -DBUILD_DATE_GUI="$(BUILD_DATE_GUI)" $(LIBS)
CFLAGS_DEBUG=-std=c11 -Wall -DBUILD_DATE_GUI="$(BUILD_DATE_GUI)" $(DEBUG) $(LIBS)

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(OBJS)
	$(CC) $(CFLAGS) -o $(BINOUT) $(OBJS)

debug: $(OBJS)
	$(CC) $(CFLAGS_DEBUG) -o $(BINOUT_DEBUG) $(OBJS)

clean:
	rm -fv *.o

.PHONY: install,clean,debug
