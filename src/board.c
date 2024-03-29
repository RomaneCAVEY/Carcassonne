#include "board.h"

#define BOARD_SIZE 201
#define BOARD_CENTER 101

struct board_t {
  struct tile_t** tiles;
};

struct board_t* board_init(struct tile_t tile) {

  struct board_t *board = malloc(sizeof(struct board_t));

  for (int i = 0; i < BOARD_SIZE; i++) {
    board->tiles[i] = malloc(sizeof(struct tile_t) * 201);
    for (int j = 0; j < BOARD_SIZE; j++) {
      board->tiles[i][j] = CARC_TILE_EMPTY;
    }
  }

  board->tiles[BOARD_CENTER][BOARD_CENTER] = tile;

  return board;
}

int board_add(struct board_t* board, struct tile_t tile, int x, int y) {
  if (board->tiles[BOARD_CENTER + x][BOARD_CENTER + y] != CARC_TILE_EMPTY)
    return 0;

  board->tiles[BOARD_CENTER + x][BOARD_CENTER + y] = tile;

  return 1;
}

struct tile_t board_get(struct board_t* board, int x, int y) {
  return board->tiles[BOARD_CENTER + x][BOARD_CENTER + y];
}

void board_free(struct board_t* board) {
  for (int i = 0; i < BOARD_SIZE; i++) {
    free(board->tiles[i]);
  }
  free(board);
  board = NULL;
}


