# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Wextra -ggdb3

# Directories
SRCDIR = src
BINDIR = bin
BUILDDIR = build
INCDIR = include
LIBDIR = lib

# Targets
CLIENT = $(BINDIR)/client
SERVER = $(BINDIR)/server

# Source and object files
COMMON_SRCS = $(wildcard $(SRCDIR)/common/*.c)
CLIENT_SRCS = $(wildcard $(SRCDIR)/client/*.c)
SERVER_SRCS = $(wildcard $(SRCDIR)/server/*.c)

COMMON_OBJS = $(patsubst $(SRCDIR)/common/%.c, $(BUILDDIR)/common/%.o, $(COMMON_SRCS))
CLIENT_OBJS = $(patsubst $(SRCDIR)/client/%.c, $(BUILDDIR)/client/%.o, $(CLIENT_SRCS))
SERVER_OBJS = $(patsubst $(SRCDIR)/server/%.c, $(BUILDDIR)/server/%.o, $(SERVER_SRCS))

# Include directories
INCLUDES = -I$(INCDIR)/common -I$(INCDIR)/client -I$(INCDIR)/server

.PHONY: all clean

all: $(CLIENT) $(SERVER)

# Client target
$(CLIENT): $(COMMON_OBJS) $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Server target
$(SERVER): $(COMMON_OBJS) $(SERVER_OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Generic rule for building objects
$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(BUILDDIR)/*
