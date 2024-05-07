IGRAPH_PATH ?= /net/npers/renault/save/igraph-0.10.10/install
IGRAPH_LIBDIR = $(shell [ -e $(IGRAPH_PATH)lib ] && \
    echo $(IGRAPH_PATH)lib || \
    echo $(IGRAPH_PATH)lib64) 

CFLAGS = -std=gnu99 -Wall -Wextra -Wno-trampolines -fPIC -g3 -O0 \
	-I$(IGRAPH_PATH)/include/igraph
LDFLAGS = -ligraph -lm -ldl -L$(IGRAPH_LIBDIR) -Wl,--rpath=${IGRAPH_LIBDIR}

BASE_FILES = \
        deck.c

all: build

build: server client deck.o

%.o: src/%.c
	$(CC) $< $(CFLAGS) -c --coverage

server: server.o deck.o tile.o board.o super_board.o graph.o score.o common.o meeple.o
	gcc $(CFLAGS) $^ -o install/$@ -lgcov --coverage $(LDFLAGS)

player0a.so: player0a.o board.o deck.o tile.o meeple.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl -lgcov --coverage

player0b.so: player0b.o board.o deck.o tile.o meeple.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl -lgcov --coverage

player1.so: player1.o board.o deck.o tile.o graph.o super_board.o meeple.o score.o common.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl -lgcov --coverage $(LDFLAGS)

player2.so: player2.o board.o deck.o tile.o graph.o super_board.o meeple.o score.o common.o
	gcc $(CFLAGS) -shared -o install/$@ $^ -ldl -lgcov --coverage $(LDFLAGS)

client: player0a.so player0b.so player1.so player2.so

test_tile.o: src/test/test_tile.c
	$(CC) src/test/test_tile.c $(CFLAGS) -c

test_board.o: src/test/test_board.c
	$(CC) src/test/test_board.c $(CFLAGS) -c

test_deck.o: src/test/test_deck.c
	$(CC) src/test/test_deck.c $(CFLAGS) -c

test_common.o: src/test/test_common.c
	$(CC) src/test/test_common.c $(CFLAGS) -c

test_score.o: src/test/test_score.c
	$(CC) src/test/test_score.c $(CFLAGS) -c

alltests: src/test/alltests.c test_deck.o test_tile.o test_board.o tile.o board.o deck.o test_common.o common.o test_score.o score.o super_board.o graph.o meeple.o
	gcc $(CLFAGS) $^ -o install/$@ -lgcov --coverage -ldl $(LDFLAGS)

test: alltests

install: server client test

clean:
	@rm -f *~ src/*~ *.o *.gcno *.gcda *.gcov

.PHONY: client install test clean



