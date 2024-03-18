IGRAPH_PATH ?= /net/npers/renault/save/igraph-0.10.10/install
CFLAGS = -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 \
	-I$(IGRAPH_PATH)/include/igraph
LDFLAGS = -ligraph -lm -ldl -L$(IGRAPH_PATH)/lib -Wl,--rpath=${IGRAPH_PATH}/lib

BASE_FILES = \
        deck.c

all: build

build: server client deck.o

%.o: src/%.c
	$(CC) $< $(CFLAGS) -c

server:

client:

alltests:

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ *.o

.PHONY: client install test clean
