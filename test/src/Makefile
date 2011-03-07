VPATH = ../include ../../include ../../lib
INCPATH = -I../include -I../../include

TSRC = access_ref_test.c canonicalize_test.c executor_test.c sec_context_test.c base64_test.c crypto_test.c log_test.c validate_test.c properties_test.c

# used for the clean target
TOBJ = access_ref_test.o canonicalize_test.o executor_test.o sec_context_test.o base64_test.o crypto_test.o log_test.o validate_test.o properties_test.o

# used for the clean target
TEXE = access_ref_test canonicalize_test executor_test sec_context_test base64_test crypto_test log_test validate_test properties_test

LIBPATH = ../../lib
LIB = -lgcrypt -llog4c -lesapi

all: test runtests

test: access_ref_test canonicalize_test executor_test sec_context_test base64_test crypto_test log_test validate_test properties_test

access_ref_test: access_ref_test.c minunit.h access_ref.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB) 

canonicalize_test: canonicalize_test.c minunit.h canonicalize.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

executor_test: executor_test.c minunit.h executor.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

sec_context_test: sec_context_test.c minunit.h esapi.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

base64_test: base64_test.c minunit.h base64.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

crypto_test: crypto_test.c minunit.h crypto.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

log_test: log_test.c minunit.h log.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

validate_test: validate_test.c minunit.h validate.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

properties_test: properties_test.c minunit.h properties.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

user_test: user_test.c minunit.h user.h
	$(CC) $(CFLAGS) $(INCPATH) $@.c -o $@ -L$(LIBPATH) $(LIB)

runtests:
	for i in *_test ; do echo $$i: ; ./$$i ; done	

clean:
	rm -f $(TOBJ)
	rm -f $(TEXE)
