# Makefile for Project 1

# Tools
CC := gcc

# Directories
SRCDIR := src
BINDIR := bin
BUILDDIR := build
TARGET := proj1

# GCC Debug Flags
DEBUGFLAGS := -g3 -O0 -Wall -pg
RTCHECKS := -fmudflap -fstack-check -gnato
# GCC Production Flags
PRODFLAGS := -Wall -O2
# Active Flags
CFLAGS := -std=gnu99 $(DEBUGFLAGS)
LINK := $(DEBUGFLAGS)

# File paths
SRCEXT := c
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Targets

all: $(OBJECTS)
	@echo " Linking..."; $(CC) $(LINK) $(OBJECTS) -o $(BINDIR)/$(TARGET)
 
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@echo " CC $<"; $(CC) $(CFLAGS) -c -o $@ $<
	
clean:
	@echo "Cleaning...";
	rm -rf $(BUILDDIR) $(BINDIR)/$(TARGET)*

.PHONY: all clean


