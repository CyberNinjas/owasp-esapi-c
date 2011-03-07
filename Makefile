MAKE = make
CC=gcc
#CFLAGS=-std=c99 -Wall -pedantic -O2 -c
CFLAGS=-std=c99 -Wall -pedantic -g -c
SRCSUBDIR = src
TESTSUBDIR = test
EXAMPLESUBDIR = example
SUBDIRS = ${SRCSUBDIR} ${TESTSUBDIR}

all: lib test 

lib: always
	(cd $(SRCSUBDIR);  $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" all)

install:
	(cp lib/*.so.1 /usr/local/lib)
	
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
