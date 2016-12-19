# Makefile for Xenomai applications
# Based on the work of Paulo Pedreiras, Jul/2013
# and Goncalo Pardal, Nov/2013+
# Also based on http://stackoverflow.com/a/1484873
#    - added -Xlinker -rpath -Xlinker /usr/xenomai/lib to Makefile to eliminate
#      issue related with user space libs incompatible with kernel space ones

# Check if PATH is configured correctly
prefix :=$(shell xeno-config --prefix)

ifeq ($(prefix),)
$(error Please add <xeno-install-path>/bin to your PATH variable)
endif

# Get compiler and linker flags
TARGET = simulator
CC := $(shell xeno-config --cc)
CFLAGS := $(shell xeno-config --skin=native --cflags) $(shell sdl2-config --cflags)
LIBS := -lm -Xlinker -rpath -Xlinker /usr/xenomai/lib $(shell sdl2-config --libs)
LDFLAGS := -lnative $(shell xeno-config --skin=native --ldflags)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

default: $(OBJECTS)
# Compile application

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) $(LDFLAGS) -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
