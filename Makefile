IGRAPH_PATH ?= /net/npers/renault/save/igraph-0.10.10/install
IGRAPH_LIBDIR = $(shell [ -e $(IGRAPH_PATH)/lib ] && \
    echo $(IGRAPH_PATH)/lib || \
    echo $(IGRAPH_PATH)/lib64) 

CFLAGS = -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 \
	-I$(IGRAPH_PATH)/include/igraph
LDFLAGS = -ligraph -lm -ldl -L$(IGRAPH_PATH)/lib -Wl,--rpath=${IGRAPH_PATH}/lib

BASE_FILES = \
        deck.c

all: build

build: server client deck.o

%.o: src/%.c
	$(CC) $< $(CFLAGS) -c

server: server.o deck.o tile.o board.o
	gcc $(LDFLAGS) $^ -o install/$@

player0a.so: player0a.o board.o deck.o
	gcc -shared -o $@ $^

player0b.so: player0b.o board.o deck.o
	gcc -shared -o $@ $^

client: player0a.so player0b.so

alltests:

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ *.o

.PHONY: client install test clean

