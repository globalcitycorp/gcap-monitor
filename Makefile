#
# Makefile
#

DIST       = ./dist
PROGRAM    = ${DIST}/gcapmon
CC         = g++
CFLAGS     += -std=c++11 -Dlinux -I../nDPI/src/include -I../nDPI/example -I/usr/local/include/pcapplusplus -L../nDPI/src/lib -Wall -g -O2
HEADERS    = $(wildcard ../nDPI/src/include/*.h) $(wildcard src/gcap/*.hpp) $(wildcard src/gcap/flow/*.hpp)
OBJECTS    = src/gcap/util.o src/gcap/logger.o src/gcap/ip_flow_key.o src/gcap/flow/base_flow.o src/gcap/flow/ip4_tcp_flow.o src/gcap/host_store.o src/gcap/flow_store.o src/gcap/pcap_base_processor.o src/gcap/pcap_file_processor.o

all: $(PROGRAM)

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(PROGRAM): $(HEADERS) $(OBJECTS) src/main.cpp
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJECTS) src/main.cpp -l:libndpi.a -lgcrypt -l:libPcap++.a -l:libPacket++.a -l:libCommon++.a -lpcap

run: $(PROGRAM)
	$(PROGRAM)

clean:
	rm -f $(PROGRAM) ./src/*.o ./src/gcap/*.o ./src/gcap/flow/*.o
