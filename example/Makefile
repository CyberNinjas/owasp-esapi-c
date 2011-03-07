VPATH = ../include ../lib
INCPATH = -I../include -I../../include

TSRC = access_ref_example.c

# used for the clean target
TOBJ = access_ref_example.o

# used for the clean target
TEXE = access_ref_example

LIBPATH = ../lib
LIB = -lgcrypt -llog4c -lesapi

all: examples

examples: access_ref_example

access_ref_example: access_ref_example.c access_ref.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB) 

clean:
	rm -f $(TOBJ)
	rm -f $(TEXE)
