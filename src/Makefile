srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = /usr/lib
oldlibdir = ${prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
oldincludedir = ${prefix}/include
includedir = /usr/include

CC = gcc
RANLIB = ranlib
AR = ar
ARFLAGS = cru
RM = rm -f
CP = cp
MV = mv
LN = ln

CFLAGS = -O2 -g -Wall -Wno-pointer-sign -Wno-unused-result -Wno-unused-but-set-variable -I.

SHCFLAGS = -O2 -g -Wall -Wno-pointer-sign -Wno-unused-result -Wno-unused-but-set-variable -I. -fPIC

SHFLAGS = -shared -fPIC -Wl,-soname,libinfostd.so.1 -Wl,-rpath,/usr/local/lib

SOURCES = dyn_str.c dyn_tab.c regexp.c dyn_strs.c crc32.c base64.c hash_str.c hash_long.c heap.c tokenizer.c speller.c stemmer.c fsm.c queue.c

OBJS = dyn_str.o dyn_tab.o regexp.o dyn_strs.o crc32.o base64.o hash_str.o hash_long.o heap.o tokenizer.o speller.o stemmer.o fsm.o queue.o

SHOBJS = dyn_str.so dyn_tab.so regexp.so dyn_strs.so crc32.so base64.so hash_str.so hash_long.so heap.so tokenizer.so speller.so stemmer.so fsm.so queue.so

HEADERS = infostd.h

.SUFFIXES:	.so

all: libinfostd.a libinfostd.so

.c.so:
	${RM} $@
	$(CC) -c $(SHCFLAGS) -o $*.o $<
	$(MV) $*.o $@

libinfostd.a: ${OBJS} ${HEADERS}
	$(AR) $(ARFLAGS) libinfostd.a ${OBJS}
	$(RANLIB) libinfostd.a

libinfostd.so: ${SHOBJS} ${HEADERS}
	${CC} ${SHFLAGS} -o libinfostd.so.1 ${SHOBJS} ${LDFLAGS}

install: libinfostd.a libinfostd.so.1
	if test ! -d ${libdir}; then mkdir ${libdir}; fi
	/usr/bin/install -c -m 444 libinfostd.a ${libdir}/libinfostd.a
	/usr/bin/install -c -m 755 libinfostd.so.1 ${libdir}/libinfostd.so.1
	$(LN) -sf ${libdir}/libinfostd.so.1 ${libdir}/libinfostd.so
	if test ! -d ${includedir}; then mkdir ${includedir}; fi
	/usr/bin/install -c -m 444 infostd.h ${includedir}/infostd.h

clean:
	rm -f *.o *.a *.so* *~ core
