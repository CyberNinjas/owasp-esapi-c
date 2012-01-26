MAKE = make
CC=gcc

# Debug builds
CFLAGS  += -O0 -g3 -ggdb -DDEBUG=1

# Release builds
# CFLAGS=-O2 -g -DNDEBUG=1

# Common to all builds
CFLAGS  += -std=c99 -Wall -Wextra -Wno-unused -Wformat=2 -Wformat-security -fstack-protector -pedantic -fPIC -c

# Clang with Regehr (et al) Integer Overflow Checker (IOC, http://embed.cs.utah.edu/ioc/)
# CFLAGS  +=-fcatch-undefined-c99-behavior

# Linker hardening
LDFLAGS  = -Wl,-z,nodlopen -Wl,-z,nodldump -Wl,-z,relro -Wl,-z,now

SRCSUBDIR = src
TESTSUBDIR = test
EXAMPLESUBDIR = example
SUBDIRS = ${SRCSUBDIR} ${TESTSUBDIR}

# Default prefix for make install and uninstall. The names and default values are taken from
# Stallman's GNU Make, Chapter 14, Section 4, Variables for Installation Directories
ifeq ($(prefix),)
  prefix = /usr/local
endif

ifeq ($(exec_prefix),)
  exec_prefix = $(prefix)
endif

ifeq ($(bindir),)
  bindir = $(exec_prefix)/bin
endif

ifeq ($(libdir),)
  libdir = $(exec_prefix)/lib
endif

ifeq ($(includedir),)
  includedir = $(prefix)/include
endif

all: lib test 

lib: always
	(cd $(SRCSUBDIR);  $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" all)

install: directories
	(cp lib/*.so.1 $(libdir))
	cp -r include/* $(includedir)/esapi-c

uninstall:
	-rm $(libdir)/libesapi.so.1
	-rm -rf $(includedir)/esapi-c
	
directories:
	-mkdir -p $(includedir)/esapi-c
	
test: always
	(cd $(TESTSUBDIR);  $(MAKE) all)

runtests:
	(cd $(TESTSUBDIR)/$(SRCSUBDIR); $(MAKE) runtests)
	
examples:
	(cd $(EXAMPLESUBDIR); $(MAKE) all)
	
docs:
	doxygen Doxyfile
	
clean: 
	(cd $(SRCSUBDIR); $(MAKE) clean)
	(cd $(TESTSUBDIR); $(MAKE) clean)
	(cd $(EXAMPLESUBDIR); $(MAKE) clean)

always:
