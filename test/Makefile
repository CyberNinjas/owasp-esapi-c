CC=gcc
#CFLAGS=-Wall -O2 -std=c99
CFLAGS=-Wall -g -std=c99 -D__VERSION_MAJOR_NUMBER=1 -D__VERSION_MINOR_NUMBER=0 -D__BUILD_NUMBER=1
MAKE = make
SUBDIRS = src

all: test runtests
	
test:
	(cd src; $(MAKE) CC="$(CC)" CFLAGS="$(CFLAGS)" test)

runtests:
	(cd src; $(MAKE) runtests)
	
clean:
	(cd src; $(MAKE) clean)

