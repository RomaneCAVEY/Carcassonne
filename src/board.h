#ifndef __BOARD_H__
#define __BOARD_H__

#include "deck.h"

struct board_t;

struct board_t* board_init(struct tile_t tile);

int board_add(struct board_t* board, struct tile_t tile, int x, int y);

struct tile_t board_get(struct board_t* board, int x, int y);

void board_free(struct board_t* board);

#endif // __BOARD_H__
