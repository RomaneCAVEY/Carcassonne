IGRAPH_PATH ?= /net/npers/renault/save/igraph-0.10.10/install
IGRAPH_LIBDIR = $(shell [ -e $(IGRAPH_PATH)/lib ] && \
    echo $(IGRAPH_PATH)/lib || \
    echo $(IGRAPH_PATH)/lib64) 

CFLAGS = -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 -O0 \
	-I$(IGRAPH_PATH)/include/igraph
LDFLAGS = -ligraph -lm -ldl -L$(IGRAPH_LIBDIR) -Wl,--rpath=${IGRAPH_LIBDIR}

BASE_FILES = \
        deck.c

all: build

build: server client deck.o

%.o: src/%.c
	$(CC) $< $(CFLAGS) -c

server: server.o deck.o tile.o board.o
	gcc $(CFLAGS) $^ -o install/$@  $(LDFLAGS)

player0a.so: player0a.o board.o deck.o tile.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl

player0b.so: player0b.o board.o deck.o tile.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl

player1.so: player1.o board.o deck.o tile.o graph.o super_board.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl $(LDFLAGS)

player2.so: player2.o board.o deck.o tile.o graph.o super_board.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl $(LDFLAGS)

client: player0a.so player0b.so player1.so player2.so

test_tile.o:
	$(CC) src/test/test_tile.c $(CFLAGS) -c

test_tile: tile.o test_tile.o
	gcc $(CFLAGS) $^ -o install/$@

test_board.o :
	$(CC) src/test/test_board.c $(CFLAGS) -c

test_board: board.o tile.o test_board.o
	gcc $(CFLAGS) $^ -o install/$@

test_desk.o :
	$(CC) src/test/test_desk.c $(CFLAGS) -c

test_deck: deck.o tile.o test_desk.o
	gcc $(CFLAGS) $^ -o install/$@



alltests: test_deck test_tile test_board

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ *.o

.PHONY: client install test clean



