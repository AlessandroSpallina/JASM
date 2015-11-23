#!/usr/bin/make -f

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=-g
CFLAGS=-O2 -pipe -Wall -std=c11 -D_XOPEN_SOURCE=700 $(DEBUG)
LIBS=
BINOUT=jasm

####    SOURCES & RULES    ####

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install:$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BINOUT) $(OBJS) 

clean:
	rm -fv *.o

.PHONY: install,clean
