#!/usr/bin/make -f
#Stefano Belli, <stefano9913@gmail.com>

SHELL=/bin/sh

CC=gcc
LIBS=$(shell pkg-config --cflags gtk+-2.0 --libs gtk+-2.0) 
DEBUG=-g
BINOUT=jasmgtk
CFLAGS=-O2 -pipe -std=c11 -Wall $(LIBS) -o $(BINOUT) $(DEBUG)

OBJS:=$(patsubst %.c,%.o,$(wildcard *.c))

install: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS)

clean:
	rm -fv *.o

.PHONY: install,clean
