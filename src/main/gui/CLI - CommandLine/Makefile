#!/usr/bin/make -f

SHELL = /bin/sh

####    CONFIGURATION    ####

CC=gcc
DEBUG = -g
CFLAGS=-Wall -std=c11 $(DEBUG)
LIBS=

####    SOURCES & RULES    ####

OBJS := $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: all
all : clean cli

jasm : $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $@ $(OBJS)


.PHONY: clean
clean :
	rm -f *.o cli
