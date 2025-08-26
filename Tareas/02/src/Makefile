# Linux Makefile
# Socket class
#

CC  := gcc
CXX := g++
CFLAGS := -g -c
CXXFLAGS := -g -c
DEPS := Socket.h
TARGETS := ipv6-udp-client.out ipv6-udp-server.out

ALL := $(TARGETS)
$(info $$TARGETS = $(TARGETS))

all: $(TARGETS)

%.o: %.cc $(DEPS)
	$(CXX) $(CXXFLAGS) $<

%.out: %.o Socket.o VSocket.o
	$(info $$^ = $^)
	$(CXX) -g $^ VSocket.o Socket.o -o $^

ipv6-udp-client.out: ipv6-udp-client.o VSocket.o Socket.o
	$(CXX) -g ipv6-udp-client.o VSocket.o Socket.o -o $@

ipv6-udp-server.out: ipv6-udp-server.o VSocket.o Socket.o
	$(CXX) -g ipv6-udp-server.o VSocket.o Socket.o -o $@

clean:
	rm -f *.o *.out

