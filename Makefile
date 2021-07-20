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
OBJECTS    = $(wildcard ../nDPI/src/lib/*.o) $(wildcard ../nDPI/src/lib/protocols/*.o) $(wildcard ../nDPI/src/lib/third_party/src/*.o)

$(PROGRAM): $(HEADERS) $(OBJECTS) $(DIST)
	$(CC) $(CFLAGS) src/main.c -o $(PROGRAM)

run: $(PROGRAM)
	$(PROGRAM)
