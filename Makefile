#
#
#

prefix     = /usr
libdir     = ${prefix}/lib
includedir = ${prefix}/include/ndpi
DIST       = ./dist
PROGRAM    = ${DIST}/gcapmon
CC         = gcc
CFLAGS     += -Dlinux -I../nDPI/src/include -I../nDPI/example -Wall -g -O2
HEADERS    = $(wildcard ../nDPI/src/include/*.h)
OBJECTS    = ../nDPI/src/lib/libndpi.a

$(PROGRAM): $(HEADERS) $(OBJECTS) src/main.c
	$(CC) $(CFLAGS) -o $(PROGRAM) src/main.c ../nDPI/src/lib/libndpi.a -lgcrypt

run: $(PROGRAM)
	$(PROGRAM)
