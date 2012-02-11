VPATH = ../include ../../include ../../lib
LIBPREFIX=lib
STATICLIBSUFFIX=.a
SHAREDLIBSUFFIX=.so.1
D_SRC=.
D_INC=../include
D_OBJ=../obj
D_LIB=../lib
TARGET=esapi
STATICOUT=$(LIBPREFIX)esapi$(STATICLIBSUFFIX)
SHAREDOUT=$(LIBPREFIX)esapi$(SHAREDLIBSUFFIX)

SOURCES = access_ref.c base64.c authenticator.c authorization.c \
          sec_context.c canonicalize.c codec.c properties.c \
          crypto.c validate.c log.c unix_codec.c windows_codec.c \
          executor.c user.c local_strdup.c

LIBRARIES= -lgcrypt -llog4c

OBJECTS = access_ref.o base64.o authenticator.o authorization.o \
          sec_context.o canonicalize.o codec.o properties.o \
          crypto.o validate.o log.o unix_codec.o windows_codec.o \
          executor.o user.o local_strdup.o

all: $(SOURCES) lib

lib: $(STATICOUT) $(SHAREDOUT)

$(STATICOUT): $(OBJECTS)
	ar rcs $(STATICOUT) $(OBJECTS)
	mv $(STATICOUT) $(D_LIB)

$(SHAREDOUT): $(OBJECTS)
	$(CC) -shared -o $(SHAREDOUT) $(OBJECTS) $(LIBRARIES)
	mv $(SHAREDOUT) $(D_LIB)

access_ref.o: access_ref.c access_ref.h uthash.h
	$(CC) $(CFLAGS)	-I$(D_INC) access_ref.c

base64.o: base64.c base64.h 
	$(CC) $(CFLAGS)	-I$(D_INC) base64.c

authenticator.o: authenticator.c authenticator.h base64.h esapi.h user.h
	$(CC) $(CFLAGS)	-I$(D_INC) authenticator.c

authorization.o: authorization.c authorization.h user.h
	$(CC) $(CFLAGS)	-I$(D_INC) authorization.c

sec_context.o: sec_context.c local_strdup.c base64.h esapi.h
	$(CC) $(CFLAGS)	-I$(D_INC) sec_context.c

canonicalize.o: canonicalize.c codec.h canonicalize.h 
	$(CC) $(CFLAGS)	-I$(D_INC) canonicalize.c

codec.o: codec.c codec.h
	$(CC) $(CFLAGS)	-I$(D_INC) codec.c

properties.o: properties.c properties.h
	$(CC) $(CFLAGS)	-I$(D_INC) properties.c

crypto.o: crypto.c base64.h crypto.h esapi.h
	$(CC) $(CFLAGS)	-I$(D_INC) crypto.c

validate.o: validate.c validate.h utlist.h codec.h esapi.h
	$(CC) $(CFLAGS)	-I$(D_INC) validate.c

log.o: log.c log.h user.h
	$(CC) $(CFLAGS) -I$(D_INC) log.c

executor.o: executor.c executor.h log.h user.h codec.h
	$(CC) $(CFLAGS) -I$(D_INC) executor.c

unix_codec.o: unix_codec.c unix_codec.h 
	$(CC) $(CFLAGS) -I$(D_INC) unix_codec.c

windows_codec.o: windows_codec.c windows_codec.h
	$(CC) $(CFLAGS) -I$(D_INC) windows_codec.c

user.o: user.c user.h
	$(CC) $(CFLAGS) -I$(D_INC) user.c

local_strdup.o: local_strdup.c esapi.h
	$(CC) $(CFLAGS) -I$(D_INC) local_strdup.c

clean:
	-rm -f $(OBJECTS) $(D_LIB)/libesapi\.* core *.core
	-rm -rf $(OUT)
