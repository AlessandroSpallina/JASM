#!/usr/bin/make -f

SHELL=/bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG=
CFLAGS=-O2 -pipe -Wall -std=c11 $(DEBUG)
LIBS=
BINOUT=jasm

####    SOURCES & RULES    ####

#OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install:#$(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(BINOUT) *.c 

clean:
	rm -fv *.o

.PHONY: install,clean
