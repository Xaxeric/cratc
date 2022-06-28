# Compiler
CC = gcc

# Source file
SRC = cratc.c

# Includes and libs
INCS = `pkg-config --cflags ncurses`

# Prefix path
PREFIX = /usr/local

# Compiler flags
CFLAGS = -O2 -Wall -Wextra
LDFLAGS = -lncursesw
CRATCCFLAGS = $(INCS) $(CFLAGS)
CRATCLDFLAGS = $(LIBS) $(LDFLAGS)
