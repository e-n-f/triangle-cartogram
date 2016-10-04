PREFIX ?= /usr/local
MANDIR ?= $(PREFIX)/share/man/man1/
BUILDTYPE ?= Release
SHELL = /bin/bash

# inherit from env if set
CC := $(CC)
CXX := $(CXX)
CFLAGS := $(CFLAGS)
CXXFLAGS := $(CXXFLAGS) -std=c++11
LDFLAGS := $(LDFLAGS)
WARNING_FLAGS := -Wall -Wshadow
RELEASE_FLAGS := -O3 -DNDEBUG
DEBUG_FLAGS := -O0 -DDEBUG -fno-inline-functions -fno-omit-frame-pointer

ifeq ($(BUILDTYPE),Release)
	FINAL_FLAGS := -g $(WARNING_FLAGS) $(RELEASE_FLAGS)
else
	FINAL_FLAGS := -g $(WARNING_FLAGS) $(DEBUG_FLAGS)
endif

all: eq

PG=

ALL_H = $(shell find . '(' -name '*.h' -o -name '*.hpp' ')')
H = $(wildcard *.h) $(wildcard *.hpp)
C = $(wildcard *.c) $(wildcard *.cpp)

INCLUDES = -I/usr/local/include -I.
LIBS = -L/usr/local/lib

eq: equalize.o
	$(CXX) $(PG) $(LIBS) $(FINAL_FLAGS) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -lm -lz -lsqlite3 -lpthread

%.o: %.c $(ALL_H)
	$(CC) $(PG) $(INCLUDES) $(FINAL_FLAGS) $(CFLAGS) -c -o $@ $<

%.o: %.cpp $(ALL_H)
	$(CXX) $(PG) $(INCLUDES) $(FINAL_FLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f tippecanoe *.o

indent:
	clang-format -i -style="{BasedOnStyle: Google, IndentWidth: 8, UseTab: Always, AllowShortIfStatementsOnASingleLine: false, ColumnLimit: 0, ContinuationIndentWidth: 8, SpaceAfterCStyleCast: true, IndentCaseLabels: false, AllowShortBlocksOnASingleLine: false, AllowShortFunctionsOnASingleLine: false, SortIncludes: false}" $(C) $(H)
