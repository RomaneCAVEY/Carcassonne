#ifndef __BOARD_H__
#define __BOARD_H__

#include <igraph.h>
#include "deck.h"

struct coord_t {
  int x, y;
};

struct board_t {
  igraph_t graph;
  struct coord_t* coord;
  unsigned int* last_node;
};

struct board_t init_board(struct tile_t tile);

void board_add_tile(struct board_t *board, struct tile_t tile);

void destroy_board(struct board_t board);

#endif // __BOARD_H__
