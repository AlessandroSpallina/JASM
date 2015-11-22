#!/usr/bin/make -f

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g
CFLAGS=-O2 -pipe -Wall -std=c11 $(DEBUG)
LIBS=
BINOUT=jasmcli

####    SOURCES & RULES    ####

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BINOUT) $(OBJS)

clean:
	rm -fv *.o 

#make * (opt) means all
.PHONY: install,clean
